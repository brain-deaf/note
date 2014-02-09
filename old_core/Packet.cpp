#include "Packet.h"
#include <algorithm>
Packet::Packet(const size_type& size): packet_(size) {
}

Packet::Packet(const size_type& size, const Sample& sample): packet_(size,sample) {
}

Packet::size_type Packet::size() const {
    return packet_.size(); 
}

void Packet::reserve(size_type size) {
    packet_.reserve(size);
}

Sample& Packet::operator[](const size_type pos) {
    return packet_[pos];
}

const Sample& Packet::operator[](const size_type pos) const {
    return packet_[pos];
}

Sample& Packet::at(size_type pos) {
    return packet_.at(pos);
}
const Sample& Packet::at(size_type pos) const {
    return packet_.at(pos);
}
Packet::iterator Packet::begin() {
    return packet_.begin();
}

Packet::const_iterator Packet::begin() const {
    return packet_.begin();
}

Packet::iterator Packet::end() {
    return packet_.end();
}

Packet::const_iterator Packet::end() const {
    return packet_.end();
}

Packet::reverse_iterator Packet::rbegin() {
    return packet_.rbegin();
}

Packet::const_reverse_iterator Packet::rbegin() const {
    return packet_.rbegin();
}

Packet::reverse_iterator Packet::rend() {
    return packet_.rend();
}

Packet::const_reverse_iterator Packet::rend() const {
    return packet_.rend();
}

Sample* Packet::data() {
    return packet_.data();    
}

Packet Packet::operator+(const Packet& other) const {
    Packet ret(std::max(size(),other.size()));
    for(size_type i = 0; i < std::min(size(),other.size()); ++i) {
        ret[i]=packet_[i]+other[i];
    }
    return ret;
}

Packet Packet::operator+=(const Packet& other) {
    for(size_type i=0; i < std::min(size(),other.size()); ++i) {
        (*this)[i]+=other[i];
    }
    return *this;
}

