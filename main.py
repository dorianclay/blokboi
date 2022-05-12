import argparse
import os
import io
import json
from pathlib import Path
from contextlib import redirect_stderr
from pydoc import describe
from sqlite3 import paramstyle
import numpy as np
from yaml import parse


from blokboi import Game
from src.logger import Logger
from src.image_gen import ImageGen
from src.gui import *
from src.map_loader import MapLoader
from util.gen_ex_maps import generate_toys
from util.score_heuristic import score_heuristic
from run_tests import LocalTestRunner
from src.pycolors import Colors


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


def generate(**kwargs):
    logger = Logger.log_setup(
        "blokboiGen", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
    logger.info("Beginning generating random scenes...")
    datapath = Path(kwargs["datapath"])

    files_grabbed = []
    [
        files_grabbed.extend(Path(datapath).glob(f"*/*.{ext}"))
        for ext in MapLoader.filetypes()
    ]
    if files_grabbed != None:
        if kwargs["yes"]:
            confirmation = "yes"
        else:
            confirmation = input("Overwrite existing files? [Y/n]: ")
        if confirmation not in ["y", "yes", "Yes", "yes", ""]:
            print("Exiting.")
            exit()
        for path in files_grabbed:
            os.remove(path)

    loader = MapLoader()
    count = 0
    game_instance = Game()
    while count < kwargs["number"]:
        try:
            game_instance.newGame()
        except RuntimeError:
            logger.exception()
            logger.info(f"Failed to make a playable scene. Asking for input...")
            if kwargs["yes"]:
                confirmation = "yes"
            else:
                confirmation = input(
                    "Failed to make a playable scene. Would you like to continue? [y/N]: "
                )
            if confirmation in ["yes", "Yes", "y", "Y"]:
                logger.info("...Continuing.")
                continue
            else:
                break
        else:
            loader.save(game_instance)
            count += 1
            logger.info(f"Finished making {count} scenes.")


def gui(**kwargs):
    logger = Logger.log_setup(
        "blokboi", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
    logger.debug("Running game app...")
    size = (15, 20)
    scale = kwargs["scale"]
    loader = MapLoader()
    if kwargs["list"]:
        print(f"{Colors.underline}Maps available:{Colors.reset}")
        [print(name) for name in sorted(loader.list())]
    else:
        if kwargs["loadn"]:
            scene, obj, relation, coords, features, truth = loader.loadn(
                kwargs["loadn"]
            )
            app = App(
                game_instance=Game(scene, obj, relation, coords, features),
                width=size[1],
                height=size[0],
                scale=scale,
                datapath=Path(kwargs["datapath"]),
            )
        elif kwargs["loadm"]:
            scene, obj, relation, coords, features, truth = loader.loadm(
                kwargs["loadm"]
            )
            app = App(
                game_instance=Game(scene, obj, relation, coords, features),
                width=size[1],
                height=size[0],
                scale=scale,
                datapath=Path(kwargs["datapath"]),
            )
        elif kwargs["load"]:
            scene, obj, relation, coords, features, truth = loader.load(kwargs["load"])
            app = App(
                game_instance=Game(scene, obj, relation, coords, features),
                width=size[1],
                height=size[0],
                scale=scale,
                datapath=Path(kwargs["datapath"]),
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
    parser.add_argument(
        "--detail",
        default="info",
        choices=["debug", "info", "warning", "error", "critical"],
        help="Level of detail to python log (default: info).",
    )
    parser.add_argument(
        "--datapath",
        type=str,
        default="data",
        help="Path to the data directory (default: data).",
    )
    parser.add_argument(
        "--assetpath",
        type=str,
        default="assets",
        help="Path to the asset directory (default: assets).",
    )
    parser.add_argument(
        "--resultpath",
        type=str,
        default="results",
        help="Path to the analysis result directory (default: results).",
    )

    # Main subparsers
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
        help="Load a toy example scene NAME.",
    )
    gui_loadgroup.add_argument(
        "-lm",
        "--loadm",
        type=int,
        metavar="NUM",
        help="Load the mechanically-generated scene NUM.",
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
    gui_loadgroup.add_argument(
        "--list", action="store_true", help="List the named scenes available."
    )
    parser_gui.set_defaults(func=gui)

    parser_test = subparsers.add_parser(
        "test", description="Simple test of blokboi.", help="Run a simple test."
    )
    parser_test.set_defaults(func=test)

    parser_gen = subparsers.add_parser(
        "generate",
        description="Generate scene dataset.",
        help="Generate scene dataset.",
    )
    parser_gen.add_argument(
        "number",
        type=int,
        default=5000,
        help="The number of scenes to generate. (default: 5000)",
    )
    parser_gen.add_argument(
        "--image_only", action="store_true", help="Only generate images."
    )
    parser_gen.add_argument(
        "--heuristic_limit",
        default=100,
        type=int,
        metavar="NUM",
        help="The number of times to try building a playable scene. (default: 100)",
    )
    parser_gen.add_argument(
        "-y", "--yes", action="store_true", help="Yes to all prompts."
    )
    parser_gen.set_defaults(func=generate)

    parser_util = subparsers.add_parser(
        "util", description="Utility commands.", help="Utility commands."
    )

    # Utility subparsers
    util_parsers = parser_util.add_subparsers(title="utilities")
    parser_toy = util_parsers.add_parser(
        "toygen",
        description="Generate toy examples scenes.",
        help="Generate toy example scenes.",
    )
    parser_toy.set_defaults(func=generate_toys)

    parser_heur_score = util_parsers.add_parser(
        "score_heuristic",
        description="Score the heuristic solutions.",
        help="Score the heuristic solutions.",
    )
    parser_heur_score.add_argument(
        "--pregen",
        action="store_true",
        help="Score the 'ground truth' on pregenerated maps.",
    )
    parser_heur_score.add_argument(
        "--analyze", action="store_true", help="Perform analysis on the scores."
    )
    parser_heur_score.set_defaults(func=score_heuristic)

    args = parser.parse_args()
    args.func(**vars(args))
