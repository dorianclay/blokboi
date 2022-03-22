cdef extern from "game.cpp":
    pass

cdef extern from "game.h":
    cdef cppclass Game:
        Game() except +
        void newGame()
        void resetGame()
        PlayerController *player_controller()
        string strprint()