#ifndef WAVESYNTH_H
#define WAVESYNTH_H 
#include <functional>
#include "global.h"
#include "Instrument.h"

class WaveSynth : public Instrument {

public:
    class Wave {
    public:
        typedef std::function<double(double)> Functor;
        Wave();
        Wave(const Functor& functor_initializer);

        double operator()(const double angle) const;
        
    private:
        const Functor functor_;
    };

    class Waves {
        typedef std::vector<Wave> Waves_;
        const Waves_ waves_;
    public:
        typedef Waves_::size_type size_type;
        explicit Waves(const size_type size);
        Waves(const size_type size,const Wave& wave_initializer);
        Waves(std::initializer_list<Wave> l);

        double operator()(const double angle) const;
    };

WaveSynth(const Offset& sample_rate_init,const double& freq_reference_init, const std::string& dumpname);
    WaveSynth(const WaveSynth& this_initializer)=delete;
    WaveSynth(WaveSynth&& this_initializer)=delete;

    virtual ~WaveSynth() {}

private: 
    Waves waves_;
    virtual Packet gen(const Note& note);
};

#endif /* WAVESYNTH_H */
