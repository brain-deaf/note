from libcpp.vector cimport vector

cdef extern from "player.h":
    cdef cppclass Player:
        Player()
        void start_main()
        void play_sample()
        void chan(const char* name, const char* path)
        void set_volume(double _volume)
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
    def play_sample(self):
        self.thisptr.play_sample()
    def set_volume(self, _volume):
        self.thisptr.set_volume(_volume)
    def get_midi_in(self, f):
        midi_init(self.thisptr, <void*>f)
    def add_chan(self, name, path):
        self.thisptr.chan(name, path)
    def destruct(self):
        del self.thisptr
