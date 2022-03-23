# distutils: language = c++

from lib.cgame cimport Game

def main():
    test_game = new Game()
    test_game.newGame()
    test_game.representation()