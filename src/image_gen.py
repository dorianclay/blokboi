from PIL import Image
from pathlib import Path
import logging

assets = {}

paths = Path('assets').glob(f'**/*.png')
for path in paths:
    assets[path.stem] = Image.open(path)

class ImageGen:

    def make_image_from(mapstring: str, path: Path) -> None:
        logger = logging.getLogger('image_gen')
        pass