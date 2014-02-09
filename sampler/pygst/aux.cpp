#include "player.h"
#include "midi.h"


void MidiInput::mycallback( double deltatime, std::vector< unsigned char > *message, void *userData ){
  unsigned int nBytes = message->size();
  int byte;
  int value;
  Player* my_player = (Player*)userData;

  for ( unsigned int i=0; i<nBytes; i++ ){
     byte = i;
     value = (int)message->at(i);
     std::cout << "Byte " << byte << " = " << value << ", ";
     if (byte == 0 && value == 144){
         my_player->play_sample("test.ogg");
     }
         
  }
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}

int MidiInput::midi_listen(void* player){
  RtMidiIn *midiin = new RtMidiIn();
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 1 );
  midiin->setCallback( &mycallback, player );
  midiin->ignoreTypes( false, false, false );

  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);

 cleanup:
  delete midiin;

  return 0;
}

static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data){
    Player* my_player = (Player*) data;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:{
             g_print ("End of stream\n");
             my_player->pipeline = NULL;
             my_player->pipeline = gst_pipeline_new("audio-player");
             my_player->sink     = gst_element_factory_make ("autoaudiosink", "audio-output");
             my_player->adder    = gst_element_factory_make ("adder", "mixer");
             my_player->vol    = gst_element_factory_make ("volume", "volume-mixer");

             g_object_set(G_OBJECT(my_player->vol), "volume", my_player->volume, NULL);
             printf("set vol: %f\n", my_player->volume);

             gst_bin_add_many(GST_BIN(my_player->pipeline), my_player->adder, my_player->sink, my_player->vol, NULL);
             gst_element_link(my_player->adder, my_player->vol);
             gst_element_link(my_player->vol, my_player->sink);

             my_player->bus = gst_pipeline_get_bus (GST_PIPELINE (my_player->pipeline));
             my_player->bus_watch_id = gst_bus_add_watch (my_player->bus, bus_call, my_player);
             gst_object_unref (my_player->bus);

             break;}
        case GST_MESSAGE_ERROR: {
             gchar  *debug;
             GError *error;
             gst_message_parse_error (msg, &error, &debug);
             g_free (debug);
             g_printerr ("Error: %s\n", error->message);
             g_error_free (error);
             g_main_loop_quit (my_player->loop);
             break;
        }
        default:
             break;
    }
    return TRUE;
}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){
  printf("on pad added\n");
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;
  g_print ("Dynamic pad created, linking demuxer/decoder\n");
  sinkpad = gst_element_get_static_pad (decoder, "sink");
  gst_pad_link (pad, sinkpad);
  if (sinkpad == NULL) printf("error retrieving sinkpad from decoder\n");
  gst_object_unref (sinkpad);
}

Player::Player(){
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);

    gpointer my_player;
    my_player = this;

    count = 0;
    pipeline = gst_pipeline_new("audio-player");
    source   = gst_element_factory_make("filesrc", "file-source");
    sink     = gst_element_factory_make("autoaudiosink", "sink");
    adder    = gst_element_factory_make("adder", "mixer");
    vol      = gst_element_factory_make("volume", "volume-mixer");
    volume = 0.1;
    g_object_set(G_OBJECT(vol), "volume", volume, NULL);

    vol_track1 = NULL;
    vol_track2 = NULL;

    if (!sink) printf("sink could not be created\n");
    if (!adder) printf("adder could not be created\n");
    if (!pipeline) printf("pipeline could not be created\n");
    if (!vol) printf("volume-mixer could not be created\n");

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, my_player);
    gst_object_unref (bus);

    gst_bin_add_many(GST_BIN(pipeline), adder, vol, sink, NULL);
    printf("link adder and volume: %d\n", gst_element_link(adder, vol));
    printf("link volume and sink: %d\n" , gst_element_link(vol, sink));

    midi_in = new MidiInput();
}

void Player::_d_Player(){
    g_print("destructor\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    if (pipeline != NULL){
        gst_object_unref(GST_OBJECT(pipeline));
    }
    g_source_remove(bus_watch_id);
    g_main_loop_quit (loop);
    g_main_loop_unref(loop);
}

void Player::start_main(){
    g_main_loop_run(loop);
}
    
void Player::play_sample(char* sample_name){
    std::string _count = std::to_string(count);

    GstElement* _source   = gst_element_factory_make("filesrc", (std::string("file-source")+_count).c_str());
    GstElement* _demuxer  = gst_element_factory_make ("oggdemux",(std::string("ogg-demuxer")+_count).c_str());
    GstElement* _decoder  = gst_element_factory_make ("vorbisdec",(std::string("vorbis-decoder")+_count).c_str());
    GstElement* _conv     = gst_element_factory_make ("audioconvert",(std::string("converter")+_count).c_str());
    GstElement* _vol     = gst_element_factory_make ("volume",(std::string("volume-mixer")+_count).c_str());

    std::string track_name("test.ogg");
    std::string _sample_name(sample_name);
    if (_sample_name == track_name.c_str()){
        printf("set volume for this sample to track 1 volume\n");
        vol_track1 = _vol;
        g_object_set(G_OBJECT(_vol), "volume", volume_track1, NULL);
    }else{
        printf("set volume for this sample to track 2 volume\n");
        vol_track2 = _vol;
        g_object_set(G_OBJECT(_vol), "volume", volume_track2, NULL);
    }
    

    if (!_source || !_demuxer || !_decoder || !_conv){
        printf("One element couldn't be created.");
    }

    gst_bin_add_many(GST_BIN(pipeline), _source, _demuxer, _decoder, _conv, _vol, NULL);
    g_signal_connect (_demuxer, "pad-added", G_CALLBACK (on_pad_added), _decoder); 

    g_object_set(G_OBJECT(_source), "location", sample_name, NULL);
    printf("linking source and demux %d\n", gst_element_link(_source, _demuxer));
    printf("linking decoder and conv %d\n", gst_element_link(_decoder, _conv));
    printf("linking conv and volume   %d\n", gst_element_link(_conv, _vol));
    printf("linking volume and adder   %d\n", gst_element_link(_vol, adder));

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    count++;
}
    
void Player::set_volume(double _volume){
    volume = _volume;
    g_object_set(G_OBJECT(vol), "volume", _volume, NULL);
    return;
}

void Player::set_volume_track1(double _volume){
    volume_track1 = _volume;
    if (vol_track1 != NULL) g_object_set(G_OBJECT(vol_track1), "volume", _volume, NULL);
    return;
}

void Player::set_volume_track2(double _volume){
    volume_track2 = _volume;
    if (vol_track2 != NULL) g_object_set(G_OBJECT(vol_track2), "volume", _volume, NULL);
    return;
}

void Player::print_gst_version() {
    const gchar *nano_str;
    guint major, minor, micro, nano;
    gst_version (&major, &minor, &micro, &nano);
    if (nano == 1)
        nano_str = "(CVS)";
   else if (nano == 2)
        nano_str = "(Prerelease)";
    else
        nano_str = "";
    printf ("This program is linked against GStreamer %d.%d.%d %s\n",
          major, minor, micro, nano_str);
    return;
}

/*void Player::play(char* track_name){
    GstState state;
    gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
    if (state == GST_STATE_PLAYING){
        g_print("pausing...");
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
    }else{
        g_print("playing...");
        gst_element_set_state(pipeline, GST_STATE_READY);
        g_object_set(G_OBJECT(source), "location", track_name, NULL);
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }
}*/

void Player::listen(){
    midi_in->midi_listen(this);
    return;
}
