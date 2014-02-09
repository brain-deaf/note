#include <algorithm>
#include <iterator>
#include <iostream>
#include <memory>
#include <future>
#include "Player.h"
#include "Time.h"
#include "Exceptions.h"

/////////////////////////////////////
//Player::Backend inner class definition
/////////////////////////////////////

Player::Backend::Backend(Player * player):
    pipeline_(),
    appsrc_(),
    conv_(),
    audiosink_(),
    loop_(),
    push_id_(),
    bus_watch_id_(),
    player_(player) {
    if(!gst_is_initialized()) {
        GError *err;
        if(!gst_init_check(nullptr,nullptr,&err)) {
            std::exit(err->code);
        }
    }

    pipeline_ = gst_pipeline_new ("pipeline");

    if(pipeline_==nullptr) {
        std::exit(EXIT_FAILURE);
    };

    build_gst_element(appsrc_,"appsrc","source");
    build_gst_element(conv_,"audioconvert","conv");

    GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline_));
    bus_watch_id_ = gst_bus_add_watch (bus, wrap_bus_callback, this);
    gst_object_unref (bus);
    
    GstCaps * caps = gst_caps_new_simple(
        "audio/x-raw",
        "format", G_TYPE_STRING, format_,
        "rate", G_TYPE_INT, Config::sample_rate,
        "channels",G_TYPE_INT, Config::channels,
        "signed", G_TYPE_BOOLEAN, TRUE,
        "layout", G_TYPE_STRING, "interleaved",
        nullptr);

    g_object_set(G_OBJECT(appsrc_),"caps",caps,nullptr);
    g_object_set(G_OBJECT(appsrc_),"is-live",true,nullptr);
    g_object_set(G_OBJECT(appsrc_),"min-latency",0,nullptr);
    g_object_set(G_OBJECT(appsrc_),"emit-signals",false,nullptr);
    g_object_set(G_OBJECT(appsrc_),"format",GST_FORMAT_TIME,nullptr);

    //the gstreamer main loop is the main event loop for audio generation
    loop_ = g_main_loop_new (nullptr, FALSE);

    gst_bin_add_many (GST_BIN (pipeline_), appsrc_, conv_, nullptr);
    gst_element_link (appsrc_, conv_);

    GstAppSrcCallbacks callbacks = {wrap_need_data, wrap_enough_data, wrap_seek_data};
    gst_app_src_set_callbacks(GST_APP_SRC(appsrc_), &callbacks, this, nullptr);
}

Player::Backend::~Backend() {
    gst_element_set_state (pipeline_, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline_));
    g_main_loop_unref (loop_);
}

void Player::Backend::build_gst_element(GstElement * &element, const char * kind, const char * name) {
    element = gst_element_factory_make(kind,name);
    if(element==nullptr) {
        std::exit(EXIT_FAILURE);
    }
}

void Player::Backend::wrap_need_data(GstAppSrc * element, guint length, gpointer instance) {
    Backend * this_ = static_cast<Backend *>(instance);
    this_->need_data(length);
}

void Player::Backend::wrap_enough_data(GstAppSrc * src, gpointer instance) {
    Backend * this_ = static_cast<Backend *>(instance);
    this_->enough_data();
}

gboolean Player::Backend::wrap_seek_data(GstAppSrc * element, guint64 destination, gpointer instance) {
    Backend * this_ = static_cast<Backend *>(instance);
    return this_->seek_data(Offset(destination));
}

gboolean Player::Backend::wrap_push_data(gpointer instance) {
    Backend * this_ = static_cast<Backend *>(instance);
    return this_->player_->push_data();
}

bool Player::Backend::push_data(Packet& data, Offset current_offset) {
    GstBuffer * buffer = gst_buffer_new_allocate(
        nullptr, data.size()*Config::word_size, nullptr);
    Offset packet_size = Offset(data.size());

    GST_BUFFER_PTS(buffer) = current_offset.to_time(Config::sample_rate).value();
    GST_BUFFER_DURATION(buffer) = packet_size.to_time(Config::sample_rate).value();
    GST_BUFFER_OFFSET(buffer) = current_offset.value();
    GST_BUFFER_OFFSET_END(buffer) = (current_offset+packet_size).value();

    auto size = data.size() * Config::word_size;
    auto rsize = gst_buffer_fill(buffer, 0, static_cast<void *>(data.data()), size);
    assert(size==rsize);

    auto ret = gst_app_src_push_buffer(GST_APP_SRC(appsrc_), buffer); 
    if ( ret != GST_FLOW_OK) {
        throw BadFlowException("bad flow while pushing buffer");
    }
    return true;
}

gboolean Player::Backend::wrap_bus_callback(GstBus * bus, GstMessage * message, gpointer instance) {
    Backend * this_ = static_cast<Backend *>(instance);
    return this_->bus_callback(bus,message);
}

void Player::Backend::play(Offset end_offset) {
    g_object_set(G_OBJECT(appsrc_),"size",end_offset.value()*Config::word_size,nullptr);
    gst_element_set_state (pipeline_, GST_STATE_PLAYING);
    main_loop_done_ = std::async(std::launch::async,
        [this](){return g_main_loop_run (loop_);});
}

