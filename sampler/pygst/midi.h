#ifndef midi_h
#define midi_h

#include <iostream>
#include <cstdlib>
#include <thread>
#include "RtMidi.h"

typedef void (*cb)(double deltatime, std::vector<unsigned char>* message, void* userdata);

class MidiInput{
public:
	static void midi_listen( double deltatime, std::vector< unsigned char > *message, void *userData );
    static int midi_setup(cb user_function, void* userdata);
};

#endif //midi_h
