cdef extern from "game.cpp":
    pass

from libcpp.string cimport string
from cplayer_controller import PlayerController

cdef extern from "game.h":
    cdef cppclass Game:
        Game() except +
        void newGame()
        void resetGame()
        void move(int direction)
        void jump()
        void pick_up()
        void put_down()
        string representation()