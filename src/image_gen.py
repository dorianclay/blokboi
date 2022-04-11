from PIL import Image
from pathlib import Path
import logging

assets = {}

paths = Path("assets/1x").glob(f"**/*.png")
for path in paths:
    assets[path] = Image.open(path)


class ImageGen:

    _str_path_map = {
        ".": "sky",
        "@": "ground",
        "r": "blocks/block_red_x",
        "o": "blocks/block_orange_x",
        "y": "blocks/block_yellow_x",
        "g": "blocks/block_green_x",
        "b": "blocks/block_blue_x",
        "p": "blocks/block_purple_x",
        "P": "boi_2",
    }

    _pathname_map = {
        ".": "sky",
        "@": "ground",
        "r": "blocks/block_red",
        "o": "blocks/block_orange",
        "y": "blocks/block_yellow",
        "g": "blocks/block_green",
        "b": "blocks/block_blue",
        "p": "blocks/block_purple",
        "P": "boi",
    }

    _skycolor = (90, 192, 236)

    def make_image_from_str(mapstring: str, path: Path, scale: int = 1) -> None:
        """
        Generates an image from a string representation

        ### Parameters
        `mapstring : str`:
            The string representation of a Blokboi map.
        `path : Path`:
            The path to save the image to.
        """
        logger = logging.getLogger("image_gen")
        lines = mapstring.split("\n")
        width = len(lines[0]) * 16
        height = len(lines) * 16
        newImage = Image.new("RGB", (width, height), ImageGen._skycolor)
        count_r = 0
        for row in lines:
            count_c = 0
            for col in row:
                # TODO: update this key for an unnumbered block
                region = assets[ImageGen.get_image_path([col, -1], scale)].crop(
                    (0, 0, 16, 16)
                )
                newImage.paste(
                    region,
                    (
                        count_c * 16,
                        count_r * 16,
                        (count_c + 1) * 16,
                        (count_r + 1) * 16,
                    ),
                    mask=region,
                )
                count_c += 1
            count_r += 1
        newImage.save(path)

    def get_image_path(
        obj_arr: list, scale: int = 1, assetpath: Path = Path("assets")
    ) -> Path:
        """
        Returns the path to the image that corresponds with an object.

        ### Parameters
        `obj_arr : list`:
            The list representation of a Blokboi GameObject.
        `scale : int`:
            The scale of the image.
        `assetpath : Path`:
            The path to the assets directory.

        ### Returns
        `Path`
            The Path of the image.
        """
        if obj_arr[0] in [".", "@"]:
            return assetpath / f"{scale}x" / f"{ImageGen._pathname_map[obj_arr[0]]}.png"
        elif obj_arr[0] == "P":
            # TODO: make boi not static.
            if obj_arr[1] == 1:
                return (
                    assetpath
                    / f"{scale}x"
                    / f"{ImageGen._pathname_map[obj_arr[0]]}_2_r.png"
                )
            else:
                return (
                    assetpath
                    / f"{scale}x"
                    / f"{ImageGen._pathname_map[obj_arr[0]]}_2_l.png"
                )
        elif obj_arr[1] == "X":
            return (
                assetpath / f"{scale}x" / f"{ImageGen._pathname_map[obj_arr[0]]}_x.png"
            )
        else:
            return (
                assetpath
                / f"{scale}x"
                / f"{ImageGen._pathname_map[obj_arr[0]]}_{obj_arr[1]}.png"
            )

    def get_sky_hex():
        return "%02x%02x%02x" % ImageGen._skycolor

    def get_sky_rgb():
        return ImageGen._skycolor
