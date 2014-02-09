#include <boost/python.hpp>
#include "Beat.h"
using namespace boost::python;
void export_Beat() {
    class_<Beat>("Beat",init<Beat::underlying,Beat::underlying>())
        .def(init<int64_t,int64_t>())
        .def(self * int())
        .def(int() * self)
        .def(self == self)
        .def(self < self)
        .def(self > self)
        .def(self / self)
        .def(self - self)
        .def(self + self)
        .def(self * Beat::underlying())
        .def(Beat::underlying() * self)
        .def(self / Beat::underlying())
        .def("as_double", &Beat::as_double)
        .add_property("num",&Beat::num)
        .add_property("denom",&Beat::denom);
}
