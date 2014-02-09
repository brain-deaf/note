#include <boost/python.hpp>
#include "Sample.h"

using namespace boost::python;

void export_Sample() {
    class_<Sample>("Sample",init<Sample::underlying>())
        .def("value",&Sample::value)
        .def("max_intensity",&Sample::max_intensity)
        .def(self == self)
        .def(self < self)
        .def(self > self)
        .def(self + self)
        .def(self += self)
        .def(self / Sample::underlying());
}
