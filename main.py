# import argparse

from src.logger import Logger
from src.pycolors import Colors
from blokboi import Game

def main():
    logger = Logger.log_setup('blokboi', detail='debug', suppress_datetime=False, console=True)

    logger.info('Hello World! Starting blokboi slave')

    game_instance = Game()
    logger.info("Got map:\n" + game_instance.__repr__())

if __name__ == '__main__':
    main()