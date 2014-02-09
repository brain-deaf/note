#ifndef CONFIG_H
#define CONFIG_H 
#include "Beat.h"
#include "Offset.h"
#include "Sample.h"

namespace Config {
    /// player's current sample rate
    constexpr Offset sample_rate(44100);
    const Beat tempo(2); //beats per second
    constexpr int count_ns_in_sec = 1000000000;
    /// reference frequency of the whole system, that is, frequency of pitch_class = 0, octave = 0
    constexpr double freq_reference = 220;
    /// amount of bytes in a sample 
    constexpr int word_size=sizeof(Sample::underlying);
    /// amount of channels in our audio stream
    constexpr int channels = 1;
    /// how many pitch classes are there in an octave?
    constexpr int pc_count = 12;
}

#endif /* CONFIG_H */
