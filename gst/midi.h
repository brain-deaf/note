#ifndef midi_h
#define midi_h

#include <iostream>
#include <cstdlib>
#include <thread>
#include <stdexcept>
#include <gst/gst.h>
#include "RtMidi.h"

typedef void (*cb)(double deltatime, std::vector<unsigned char>* message, void* userdata);

class MidiInput{
    RtMidiIn rtmidi_in;
public:
    MidiInput(int port, cb userfunction, gpointer userdata);
    static void midi_listen( double deltatime, std::vector< unsigned char > *message, void *userData );
};

class NoPortsException : public std::runtime_error{
public:
    NoPortsException(std::string& what_arg):runtime_error(what_arg){}
    NoPortsException(const char * what_arg):runtime_error(what_arg){}
};

#endif //midi_h
