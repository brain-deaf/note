#include <boost/python.hpp>
#include "WaveSynth.h"

using namespace boost::python;
void export_WaveSynth() {
    class_<WaveSynth,
        std::shared_ptr<WaveSynth>,
        bases<Instrument>,
        boost::noncopyable> (
            "WaveSynth",
            "creates a signal based on a waveform",
            init<Offset,double,std::string>());
    implicitly_convertible<std::shared_ptr<WaveSynth>,InstrumentHandle>();
}

