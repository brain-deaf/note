#include <iostream>
#include "sampler.h"
#include "midi.h"


Sampler::Sampler(int* argc, char *** argv) :
    chans(),
    volume(1.0),bus_watch_id(0) {

    gst_init(argc, argv);
    loop = g_main_loop_new(nullptr, false);

    pipeline = gst_pipeline_new("audio-player");
    //vol = gst_element_factory_make("volume", "master-volume");
    mixer = gst_element_factory_make("adder", "mixer");
    sink = gst_element_factory_make("autoaudiosink", "sink"); 
    
    //g_object_set(G_OBJECT(vol), "volume", volume, nullptr);

    if (!pipeline)  std::cerr << "pipeline could not be created\n";
    if (!sink)      std::cerr << "sink could not be created\n";
    if (!mixer)     std::cerr << "mixer could not be created\n";
    //if (!vol)       std::cerr << "master-volume could not be created\n";

    GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, this);
    gst_object_unref (bus);

    gst_bin_add_many(GST_BIN(pipeline),mixer,sink,nullptr);
    gst_element_link(mixer,sink);
}

Sampler::~Sampler() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    if (pipeline != nullptr){
        gst_object_unref(GST_OBJECT(pipeline));
    }
    g_source_remove(bus_watch_id);
    g_main_loop_quit (loop);
    g_main_loop_unref(loop);
}

void Sampler::set_volume(double v) {
    volume = v;
    //g_object_set(G_OBJECT(vol), "volume", volume, nullptr);

} 


gboolean Sampler::bus_call(GstBus* bus, GstMessage* msg, gpointer data){
    Sampler * s = static_cast<Sampler*>(data);
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:
            s->debug();
            s->ready();
            s->quit();
            break;
        case GST_MESSAGE_ERROR:
            gchar  *debug;
            GError *error;
            gst_message_parse_error (msg, &error, &debug);
            g_free (debug);
            g_printerr ("Error: %s\n", error->message);
            g_error_free (error);
            s->quit();
            break;
        default:
             break;
    }
    return true;
}
    
void Sampler::add_chan(Chan * chan) {
    chans[chan->get_name()] = chan;
    chan->add_bin(pipeline);
    GstPad * sinkpad = gst_element_get_request_pad(mixer,"sink_%u");
    auto r = gst_pad_link(chan->source_pad(), sinkpad);
    if(r) {
        std::cerr<< "error code: " << r << std::endl;
    }
    gst_object_unref(sinkpad);
}
