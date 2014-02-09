#ifndef BEAT_H
#define BEAT_H 
#include <boost/rational.hpp>
class Time;
class Offset;
class Beat {
    boost::rational<int64_t> beat_;
public:
    typedef boost::rational<int64_t> underlying_rational;
    typedef int64_t underlying;
    Beat(underlying_rational beat_init):
        beat_(beat_init) {}
    Beat(underlying num, underlying denom=1):
        beat_(num,denom) {}
    Time to_time(Beat tempo) const;
    Offset to_offset(Beat tempo, Offset sample_rate) const;

    bool operator==(const Beat& other) const;
    bool operator<(const Beat& other) const;
    bool operator>(const Beat& other) const;

    Beat operator/(const Beat& other) const;
    Beat operator-(const Beat& other) const;
    Beat operator+(const Beat& other) const;

    Beat operator*(const underlying& other) const;
    Beat operator/(const underlying& other) const;

    double as_double() const;

    underlying num() const;
    underlying denom() const;
};

Beat operator*(const Beat::underlying& lhs, const Beat& rhs);
#endif /* BEAT_H */
