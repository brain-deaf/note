#include <string>
#include <boost/python.hpp>
#include "Player.h"

using namespace boost::python;
void export_Player() {
    scope Player_scope = class_<Player,PlayerHandle, boost::noncopyable>("Player", init<Player::BackendType, Offset, double, std::string>())
        .def("play",&Player::play)
        .def("add_instrument",&Player::add_instrument)
        .def("wait_until_ready",&Player::wait_until_ready)
        .def("sample_rate",&Player::sample_rate)
        .def("freq_reference",&Player::freq_reference);
    enum_<Player::BackendType>("BackendType")
        .value("local", Player::BackendType::local)
        .value("vorbis", Player::BackendType::vorbis);    
}
