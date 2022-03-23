# import argparse

from src.logger import Logger
from src.pycolors import Colors
from cgame import PyGame

def main():
    logger = Logger.log_setup('blokboi', detail='debug', suppress_datetime=False, console=False)

    print(Colors.makeCyan('Hello World! Starting blokboi slave'))

    game_instance = PyGame()
    game_instance.newGame()
    logger.info(game_instance.representation())

if __name__ == '__main__':
    main()