import shutil
from PIL import Image
from pathlib import Path
import os
import argparse


def main(**kwargs):
    ratio = kwargs["ratio"]

    assetpath = Path(kwargs["assetpath"])

    if os.path.exists(assetpath / f"{ratio}x"):
        shutil.rmtree(assetpath / f"{ratio}x")

    os.mkdir(assetpath / f"{ratio}x")
    os.mkdir(assetpath / f"{ratio}x" / "blocks")

    paths = (assetpath / "1x").glob(f"**/*.png")
    for path in paths:
        orig = Image.open(path)
        relevant = path.relative_to(assetpath / "1x")

        size = [x * ratio for x in orig.size]

        out = orig.resize(size, resample=Image.NEAREST)

        out.save(assetpath / f"{ratio}x" / relevant)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("ratio", help="The ratio to convert to", type=int)
    parser.add_argument(
        "-p",
        "--path",
        help="Path to assets directory",
        default="assets",
        type=str,
    )
    args = parser.parse_args()
    main(**vars(args))
