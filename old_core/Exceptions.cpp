#include "Exceptions.h"

BadFlowException::BadFlowException(const char * cstr):
    message_(cstr) { 
}

BadFlowException::BadFlowException(const std::string& str):
    message_(str) {
}

const char * BadFlowException::what() {
    return message_.c_str();
}

