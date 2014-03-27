#include "sampler.h"
#include "chan.h"

int main() {
    Sampler s;
    Chan * c1 = new Chan("bowl1","bowl1.wav");
    Chan * c2 = new Chan("bowl2","bowl2.wav");
    Chan * c3 = new Chan("donk","donk.ogg");
    
    s.add_chan(c1);
    s.add_chan(c2);
    s.add_chan(c3);

    s.play_all();
    s.go();
}
