import shutil
from PIL import Image
from pathlib import Path
import os
import argparse


def main(**kwargs):
    ratio = kwargs["ratio"]

    assetspath = Path(kwargs["path"])

    if os.path.exists(assetspath / f"{ratio}x"):
        shutil.rmtree(assetspath / f"{ratio}x")

    os.mkdir(assetspath / f"{ratio}x")
    os.mkdir(assetspath / f"{ratio}x" / "blocks")

    paths = (assetspath / "1x").glob(f"**/*.png")
    for path in paths:
        orig = Image.open(path)
        relevant = path.relative_to(assetspath / "1x")

        size = [x * ratio for x in orig.size]

        out = orig.resize(size, resample=Image.NEAREST)

        out.save(assetspath / f"{ratio}x" / relevant)


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
