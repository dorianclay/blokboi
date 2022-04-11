import argparse
from ast import arg
from PIL import Image
from pathlib import Path
import os


def main(**kwargs):
    assetspath = Path(kwargs["path"])

    paths = assetspath.glob("**/boi*.png")
    for path in paths:
        orig = Image.open(path)

        out = orig.transpose(Image.FLIP_LEFT_RIGHT)

        out.save(path.parent / f"{path.stem}_l.png")
        orig.save(path.parent / f"{path.stem}_r.png")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p",
        "--path",
        help="Path to assets directory",
        default="assets",
        type=str,
    )
    args = parser.parse_args()
    main(**vars(args))
