#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include <boost/functional/hash.hpp>
#include <functional>
#include <tuple>
#include "global.h"
class Note {
public:
    Note(int pc_init, int octave_init, Sample intensity_init, Beat pos_init, Beat len_init);
    
    typedef int (Note::* int_void_const)() const;
    typedef Beat (Note::* beat_void_const)() const;
    

    int pitch_class() const;
    int pitch_class(int ref) const;

    int octave() const;
    int octave(int ref) const;

    Beat position() const;
    Beat position(Beat ref) const;
    Beat end() const;
    Beat end(Beat red) const;

    //note length is consistent in all reference frames, so no overload
    Beat length() const;
    //no such thing as frame of reference for "intensity" -- the scale is by definition absolute, so no need to overload this one either
    Sample intensity() const;

    bool operator<(const Note&) const;
    bool operator==(const Note&) const;

private: 
    int pitch_class_;
    int octave_;

    Sample intensity_;

    Beat pos_;
    Beat length_;
}; 

struct Note_hash_comp{
    bool operator()(const Note& lhs, const Note& rhs) const noexcept{
        return 
            (lhs.pitch_class() == rhs.pitch_class()) &&
            (lhs.octave() == rhs.octave()) &&
            (lhs.intensity() == rhs.intensity()) &&
            (lhs.length() == rhs.length());
    }
};

struct Note_hash {
    size_t operator()(const Note& note) const {
            return boost::hash_value(std::make_tuple(
                note.pitch_class(),note.octave(),note.intensity().value(),note.length().as_double()));
    }
};
#endif /* NOTE_H */
