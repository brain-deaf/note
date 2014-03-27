#include "chan.h"

Chan::Chan(const char * n, const char * p):
    name(n),
    path(p),
    src(gst_element_factory_make("filesrc",n)),
    conv(gst_element_factory_make("audioconvert",nullptr)),
    vol(gst_element_factory_make("volume",nullptr)),
    bin(gst_bin_new(nullptr)),
    volume(1.0) {
    g_object_set(G_OBJECT(src),"location",p,nullptr);
    g_object_set(G_OBJECT(vol), "volume", volume, nullptr);
    gst_bin_add_many(GST_BIN(bin),src,vol,conv,nullptr);
    gst_element_link_many(src,vol,conv,nullptr);
    out = gst_ghost_pad_new(nullptr,gst_element_get_static_pad(conv,"src"));
    gst_element_add_pad(bin,out);
}

GstPad * Chan::source_pad() {
    return out;
}

void Chan::add_bin(GstElement * p) {
    gst_bin_add(GST_BIN(p),bin);
}

