import os
from pathlib import Path
from tkinter import Image
import numpy as np
import logging
import json

from blokboi import Game
from src.image_gen import ImageGen


class MapLoader:
    def __init__(self, datapath=Path("data")):
        self.logger = logging.getLogger("blokboi.mapLoader")
        self.datapath = datapath

    def load(self, map_key: int) -> tuple:
        """
        Load a map with the map key.

        ### Parameters
        `map_key : int`:
            the map number key.

        ### Returns
        `tuple`
            (scene, objective, relationship, coordinates, ground truth)
        """
        self.logger.info(f"Loading map {map_key}.")
        scene = np.load(self.datapath / "scenes" / f"scene_{map_key:05d}.npy")
        with open(
            self.datapath / "objectives" / f"objective_{map_key:05d}.json", "r"
        ) as jsonfile:
            objective = json.load(jsonfile)
        return (
            scene,
            objective["objective"],
            objective["relationship"],
            objective["coordinates"],
            objective["features"],
            objective["ground truth"],
        )

    def loadm(self, map_key: int) -> tuple:
        """
        Load a mechanically-generated map with the map key.

        ### Parameters
        `map_key : int`:
            the map number key.

        ### Returns
        `tuple`
            (scene, objective, relationship, coordinates, ground truth)
        """
        self.logger.info(f"Loading map {map_key}.")
        scene = np.load(
            self.datapath / "mechanical" / "scenes" / f"scene_{map_key:05d}.npy"
        )
        with open(
            self.datapath
            / "mechanical"
            / "objectives"
            / f"objective_{map_key:05d}.json",
            "r",
        ) as jsonfile:
            objective = json.load(jsonfile)
        return (
            scene,
            objective["objective"],
            objective["relationship"],
            objective["coordinates"],
            objective["features"],
            objective["ground truth"],
        )

    def loadn(self, map_name: str) -> tuple:
        """
        Load a map by name.

        ### Parameters
        `map_name : str`:
            the map name.

        ### Returns
        `tuple`
            (scene, objective, relationship, coordinates, ground truth)
        """
        self.logger.info(f"Loading map {map_name}.")

        scenenum = -1
        paths = Path(self.datapath / "toy").glob("**/*.json")
        for filepath in paths:
            with open(filepath, "r") as jsonfile:
                obj_dict = json.load(jsonfile)
                if obj_dict["id"] == map_name:
                    objective = obj_dict
                    scenenum = int(filepath.stem.split("_")[1])
                    scene = np.load(
                        self.datapath / "toy/scenes" / f"scene_{scenenum:05d}.npy"
                    )
                    break
            scenenum += 1

        return (
            scene,
            objective["objective"],
            objective["relationship"],
            objective["coordinates"],
            objective["features"],
            objective["ground truth"],
        )

    def save(self, game_instance: Game) -> None:
        """
        Save the intial state of a blokboi game.

        ### Parameters
        `game_instance : Game`:
            A blokboi game

        ### Returns
        nothing
        """
        self.logger.info("Saving scene.")

        scene = game_instance.array()

        objective = {}
        objective["objective"] = game_instance.objective()
        objective["relationship"] = game_instance.relationship()
        objective["coordinates"] = game_instance.init_coords()
        objective["features"] = game_instance.feature_mask()
        objective["ground truth"] = game_instance.steps_taken()

        jsondir = self.datapath / "objectives"
        npydir = self.datapath / "scenes"
        imgdir = self.datapath / "images"

        if not os.path.exists(jsondir):
            os.makedirs(jsondir)
        if not os.path.exists(npydir):
            os.makedirs(npydir)
        if not os.path.exists(imgdir):
            os.makedirs(imgdir)

        existing = [
            int(name.split(".")[0].split("_")[1])
            for name in os.listdir(jsondir)
            if Path.is_file(jsondir / name)
        ]
        if len(existing) == 0:
            newnum = 0
        else:
            newnum = max(existing) + 1

        with open(jsondir / f"objective_{newnum:05d}.json", "w") as file:
            json.dump(objective, file)
        np.save(npydir / f"scene_{newnum:05d}.npy", scene)
        ImageGen.make_image_from_str(
            game_instance.init_data(), imgdir / f"image_{newnum:05d}.png"
        )
