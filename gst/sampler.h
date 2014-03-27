#ifndef sampler_h 
#define sampler_h

#include <stdio.h>
#include <gst/gst.h>
#include <unordered_map>
#include "chan.h"

class Sampler{
    std::unordered_map<std::string,Chan*> chans;
    GMainLoop * loop;
    GstElement * pipeline;
    GstElement * vol;
    GstElement * adder;
    GstElement * conv;
    GstElement * resample;
    GstElement * sink;
    double volume; //master vol
    guint bus_watch_id;
    static gboolean bus_call(GstBus* bus, GstMessage*msg, gpointer data);

public:
    Sampler();
    ~Sampler();
    void set_volume(double v);
    void add_chan(Chan *);
    void go() { g_main_loop_run(loop);}
    void play_all() {
        GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline),GST_DEBUG_GRAPH_SHOW_ALL,"test"); 
        gst_element_set_state(pipeline,GST_STATE_PLAYING);}
    void ready() { gst_element_set_state(pipeline, GST_STATE_READY);}
    void quit() { g_main_loop_quit(loop);}
};

#endif // sampler_h
