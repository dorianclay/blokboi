import argparse
import os
from pathlib import Path
from src.logger import Logger
from src.image_gen import ImageGen
from blokboi import Game

logger = Logger.log_setup('blokboi', detail='debug', suppress_datetime=False, console=True)

def test():
    logger.info("Running a simple test of Blokboi API.")
    game_instance = Game()
    logger.info("Got map:\n" + game_instance.__repr__())

    # logger.info(game_instance.scene_space())

    ImageGen.make_image_from_str(game_instance.__repr__(), Path('test.png'))


def generate_rand_scenes(num, outdir=Path('data')):
    logger.info('Beginning generating random scenes...')
    game_instance = Game()
    count = 0
    while count < num:
        game_instance.newGame()
        ImageGen.make_image_from_str(game_instance.__repr__(),outdir / f'scene_{count:05d}.png')
        count += 1
    logger.info(f'Finished making {count} scenes.')


def main(**kwargs):
    logger.info('Initializing blokboi.')
    if kwargs['test']:
        try:
            test()
        except:
            logger.exception('ERROR WHILE TESTING.')

    if kwargs['generate_images'] != None:
        try:
            num_images = int(kwargs['generate_images'][0])
            outdir = Path(kwargs['generate_images'][1])
            found = Path(outdir).glob('**/scene_*.png')
            if found != None:
                for path in found:
                    os.remove(path)
            generate_rand_scenes(num_images, outdir)
        except:
            logger.exception('Error while generating images.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Command line control for the Blokboi game API.')
    parser.add_argument('--test',
                action='store_true',
                help='run a simple test')
    parser.add_argument('--generate_images',
                nargs=2,
                help=f'generate images, < number, directory >')
    args = parser.parse_args()
    main(**vars(args))