from libcpp.vector cimport vector

cdef extern from "player.h":
    cdef cppclass Player:
        Player()
        void _d_Player()
        void start_main()
        void print_gst_version()
        void play_sample(char* sample_name)
        void set_volume(double _volume)
        void set_volume_track1(double _volume)
        void set_volume_track2(double _volume)
        void listen(void(*f)(double, vector[unsigned char]*, void*), void* userdata) nogil
        void listen2(void* userdata) nogil
        void listen3() nogil

cdef extern from "midi.h":
    cdef cppclass MidiInput:
        void mycallback(double deltatime, vector[unsigned char]* message, void* userdata) with gil
        int midi_listen(void(*)(double, vector[unsigned char]*, void*), void* userdata) nogil
        int midi_listen2(void* userdata) nogil

cdef void midi_init(Player* ob, void* function) nogil:
    with nogil:
        ob.listen(callback, function)

cdef void midi_init2(Player* ob) nogil:
    with nogil:
        ob.listen2(ob)

cdef void midi_init3(Player* ob) nogil:
    with nogil:
        ob.listen3()

cdef void callback(double deltatime, vector[unsigned char]* message, void* userdata) with gil:
        #(<object>userdata)()	
    print("hello, world!")

cdef class MidiListen:
    cdef MidiInput* thisptr
    def __cinit__(self):
        self.thisptr = new MidiInput()

cdef class PyPlayer:
    cdef Player *thisptr
    def __cinit__(self):
        self.thisptr = new Player()
    def run(self):
        self.thisptr.start_main()
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
    def get_midi_in(self, f):
        midi_init(self.thisptr, <void*>f)
    def get_midi_in2(self):
        midi_init2(self.thisptr)
    def get_midi_in3(self):
        midi_init3(self.thisptr)
