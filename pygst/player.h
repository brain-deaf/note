#ifndef player_h
#define player_h

extern "C" {
    #include <stdio.h>
    #include <gst/gst.h>
}
#include <string>
#include "midi.h"
#include "chan.h"
#include <unordered_map>


class Player{
public:
    GMainLoop* loop;
    GstBus* bus;
    int count;
    double volume;
    guint bus_watch_id;
    GstElement* pipeline;
    GstElement* source;
    GstElement* sink;
    GstElement* adder;
    GstElement* vol;

	std::unordered_map<std::string, Chan*> chans;

    MidiInput* midi_in;

    Player();
    ~Player();
    void play_sample();
    void set_volume(double _volume);
    void listen(void(*)(double, std::vector<unsigned char>* message, void* userdata), void* userdata);
	void chan(const char* name, const char* path);
	void add_chan(Chan* chan);
};

#endif //player_h
