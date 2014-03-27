#ifndef CHAN_H
#define CHAN_H

#include <string>
#include <gst/gst.h>
class Chan {
    std::string name;
    std::string path;
    GstElement * src;
    GstElement * conv;
    GstElement * vol;
    GstElement * bin;
    GstPad * out;
    double volume;

public:
    Chan(const char * name, const char * path);
    GstPad * source_pad();
    void add_bin(GstElement * p);
    std::string& get_name() {return name;}
};
#endif /* CHAN_H */
