#ifndef CHAN_H
#define CHAN_H

#include <string>
#include <gst/gst.h>
class Chan {
    std::string name;
    std::string path;
    GstElement * src;
    GstElement * decode;
//    GstElement * vol;
    GstElement * conv;
    GstElement * resample;
    GstElement * bin;
    GstPad * out;
//    double volume;
    

public:
    static void on_pad_added(GstElement * element, GstPad * pad, gpointer data);
    Chan(const char * name, const char * path);
    GstPad * source_pad();
    void add_bin(GstElement * p);
    GstElement* get_bin();
    std::string& get_name() {return name;}
};

#endif /* CHAN_H */
