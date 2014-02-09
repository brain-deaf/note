#ifndef PACKET_H
#define PACKET_H 
#include <vector>
#include <tuple>
#include "global.h"
class Packet {
    
    typedef std::vector<Sample> Packet_;
    Packet_ packet_;
public: 
    typedef Packet_::size_type size_type;
    typedef Packet_::iterator iterator;
    typedef Packet_::const_iterator const_iterator;
    typedef Packet_::reverse_iterator reverse_iterator;
    typedef Packet_::const_reverse_iterator const_reverse_iterator;

    typedef const Sample& (Packet::* const_sample_ref_sizetype_const)(size_type) const;
    typedef Sample& (Packet::* sample_ref_sizetype)(size_type);

    Packet() : packet_() {}
    Packet(const size_type& size);   
    Packet(const size_type& size, const Sample& sample);

    size_type size() const; 
    void reserve(size_type size);

    Sample& operator[](size_type pos);
    const Sample& operator[](size_type pos) const;

    Sample& at(size_type pos);
    const Sample& at(size_type pos) const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;

    Sample* data();

    Packet operator+(const Packet& other) const;
    Packet operator+=(const Packet& other);
};

#endif /* PACKET_H */
