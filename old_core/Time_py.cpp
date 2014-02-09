#include <boost/python.hpp>
#include "Time.h"
#include "Offset.h"
#include "Beat.h"

using namespace boost::python;

void export_Time() {
    class_<Time>("Time",init<Time::underlying>())
        .def("to_beat",&Time::to_beat)
        .def("to_offset",&Time::to_offset)
        .def("value",&Time::value);
}
