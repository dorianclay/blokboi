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
            (scene, objective, coordinates, relationship)
        """
        self.logger.info(f"Loading map {map_key}.")
        scene = np.load(self.datapath / f"map_{map_key}.npy")
        with open(self.datapath / f"map_{map_key}.json", "r") as jsonfile:
            obj_dict = json.load(jsonfile)
        return (
            scene,
            obj_dict["objective"],
            obj_dict["coordinates"],
            obj_dict["relationship"],
        )

    def loadn(self, map_name: str) -> tuple:
        """
        Load a map by name.

        ### Parameters
        `map_name : str`:
            the map name.

        ### Returns
        `tuple`
            (scene, objective, coordinates, relationship)
        """
        self.logger.info(f"Loading map {map_name}.")
        scene = np.load(self.datapath / "toy" / "scenes.npy")
        with open(self.datapath / "toy" / "scenes.json", "r") as jsonfile:
            objective_arr = json.load(jsonfile)
        count = 0
        for item in objective_arr:
            if item["id"] == map_name:
                break
            count += 1

        obj_dict = objective_arr[count]
        return (
            scene[count],
            obj_dict["objective"],
            obj_dict["relationship"],
            obj_dict["coordinates"],
            obj_dict["features"],
        )
