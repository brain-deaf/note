#include "Time.h"
#include "Offset.h"
#include "Beat.h"
#include "global.h"

Time::Time(int64_t time_initializer):
    time_(time_initializer) {
}

//time(ns) * (1 sec / 1e9 ns) * tempo (beats/sec) = beats
//time(ns)  | 1   (sec) | tempo (beats)
//          | 1e9 (ns)  |       (sec)
Beat Time::to_beat(const Beat& tempo) const {
    return Beat(time_*tempo/Config::count_ns_in_sec);
}

//time(ns) * (1 sec / 1e9 ns) * sample rate (sample/sec) = offset (sample)
//time(ns)  | 1   (sec) | sample rate (sample)  
//          | 1e9 (ns)  |             (sec)     
Offset Time::to_offset(const Offset& sample_rate) const {
    return Offset(time_*sample_rate.value()/Config::count_ns_in_sec);
}

Time::underlying Time::value() const {
    return time_;
}

