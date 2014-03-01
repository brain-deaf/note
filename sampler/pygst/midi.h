#ifndef midi_h
#define midi_h

#include "player.h"
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <thread>

typedef void (*cb)(double deltatime, std::vector<unsigned char>* message, void* userdata);

class MidiInput{
public:
	static void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData );
    static int midi_listen(cb user_function, void* userdata);
};

#endif //midi_h
