from pathlib import Path
import numpy as np
import logging
import json

from blokboi import Game


class MapLoader:

    name_dict = {
        "demo": "demo_scene",
    }

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
