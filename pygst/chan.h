#ifndef CHAN_H
#define CHAN_H

#include <string>
#include <gst/gst.h>
class Chan {
//    GstElement * vol;
//    double volume;
    

public:
    static void on_pad_added(GstElement * element, GstPad * pad, gpointer data);
    Chan(const char * name, const char * path);
    //GstPad * source_pad();
    void add_bin(GstElement * p);
    GstElement* get_bin();
    std::string& get_name() {return name;}
	GstPad * source_pad();
	GstElement * converter();

    std::string name;
    std::string path;
    GstElement * src;
    GstElement * decode;
    GstElement * conve;
    //GstElement * conver;
    GstElement * resample;
    GstElement * bin;
	GstPad * out;

};

#endif /* CHAN_H */
