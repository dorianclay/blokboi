import os
from blokboi import Game
from src.map_loader import MapLoader
from src.logger import Logger
from pathlib import Path
import numpy as np
import multiprocessing as mp


def score_single_scene(logger, loader, file, resultfile):
    with open(resultfile, "wb") as npyfile:
        scene_id = file.stem.split("_")[1]
        scene, obj, relation, coords, features, truth = loader.load(scene_id)
        game_instance = Game(scene, obj, relation, coords, features)
        score = game_instance.verify(truth)
        np.save(npyfile, np.array(score))


def score_heuristic_pregen(logger, **kwargs):
    """
    Score the heuristic solutions on pre-generated scenes.
    """
    logger.info("Scoring heuristic on pre-generated scenes...")
    resultpath = Path(kwargs["resultpath"])
    logger.info(f"Will put results in {resultpath}.")
    loader = MapLoader(Path(kwargs["datapath"]))

    resultfile = resultpath / "heuristics.npy"
    if resultfile.exists:
        os.remove(resultfile)
    if not os.path.exists(resultpath):
        os.makedirs(resultpath)

    paths = Path(kwargs["datapath"]).glob("**/*.json")
    scores = []
    count = 0

    for file in paths:
        count += 1
        logger.info(f"Scoring item #{count}.")
        proc = mp.Process(
            target=score_single_scene, args=(logger, loader, file, resultfile)
        )
        proc.start()
        proc.join()
    # with open(resultfile, "wb") as npyfile:
    # for file in paths:
    # count += 1
    # scene_id = file.stem.split("_")[1]
    # logger.info(f"Scoring item #{count}: scene {scene_id}.")
    # scene, obj, relation, coords, features, truth = loader.load(scene_id)
    # game_instance=Game(scene, obj, relation, coords, features)
    # score = game_instance.verify(truth)
    # scores.append(score)
    # np.save(npyfile, np.array(score))

    print(scores)
    logger.info("Done scoring.")


def analysis(logger, **kwargs):
    """
    Analyze given results
    """
    logger.info("Analyzing results.")
    resultfile = Path(kwargs["resultpath"]) / "heuristics.npy"

    scores = []
    count = 0
    total = 0

    with open(resultfile, "rb") as npyfile:
        while True:
            try:
                score = np.load(npyfile)
                count += 1
                total += score
                scores.append(score)
            except ValueError:
                print("Reached EOF")
                break

    print("Scores:")
    print(scores)
    print(f"Avg: {total/count}")


def score_heuristic(**kwargs):
    """
    Score the heuristic solution.
    """
    logger = Logger.log_setup(
        "heuristicscore", detail=kwargs["detail"], suppress_datetime=False, console=True
    )
    if kwargs["pregen"]:
        score_heuristic_pregen(logger, **kwargs)
    elif kwargs["analyze"]:
        analysis(logger, **kwargs)
    else:
        print("NOT IMPLEMENTED.")
