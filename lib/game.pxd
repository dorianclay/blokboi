cdef extern from "game.cpp":
    pass

from libcpp.string cimport string

cdef extern from "game.h":
    """
    #define CYTHON_COMP
    """
    cdef cppclass Game:
        Game() except +
        void newGame() except +
        void resetGame() except +
        void move(int direction) except +
        void jump() except +
        void pick_up() except +
        void put_down() except +
        string representation() except +
        string objective() except +