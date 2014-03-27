from libcpp.vector cimport vector

cdef extern from "sampler.h":
    cdef cppclass Sampler:
        Sampler()
        void set_volume(double _volume)
        void add_chan(Chan * chan)
        void go()
        void quit()

cdef extern from "chan.h":
    cdef cppclass Chan:
        Chan()
#        void listen(void(*f)(double, vector[unsigned char]*, void*), void* userdata) nogil
#
#cdef void midi_init(Chan* ob, void* function) nogil:
#    with nogil:
#        ob.listen(callback, function)
#
#cdef void callback(double deltatime, vector[unsigned char]* message, void* userdata) with gil:
#    (<object>userdata)(message[0])	

cdef class PySampler:
    cdef Sampler *thisptr
    def __cinit__(self):
        self.thisptr = new Sampler()
    def set_volume(self, _volume):
        self.thisptr.set_volume(_volume)
    def destruct(self):
        del self.thisptr
