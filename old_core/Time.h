#ifndef TIME_H
#define TIME_H 
#include <cstdint>
class Beat;
class Offset;
class Time{
    int64_t time_;
public:
    typedef int64_t underlying;

    Time(int64_t time_initializer);
    Beat to_beat(const Beat& tempo) const;
    Offset to_offset(const Offset& sample_rate) const;
    underlying value() const;
}; 
#endif /* TIME_H */
