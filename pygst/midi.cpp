#include <vector>
#include "midi.h"
#include "player.h"
void MidiInput::midi_listen( double deltatime, std::vector< unsigned char > *message, void *userData ){
  Player* my_player = static_cast<Player*>(userData);

  unsigned int nBytes = message->size();
  for (unsigned int i=0; i<nBytes; i++){
     unsigned char value = message->at(i);
     std::cout << "Byte " << i << " = " << value << ", ";
     /*if (i == 0 && value == 144){
         my_player->play_sample("test.ogg");
     }*/
         
  }
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}

int MidiInput::midi_setup(cb user_function, void* userdata){
  RtMidiIn *midiin = new RtMidiIn();
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 1 );
  midiin->setCallback( user_function, userdata );
  midiin->ignoreTypes( false, false, false );

  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);

 cleanup:
  delete midiin;

  return 0;
}
