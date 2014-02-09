#include <boost/python.hpp>
#include "Exceptions.h"
using namespace boost::python;
void export_BadFlowException() {
    class_<BadFlowException>("BadFlowException",init<std::string>());
}
