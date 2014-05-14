#include <iostream>
#include "chan.h"

void Chan::on_pad_added (GstElement *element, GstPad *pad, gpointer data){
    Chan * ch = static_cast<Chan*>(data);
    GstElement * converter = ch->converter();
	std::cout<<ch<<"\n"<<ch->conve<<"\n"<<ch->resample<<"\n"<<ch->src<<"\n"<<ch->decode<<"\n";
    GstPad *sinkpad = gst_element_get_static_pad (converter, "sink");
    if (sinkpad == nullptr) std::cerr<<"error retrieving sinkpad2"<<std::endl;
    gst_pad_link(pad,sinkpad);
    gst_object_unref (sinkpad);
    GstObject * p = gst_element_get_parent(element);
    std::string name = gst_element_get_name(p);
    GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(p),GST_DEBUG_GRAPH_SHOW_ALL,(("callback"+name).c_str()));
}

Chan::Chan(const char * n, const char * p):
    name(n),
    path(p){
    src = gst_element_factory_make("filesrc",(name+"filesrc").c_str());
    decode = gst_element_factory_make("decodebin",(name+"decode").c_str());
    //vol=gst_element_factory_make("volume",(name+"volume").c_str());

    conve=gst_element_factory_make("audioconvert",(name+"conv").c_str());
    //conver=gst_element_factory_make("audioconvert",(name+"conv2").c_str());
    resample=gst_element_factory_make("audioresample",(name+"resample").c_str());
    bin=gst_bin_new((name+"bin").c_str());
    g_object_set(G_OBJECT(src),"location",p,nullptr);
    //g_object_set(G_OBJECT(vol), "volume", volume, nullptr);

    if (!conve) printf("converter could not be created\n");
    if (!decode) printf("decoder could not be created\n");
    if (!src) printf("src could not be created\n");
    if (!resample) printf("resample could not be created\n");

    gst_bin_add_many(GST_BIN(bin),src,decode,conve,nullptr);
    gst_element_link_many(src,decode, conve, NULL);
    //g_signal_connect (decode, "pad-added", G_CALLBACK (on_pad_added), this); 
    //gst_element_link(conve,resample);

    //out = gst_ghost_pad_new(nullptr,gst_element_get_static_pad(resample,"src"));
    //gst_element_add_pad(bin,out);
	std::cout<<conve<<"\n";
}

void Chan::add_bin(GstElement * p) {
    gst_bin_add(GST_BIN(p),bin);
}

GstPad * Chan::source_pad() {
    return out;
}

GstElement * Chan::converter() {
    return conve;
}

GstElement * Chan::get_bin() {
    return bin;
}

