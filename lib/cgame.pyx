# distutils: language = c++
# distutils: include_dirs = include/loguru

from lib.game cimport Game

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PyGame:
    cdef Game c_game

    def __cinit__(self):
        self.c_game = Game()

    def newGame(self):
        self.c_game.newGame()
    
    def resetGame(self):
        self.c_game.resetGame()

    def move(self, direction):
        self.c_game.move(direction)

    def jump(self):
        self.c_game.jump()

    def pick_up(self):
        self.c_game.pick_up()

    def put_down(self):
        self.c_game.put_down()

    def representation(self):
        return self.c_game.representation()
    
    def objective(self):
        return self.c_game.objective()