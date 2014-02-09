#include <cmath>
#include <algorithm>
#include <utility>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/assert.hpp>
#include "Instrument.h"

Instrument::Instrument(): 
    Instrument(Offset(44100), 220.0,"wavesynth_log.txt") {
}

Instrument::Instrument(const std::string& dumpname):
    Instrument(Offset(44100), 220.0, dumpname) {
}

Instrument::Instrument(const Offset& sample_rate_init, const double& freq_reference_init, const std::string& dumpname): 
    sample_rate_(sample_rate_init),
    freq_reference_(freq_reference_init),
    cache_(),
    notes_(), 
    dump_(dumpname) {
}

Packet Instrument::get_samples(const Offset& start_offset, const Offset& end_offset) {
    std::vector<Note> notes_to_get;
    for(Note note : notes_) {
        if(
            (note.end().to_offset(Config::tempo, Config::sample_rate) > start_offset) &&
            (note.position().to_offset(Config::tempo, Config::sample_rate) < end_offset)) { 
            notes_to_get.push_back(note);
        }
    }

    Packet ret((end_offset-start_offset).value()); 
    for(Note note : notes_to_get) {
        render_note(ret,note,start_offset);
    }
    return ret;
}

void Instrument::add_note(const Note& note) {
    notes_.push_back(note);
    do_cache(note);
    std::sort(notes_.begin(),notes_.end());
}

//not the most efficient way to do this but I'm trying to achieve some DRY with the caching
//TODO: make this less shitty
void Instrument::add_notes(const Notes& notes) {
    for(Note note : notes) {
        add_note(note);    
    }
}

double Instrument::omega(const Note& n) const {
    return  2 * M_PI * frequency(n) / Config::sample_rate.value();
}

double Instrument::frequency(const Note& n) const {
    auto exponent = (Config::pc_count*n.octave()+n.pitch_class())/static_cast<double>(Config::pc_count);
    return Config::freq_reference * pow(2,exponent);
}

double Instrument::period(const Note& n) const {
    return 1.0/frequency(n);
}

double Instrument::rperiod(const Note& n) const {
    return 1.0/omega(n);
}
//TODO: ensure this rounds consistently, create an actual converter object
Offset Instrument::period_i(const Note& n) const {
    Offset ret(Config::sample_rate.value()*period(n)); 
    return ret;
}

Offset Instrument::rperiod_i(const Note& n) const {
    return Offset(Config::sample_rate.value()*rperiod(n));
}

Beat Instrument::stream_end() const {
    return notes_.back().end(); 
}

Offset Instrument::sample_rate() const {
    return sample_rate_;
}

double Instrument::freq_reference() const {
    return freq_reference_;
}

Sample Instrument::round(double t) const {
    using namespace boost::numeric;
    converter<Sample
        , double
        , conversion_traits<Sample, double>
        , def_overflow_handler
        , Trunc<typename conversion_traits<Sample, double>::source_type>
        > typeconverter;    
    return typeconverter(t); 
}

void Instrument::render_note(Packet& packet, const Note& note, const Offset& start_offset) {
    const Packet note_packet = cache_.at(note); 

    const Offset note_begin_output_index = note.position().to_offset(Config::tempo, Config::sample_rate) - start_offset;
    const Offset note_end_output_index = note_begin_output_index + note.length().to_offset(Config::tempo, Config::sample_rate);

    const Offset begin_output_index = 
        (note_begin_output_index < Offset(0)) ? Offset(0) : note_begin_output_index;
    const Offset end_output_index = 
        (note_end_output_index > Offset(packet.size())) ? Offset(packet.size()) : note_end_output_index;
    BOOST_ASSERT(begin_output_index < end_output_index);

    const Offset index_offset = note_begin_output_index;
    for(int i = begin_output_index.value(); i < end_output_index.value(); ++i) {
        packet.at(i) += note_packet.at(i-index_offset.value()); 
        dump_<<packet.at(i)<<" ";
    }
}

void Instrument::do_cache(const Note& note) {
    if (cache_.find(note) == cache_.end()) {
        cache_[note]=gen(note);
        cache_.at(note);
    }
}

