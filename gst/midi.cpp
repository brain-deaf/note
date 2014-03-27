#include <vector>
#include "midi.h"
#include "sampler.h"



MidiInput::MidiInput(int port, cb user_function, gpointer userdata):
    rtmidi_in() {
  unsigned int nPorts = rtmidi_in.getPortCount();
  if ( nPorts == 0 ) {
    throw NoPortsException("No ports available");
  }
  rtmidi_in.openPort(0);
  rtmidi_in.setCallback( user_function, userdata );
  rtmidi_in.ignoreTypes( false, false, false );
}
void MidiInput::midi_listen( double deltatime, std::vector< unsigned char > *message, void *userData ){

  unsigned int nBytes = message->size();
  for (unsigned int i=0; i<nBytes; i++){
     unsigned char value = message->at(i);
     std::cout << "Byte " << i << " = " << value << std::endl;
     std::cout << "TODO: map midi data events to chans" << std::endl;
         
  }
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}

