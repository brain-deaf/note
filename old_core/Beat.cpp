#include <iostream>
#include "Beat.h"
#include "Time.h"
#include "Offset.h"
#include "global.h"

//position(beats) / tempo(beats/sec) * 1e9 ns/second = time
//position(beats)   |     (sec)     | 1e9 (ns)
//                  | tempo(beats)  | 1 (sec)
Time Beat::to_time(Beat tempo) const {
    return Time(boost::rational_cast<double>(beat_/tempo.beat_)*Config::count_ns_in_sec);
}

//position (beats) * (1/tempo) (sec/beat) sample rate (sample / second)
//position (beats)  |       (sec)   | sample rate (samples)
//                  | tempo (beats) |             (second)
Offset Beat::to_offset(Beat tempo, Offset sample_rate) const {
    auto init = boost::rational_cast<double>(beat_/tempo.beat_);
    Offset ret(init*sample_rate.value());
    return ret;
}

bool Beat::operator==(const Beat& other) const {
    return beat_ == other.beat_;
}

bool Beat::operator<(const Beat& other) const {
    return beat_ < other.beat_;
}

bool Beat::operator>(const Beat& other) const {
    return beat_ > other.beat_;
}

Beat Beat::operator/(const Beat& other) const {
    return Beat(beat_/other.beat_);
}

Beat Beat::operator-(const Beat& other) const {
    return Beat(beat_-other.beat_);
}

Beat Beat::operator+(const Beat& other) const {
    return Beat(beat_+other.beat_);
}

Beat Beat::operator*(const underlying& other) const {
    return Beat(beat_*other);
}

Beat Beat::operator/(const underlying& other) const {
    return Beat(beat_/other);
}

double Beat::as_double() const {
    return boost::rational_cast<double>(beat_);
}

Beat operator*(const Beat::underlying& lhs, const Beat& rhs) {
    return rhs*lhs;
}

Beat::underlying Beat::num() const {
    return beat_.numerator();
}

Beat::underlying Beat::denom() const {
    return beat_.denominator();
}

