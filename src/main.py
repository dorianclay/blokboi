from pycolors import Colors
from lib.cgame import PyGame

def main():
    game_instance = PyGame()
    game_instance.newGame()
    print(game_instance.representation())

if __name__ == '__main__':
    main()