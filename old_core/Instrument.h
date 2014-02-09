#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <unordered_map>
#include <vector>
#include <tuple>
#include <mutex>
#include <iterator>
#include <fstream>
#include <string>
#include "Offset.h"
#include "Packet.h"
#include "global.h"
#include "Note.h"

class Instrument {
    const Offset sample_rate_;
    const double freq_reference_;
    std::unordered_map<Note, Packet, Note_hash, Note_hash_comp> cache_;
    std::vector<Note> notes_;
    std::ofstream dump_;

public:
    typedef void (Instrument::* void_Note)(const Note&);
    typedef std::vector<Note> Notes;
    Packet get_samples(const Offset& start_offset, const Offset& end_offset); 
    void add_note(const Note& note);
    void add_notes(const Notes& notes);
    
    double frequency(const Note& note) const;
    double omega(const Note& note) const;
    double period(const Note& note) const; 
    double rperiod(const Note& note) const;

    Offset period_i(const Note& note) const;
    Offset rperiod_i(const Note& note) const;

    Beat stream_end() const;

    Offset sample_rate() const;
    double freq_reference() const;

protected:
    Instrument();
    Instrument(const std::string& dumpname);
    Instrument(const Offset& sample_rate_init);
    Instrument(const Offset& sample_rate_Init, const double& freq_reference_init, const std::string& dumpname);
    virtual ~Instrument(){}

    virtual Sample round(double t) const;
    virtual Packet gen(const Note& note)=0;

    void render_note(Packet& packet,const Note& note,const Offset& start_offset);
    void do_cache(const Note& note);

    //utility functions
    template<typename Iter>
    std::tuple<Iter,Iter> find_zero_crossing(std::tuple<Iter,Iter> iters) {
        const auto start = std::get<0>(iters);
        const auto end = std::get<1>(iters);

        auto is_negative = [] (Iter it) -> bool {return ((*it) < 0);};
        bool first_was_negative = is_negative(start);
        auto it = start;

        for(; it != end; ++it) {
            if((is_negative(it) != first_was_negative) || 
               ((*it) == 0)) {
                break;
            }
        } 
        return std::make_tuple(start,it);
    }

    template<typename Iter>
    std::tuple<Iter,Iter> silence(const std::tuple<Iter,Iter> iters) {
        const auto it = std::get<0>(iters);
        const auto end = std::get<1>(iters);
        std::fill(it,end,0);
        return iters;
    }

    template<typename Iter>
    std::tuple<Iter, Iter> fade(const std::tuple<Iter,Iter> iters) {
        const Iter start = std::get<0>(iters);
        const Iter end = std::get<1>(iters);
        const int count = end - start;
        const double mult_step = 1.0/count;
        for(int i = 0; start + i < end; ++i) {
            *(start + i) = round((*(start + i)).value()*(i*mult_step));
        }
        return iters;
    }
};

typedef std::shared_ptr<Instrument> InstrumentHandle;
#endif /* INSTRUMENT_H */
