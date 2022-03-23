cdef extern from "game.cpp":
    pass

from libcpp.string cimport string

cdef extern from "game.h":
    """
    #define CYTHON_COMP
    """
    cdef cppclass Game:
        Game() except +
        void newGame()
        void resetGame()
        void move(int direction)
        void jump()
        void pick_up()
        void put_down()
        string representation()
        string objective()