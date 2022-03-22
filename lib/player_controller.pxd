cdef extern from "player_controller.cpp":
    pass

cdef extern from "player_controller.h":
    cdef cppclass PlayerController:
        PlayerController(Scene *scene, Player *player) except +
        void move(int direction)
        void jump()
        void pick_up()
        void put_down()