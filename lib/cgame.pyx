# distutils: language = c++
# distutils: include_dirs = include/loguru

from lib.game cimport Game

def main():
    test_game = new Game()
    test_game.newGame()
    test_game.representation()