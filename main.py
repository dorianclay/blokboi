import argparse

from src.logger import Logger
from src.pycolors import Colors

def main():
    logger = Logger.log_setup('blokboi', detail='debug', suppress_datetime=False, console=False)

    print(Colors.makeCyan('Hello World!'))

if __name__ == '__main__':
    main();