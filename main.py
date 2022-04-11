import argparse
import os
import io
from pathlib import Path
from contextlib import redirect_stderr
import numpy as np


from src.logger import Logger
from src.image_gen import ImageGen
from src.gui import *
from run_tests import LocalTestRunner
from blokboi import Game


def test(logger):
    logger.info("Running local tests...")
    with redirect_stderr(io.StringIO()) as f:
        results = LocalTestRunner.run_tests()
    logger.info(f"Test results:\n{f.getvalue()}")
    if results.wasSuccessful():
        logger.info("Python tests ran successfully.")
    else:
        logger.exception("Python tests failed.")

    logger.info("Running a simple test of Blokboi API.")
    game_instance = Game()
    logger.info("Got map:\n" + str(game_instance))

    arr = np.array(game_instance.array())

    strfull = ""
    for row in range(game_instance.width()):
        strrow = ""
        for col in range(game_instance.height()):
            strrow += f"[{(arr[row, col, 0])},{arr[row, col, 1]}] "
        strfull += strrow + "\n"
    logger.info("Game array representation:\n" + strfull)

    ImageGen.make_image_from_str(game_instance.__repr__(), Path("imagetest.png"))


def generate_rand_scenes(logger, num, outdir=Path("data")):
    logger.info("Beginning generating random scenes...")
    game_instance = Game()
    count = 0
    while count < num:
        game_instance.newGame()
        ImageGen.make_image_from_str(
            game_instance.__repr__(), outdir / f"scene_{count:05d}.png"
        )
        count += 1
    logger.info(f"Finished making {count} scenes.")


def main(**kwargs):
    logger = Logger.log_setup(
        "blokboi", detail="debug", suppress_datetime=False, console=True
    )
    logger.info("Initializing blokboi.")
    if kwargs["test"]:
        try:
            test(logger)
        except:
            logger.exception("ERROR WHILE TESTING.")

    if kwargs["generate_images"] != None:
        try:
            num_images = int(kwargs["generate_images"][0])
            outdir = Path(kwargs["generate_images"][1])
            found = Path(outdir).glob("**/scene_*.png")
            if found != None:
                for path in found:
                    os.remove(path)
            generate_rand_scenes(logger, num_images, outdir)
        except:
            logger.exception("Error while generating images.")

    if kwargs["gui"]:
        logger.debug("Running game app...")
        size = (15, 20)
        scale = 2
        if kwargs["load"]:
            scene = np.load("data/demo_scene.npy")
            app = App(size[1], size[0], scale=scale, game_instance=Game(scene))
        else:
            app = App(size[1], size[0], scale=scale)
        app.mainloop()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Command line control for the Blokboi game API."
    )
    runtype_group = parser.add_mutually_exclusive_group()
    runtype_group.add_argument("--test", action="store_true", help="run a simple test.")
    runtype_group.add_argument(
        "--generate_images", nargs=2, help="generate images, < number, directory >"
    )
    runtype_group.add_argument(
        "-G", "--gui", action="store_true", help="Run the GUI app."
    )
    parser.add_argument(
        "-l", "--load", action="store_true", help="load the demo scene."
    )
    parser.add_argument(
        "-d",
        "--detail",
        default="info",
        choices=["debug", "info", "warning", "errror", "critical"],
        help="level of detail to log (default: info).",
    )
    args = parser.parse_args()
    main(**vars(args))
