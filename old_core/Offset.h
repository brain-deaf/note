#ifndef OFFSET_H
#define OFFSET_H 
#include <cstdint>
class Time;
class Beat;
class Offset {
    int64_t offset_;
public:
    typedef int64_t underlying;
    constexpr Offset(): offset_() {}
    constexpr explicit Offset(underlying offset_init): offset_(offset_init) {}
    Time to_time(const Offset& sample_rate) const;
    Beat to_beat(const Offset& sample_rate, const Beat& tempo) const;
    bool operator<(const Offset& other) const;
    bool operator<=(const Offset& other) const;
    bool operator>(const Offset& other) const;
    bool operator>=(const Offset& other) const;
    bool operator==(const Offset& other) const;
    Offset operator-(const Offset& other) const;
    Offset operator+(const Offset& other) const;
    Offset& operator+=(const Offset& other);
    underlying value() const;
};

#endif /* OFFSET_H */
