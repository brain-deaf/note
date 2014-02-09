#include <boost/python.hpp>
#include "Offset.h"

using namespace boost::python;

void export_Offset() {
    class_<Offset>("Offset",init<Offset::underlying>())
        .def("value",&Offset::value)
        .def(self < self)
        .def(self <= self)
        .def(self > self)
        .def(self >= self)
        .def(self == self)
        .def(self - self)
        .def(self + self);
}
