#include <boost/python.hpp>
#include "Packet.h"

using namespace boost::python;

void export_Packet() {
    class_<Packet>("Packet", init<Packet::size_type>())
        .def(init<Packet::size_type, Sample>());
}
