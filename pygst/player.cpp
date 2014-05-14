#include "player.h"
#include "midi.h"


static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data){
    Player* my_player = (Player*) data;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:{
             g_print ("End of stream\n");
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

Player::Player(){
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);

    gpointer my_player;
    my_player = this;

    count = 0;
    pipeline = gst_pipeline_new("audio-player");
    sink     = gst_element_factory_make("autoaudiosink", "sink");
    adder    = gst_element_factory_make("adder", "mixer");
    //vol      = gst_element_factory_make("volume", "volume-mixer");

    //g_object_set(G_OBJECT(vol), "volume", volume, NULL);


    if (!sink) printf("sink could not be created\n");
    if (!adder) printf("adder could not be created\n");
    if (!pipeline) printf("pipeline could not be created\n");
    //if (!vol) printf("volume-mixer could not be created\n");

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, my_player);
    gst_object_unref (bus);

    gst_bin_add_many(GST_BIN(pipeline), adder, /*vol,*/ sink, NULL);
    //printf("link adder and volume: %d\n", gst_element_link(adder, vol));
    printf("link adder and sink: %d\n" , gst_element_link(adder, sink));

    midi_in = new MidiInput();
}

Player::~Player(){
    g_print("destructor\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    if (pipeline != NULL){
        gst_object_unref(GST_OBJECT(pipeline));
    }
    g_source_remove(bus_watch_id);
    g_main_loop_quit (loop);
    g_main_loop_unref(loop);
}

void Player::add_chan(Chan* chan){
	//chans[chan->get_name()] = chan;
	chan->add_bin(pipeline);

	//GstPad * sinkpad = gst_element_get_request_pad(adder,"sink_%u");
    gst_element_link(chan->converter(), adder);
    /*if(r) {
        std::cerr<< "error code: " << r << std::endl;
    }
    gst_object_unref(sinkpad);*/
}

void Player::chan(const char* name, const char* path){
	Chan x(name, path);
	this->add_chan(&x);
}

void Player::play_sample(){
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}
    
void Player::set_volume(double _volume){
    volume = _volume;
    g_object_set(G_OBJECT(vol), "volume", _volume, NULL);
    return;
}

void Player::listen(cb user_function, void* userdata){
    //midi_in->midi_listen(user_function, userdata);
  RtMidiIn *midiin = new RtMidiIn();
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 1 );
  midiin->setCallback( user_function, userdata );
  //midiin->setCallback( &mycallback, userdata );
  midiin->ignoreTypes( false, false, false );

  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);

 cleanup:
  delete midiin;

  return ;
    return;
}
