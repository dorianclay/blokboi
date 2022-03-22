# distutils: language = c++

from Game cimport Game

def main():
    test_game = new Game()
    test_game.newGame()
    test_game.strprint()