from csv import writer
import os
from matplotlib import pyplot as plt

import pandas as pd
from blokboi import Game
from src.map_loader import MapLoader
from src.logger import Logger
from pathlib import Path
import numpy as np
import multiprocessing as mp


SCORELIMIT = 500


def score_single_scene(logger, loader, file, resultfile, return_dict):
    # with open(resultfile, "ab") as csvfile:
    scene_id = file.stem.split("_")[1]
    scene, obj, relation, coords, features, truth = loader.load(scene_id)
    game_instance = Game(scene, obj, relation, coords, features)
    score = game_instance.verify(truth)
    # writer(csvfile).writerow([scene_id, score])
    # np.save(npyfile, np.array(score))
    return_dict[scene_id] = score


def score_heuristic_pregen(logger, **kwargs):
    """
    Score the heuristic solutions on pre-generated scenes.
    """
    logger.info("Scoring heuristic on pre-generated scenes...")
    resultpath = Path(kwargs["resultpath"])
    logger.info(f"Will put results in {resultpath}.")
    loader = MapLoader(Path(kwargs["datapath"]))

    resultfile = resultpath / "rl.csv"
    if not os.path.exists(resultpath):
        os.makedirs(resultpath)
    if os.path.exists(resultfile):
        os.remove(resultfile)

    paths = Path(kwargs["datapath"]).glob("**/*.json")
    scores = []
    count = 0

    manager = mp.Manager()
    return_dict = manager.dict()

    for file in paths:
        count += 1
        logger.info(f"Scoring item #{count}.")
        proc = mp.Process(
            target=score_single_scene,
            args=(logger, loader, file, resultfile, return_dict),
        )
        proc.start()
        proc.join()
        if count > SCORELIMIT:
            break

    with open(resultfile, "w") as csvfile:
        for scene_id, score in return_dict.items():
            data = [scene_id, score]
            writer(csvfile).writerow(data)
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
    resultfile = Path(kwargs["resultpath"]) / "heuristics.csv"

    scores = []
    count = 0
    total = 0

    # with open(resultfile, "rb") as npyfile:
    #     while True:
    #         try:
    #             score = np.load(npyfile)
    #             count += 1
    #             total += score
    #             scores.append(score)
    #         except ValueError:
    #             print("Reached EOF")
    #             break
    arr = np.loadtxt(resultfile, delimiter=",", dtype=np.int64)
    print(arr)
    scores = arr[:, 1]
    for elt in range(len(scores)):
        if scores[elt] < 0:
            scores[elt] = 1e9

    print("Scores:")
    print(scores)
    print(f"Avg: {total/len(scores)}")

    plt.title("Scores of Heuristic Solver")
    plt.ylim(1e4, 1e7)
    plt.plot(scores)
    plt.ylabel("Game Score")
    plt.yscale("log")
    plt.savefig(Path(kwargs["resultpath"]) / "heuristic_scores.png")


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
