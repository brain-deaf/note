#ifndef player_h
#define player_h

extern "C" {
    #include <stdio.h>
    #include <gst/gst.h>
}
#include <string>
#include "midi.h"


class Player{
public:
    GMainLoop* loop;
    GstBus* bus;
    int count;
    double volume;
    double volume_track1;
    double volume_track2;
    guint bus_watch_id;
    GstElement* pipeline;
    GstElement* source;
    GstElement* decoder;
    GstElement* demuxer;
    GstElement* conv;
    GstElement* sink;
    GstElement* adder;
    GstElement* vol;
    GstElement* vol_track1;
    GstElement* vol_track2;

    MidiInput* midi_in;

    Player();
    void _d_Player();
    void start_main();
    void print_gst_version();
    void play_sample(char* sample_name);
    void set_volume(double _volume);
    void set_volume_track1(double _volume);
    void set_volume_track2(double _volume);
    void listen(void(*)(double, std::vector<unsigned char>* message, void* userdata), void* userdata);
};

#endif //player_h
