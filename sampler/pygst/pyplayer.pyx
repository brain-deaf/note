from libcpp.vector cimport vector

cdef extern from "player.h":
    cdef cppclass Player:
        Player()
        void start_main()
        void play_sample(char* sample_name)
        void set_volume(double _volume)
        void set_volume_track1(double _volume)
        void set_volume_track2(double _volume)
        void listen(void(*f)(double, vector[unsigned char]*, void*), void* userdata) nogil

cdef void midi_init(Player* ob, void* function) nogil:
    with nogil:
        ob.listen(callback, function)

cdef void callback(double deltatime, vector[unsigned char]* message, void* userdata) with gil:
    (<object>userdata)(message[0])	

cdef class PyPlayer:
    cdef Player *thisptr
    def __cinit__(self):
        self.thisptr = new Player()
    def play_sample(self, sample_name):
        self.thisptr.play_sample(sample_name)
    def set_volume(self, _volume):
        self.thisptr.set_volume(_volume)
    def set_volume_track1(self, _volume):
        self.thisptr.set_volume_track1(_volume)
    def set_volume_track2(self, _volume):
        self.thisptr.set_volume_track2(_volume)
    def get_midi_in(self, f):
        midi_init(self.thisptr, <void*>f)
    def destruct(self):
        del self.thisptr
