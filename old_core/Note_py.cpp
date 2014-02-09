#include "Note.h"
#include <boost/python.hpp>

using namespace boost::python;

void export_Note() {
    //these are to facilitate overload resolution
    const Note::int_void_const pc = &Note::pitch_class; 
    const Note::int_void_const oct = &Note::octave;
    const Note::beat_void_const pos = &Note::position;
    const Note::beat_void_const end_ptr= &Note::end;

    class_<Note>("Note", init<int,int,Sample,Beat,Beat>())
        .add_property("pitch_class",pc)
        .add_property("octave", oct)
        .add_property("intensity", &Note::intensity)
        .add_property("position",pos)
        .add_property("end",end_ptr)
        .add_property("length",&Note::length)
        .def(self < self)
        .def(self == self);
}
