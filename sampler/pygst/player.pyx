from libcpp.vector cimport vector

cdef extern from "player.h":
    cdef cppclass Player:
        Player()
        void _d_Player()
        void start_main()
        void print_gst_version()
        #void play(char* track_name)
        void play_sample(char* sample_name)
        void set_volume(double _volume)
        void set_volume_track1(double _volume)
        void set_volume_track2(double _volume)
        void listen() nogil

cdef extern from "midi.h":
    cdef cppclass MidiInput:
        void mycallback(double deltatime, vector[unsigned char]* message, void* userdata) with gil
        int midi_listen(void* player) nogil

cdef void midi_init(Player* ob):
    with nogil:
        ob.listen()

cdef class MidiListen:
    cdef MidiInput* thisptr
    def __cinit__(self):
        self.thisptr = new MidiInput()

cdef class PyPlayer:
    cdef Player *thisptr
    def __cinit__(self):
        self.thisptr = new Player()
    '''def __dealloc__(self):
        if self.thisptr is not NULL:
            self.thisptr._d_Player()'''
    def run(self):
        self.thisptr.start_main()
    #def play(self, track_name):
        #self.thisptr.play(track_name)
    def play_sample(self, sample_name):
        self.thisptr.play_sample(sample_name)
    def print_version(self):
        self.thisptr.print_gst_version()
    def destruct(self):
        self.thisptr._d_Player()
    def set_volume(self, _volume):
        self.thisptr.set_volume(_volume)
    def set_volume_track1(self, _volume):
        self.thisptr.set_volume_track1(_volume)
    def set_volume_track2(self, _volume):
        self.thisptr.set_volume_track2(_volume)
    def get_midi_in(self):
        midi_init(self.thisptr)
