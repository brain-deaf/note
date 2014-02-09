#include <boost/python.hpp>
#include "Instrument.h"

using namespace boost::python;

void export_Instrument() {
    const Instrument::void_Note add_note = &Instrument::add_note;
    class_<Instrument,InstrumentHandle,boost::noncopyable>("Instrument", "Abstract base class",no_init)
        .def("add_note",add_note)
        .def("add_notes",&Instrument::add_notes);
}
