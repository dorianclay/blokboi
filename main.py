import argparse
import os
import io
import json
from pathlib import Path
from contextlib import redirect_stderr
from sqlite3 import paramstyle
import numpy as np


from src.logger import Logger
from src.image_gen import ImageGen
from src.gui import *
from src.map_loader import MapLoader
from run_tests import LocalTestRunner
from blokboi import Game


def test(**kwargs):
    logger = Logger.log_setup(
        "blokboiTest", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
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


def generate_rand_scenes(**kwargs):  # logger, num, outdir=Path("data")):
    logger = Logger.log_setup(
        "blokboiImg", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
    logger.info("Beginning generating random scenes...")
    outdir = kwargs["path"]

    found = Path(outdir).glob("**/scene_*.png")
    if found != None:
        for path in found:
            os.remove(path)

    game_instance = Game()
    count = 0
    while count < kwargs["number"]:
        game_instance.newGame()
        ImageGen.make_image_from_str(
            game_instance.__repr__(), outdir / f"scene_{count:05d}.png"
        )
        count += 1
    logger.info(f"Finished making {count} scenes.")


def gui(**kwargs):
    logger = Logger.log_setup(
        "blokboi", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
    logger.debug("Running game app...")
    size = (15, 20)
    scale = kwargs["scale"]
    if kwargs["loadn"]:
        loader = MapLoader()
        scene, obj, coords, relation = loader.loadn(kwargs["loadn"])
        app = App(
            game_instance=Game(scene, obj, coords, relation),
            width=size[1],
            height=size[0],
            scale=scale,
        )
    else:
        app = App(game_instance=Game(), width=size[1], height=size[0], scale=scale)
    app.mainloop()


"""
################
ARGUMENT PARSING
################
"""

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Command line control for the Blokboi game API."
    )
    subparsers = parser.add_subparsers(title="subcommands")

    parser_gui = subparsers.add_parser(
        "gui", description="The GUI App for blokboi.", help="Run the GUI app."
    )
    parser_gui.add_argument(
        "-S", "--scale", type=int, default=2, help="Set the GUI scaling."
    )
    gui_loadgroup = parser_gui.add_mutually_exclusive_group()
    gui_loadgroup.add_argument(
        "-ln",
        "--loadn",
        nargs="?",
        const="demo",
        type=str,
        metavar="NAME",
        help="Load the demo scene NAME.",
    )
    gui_loadgroup.add_argument(
        "-l",
        "--load",
        nargs="?",
        const=1,
        type=int,
        metavar="NUM",
        help="Load the demo scene number NUM.",
    )
    parser_gui.add_argument(
        "--path",
        type=str,
        default="assets",
        help="Path to the assets directory (default: assets).",
    )
    parser_gui.add_argument(
        "--detail",
        default="info",
        choices=["debug", "info", "warning", "error", "critical"],
        help="Level of detail to python log (default: info).",
    )
    parser_gui.set_defaults(func=gui)

    parser_test = subparsers.add_parser(
        "test", description="Simple test of blokboi.", help="Run a simple test."
    )
    parser_test.add_argument(
        "--detail",
        default="info",
        choices=["debug", "info", "warning", "error", "critical"],
        help="Level of detail to python log (default: info).",
    )
    parser_test.set_defaults(func=test)

    parser_img = subparsers.add_parser(
        "img", description="Generate scene images.", help="Generate images."
    )
    parser_img.add_argument(
        "number", type=int, default=5000, help="The number of images to generate."
    )
    parser_img.add_argument(
        "--path",
        type=str,
        default="assets",
        help="Path to the assets directory (default: assets).",
    )
    parser_img.add_argument(
        "--detail",
        default="info",
        choices=["debug", "info", "warning", "error", "critical"],
        help="Level of detail to python log (default: info).",
    )
    parser_img.set_defaults(func=generate_rand_scenes)
    args = parser.parse_args()
    args.func(**vars(args))