void Player::Backend::eos() {
    GstFlowReturn r = gst_app_src_end_of_stream(GST_APP_SRC(appsrc_));
    if (r!=GST_FLOW_OK) {
        throw BadFlowException("bad flow on end of stream");
    }
}

void Player::Backend::quit() {
    gst_element_set_state(pipeline_, GST_STATE_NULL);
    g_main_loop_quit(loop_);
}

void Player::Backend::need_data(guint length) {
    player_->last_hint_ = Offset(length);
    if(push_id_ == 0) {
        push_id_=g_idle_add((GSourceFunc) Backend::wrap_push_data, this);
    } 
}

void Player::Backend::enough_data() {
    if(push_id_ != 0) {
        g_source_remove(push_id_);
        push_id_=0;
    }
}

gboolean Player::Backend::seek_data(Offset destination) {
    //TODO: IMPLEMENT THIS
    assert(false);
    return true;
}

gboolean Player::Backend::bus_callback(GstBus * bus, GstMessage * message) {
    switch(GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR:
            GError *err;
            gchar *debug;
            gst_message_parse_error(message,&err,&debug);            
            g_print("Error: %s\n",err->message);
            g_error_free(err); 
            g_free(debug);
            break;

        case GST_MESSAGE_EOS:
            quit();
            break;

        default:
            break;
    }
    return true;
}

//////////////////////////////////////
// LocalBackend inner class definition
//////////////////////////////////////
Player::LocalBackend::LocalBackend(Player * player):
    Backend(player) {
    build_gst_element(audiosink_,"autoaudiosink","output");
    gst_bin_add(GST_BIN(pipeline_),audiosink_);
    gst_element_link(conv_, audiosink_);
}

void Player::LocalBackend::wait_until_ready() {
    assert(false);
}

std::string Player::LocalBackend::where(){
    assert(false);
}


///////////////////////////////////////
// VorbisBackend inner class definition
///////////////////////////////////////

Player::VorbisBackend::VorbisBackend(Player * player, const std::string& output_name):
    Backend(player),
    where_(output_name) {
    build_gst_element(vorbisencoder_,"vorbisenc", "encoder");
    build_gst_element(oggmuxer_,"oggmux", "muxer");
    build_gst_element(audiosink_, "filesink", "sink");

    g_object_set(G_OBJECT(audiosink_),"location",output_name.c_str(),nullptr);
    gst_bin_add_many(GST_BIN(pipeline_),vorbisencoder_,oggmuxer_,audiosink_,nullptr);
    gst_element_link_many(conv_,vorbisencoder_,oggmuxer_,audiosink_,nullptr);
}

std::string Player::VorbisBackend::where() {
    return where_;
}

void Player::VorbisBackend::wait_until_ready() {
    main_loop_done_.get(); 
}

/////////////////////////
//Player class definition
/////////////////////////

//init static member variables
const char * Player::format_ = "S16LE";

Player::Player(BackendType backend_type, const Offset& sample_rate_init, const double freq_reference_init, const std::string& output_name):
    sample_rate_(sample_rate_init),
    freq_reference_(freq_reference_init),
    instruments_(),
    current_offset_(),
    end_offset_() {
    switch(backend_type) {
        case BackendType::local:
            backend_ = std::unique_ptr<LocalBackend>(new LocalBackend(this));
            break;
        case BackendType::vorbis:
            backend_ = std::unique_ptr<VorbisBackend>(new VorbisBackend(this,output_name));
            break;
    }
}

void Player::add_instrument(InstrumentHandle instrument_h) {
    instruments_.push_back(instrument_h);
}

Beat Player::get_stream_end() const {
    auto comp_by_stream_end = [](InstrumentHandle l, InstrumentHandle r) 
        -> bool {return l->stream_end() < r->stream_end();};
    return (*std::max_element(
        instruments_.begin(),
        instruments_.end(),
        comp_by_stream_end
    ))->stream_end();
}

void Player::play() {   
    Beat stream_end{get_stream_end()};
    end_offset_ = stream_end.to_offset(Config::tempo, Config::sample_rate);
    backend_->play(end_offset_);
}

std::string Player::wait_until_ready() {
    backend_->wait_until_ready();
    return backend_->where();
}

void Player::eos() {
    backend_->eos();
}

void Player::quit() {
    backend_->quit();
}

Offset Player::sample_rate() const {
    return sample_rate_;
}

double Player::freq_reference() const {
    return freq_reference_;
}

gboolean Player::push_data() {
    if(current_offset_ >= end_offset_) {
        eos();
        return false;
    }
    
    Offset packet_size = ((current_offset_+last_hint_) < end_offset_) ? last_hint_ : end_offset_ - current_offset_ ; 
    Packet data(packet_size.value());

    for (InstrumentHandle instrument_h : instruments_) {
        data+=instrument_h->get_samples(current_offset_,current_offset_+packet_size);
    }
    if (backend_->push_data(data,current_offset_));
    {
        current_offset_+=Offset(data.size());
        return true;
    }
    assert(false);
    return false; //shouldn't happen
}
