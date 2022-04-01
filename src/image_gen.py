from PIL import Image
from pathlib import Path
import logging

assets = {}

paths = Path('assets').glob(f'**/*.png')
for path in paths:
    assets[path.stem] = Image.open(path)

class ImageGen:

    _asset_map = {
        '.': 'sky',
        '@': 'ground',
        'r': 'block_red_x',
        'o': 'block_orange_x',
        'y': 'block_yellow_x',
        'g': 'block_green_x',
        'b': 'block_blue_x',
        'p': 'block_purple_x',
        'P': 'boi_2'
    }

    _skycolor = (90,192,236)

    def make_image_from_str(mapstring: str, path: Path) -> None:
        """
        Generates an image from a string representation

        ### Parameters
        `mapstring : str`:
            The string representation of a Blokboi map.
        `path : Path`:
            The path to save the image to.        
        """
        logger = logging.getLogger('image_gen')
        lines = mapstring.split('\n')
        width = len(lines[0]) * 16
        height = len(lines) * 16
        newImage = Image.new('RGB', (width, height), ImageGen._skycolor)
        count_r = 0
        for row in lines:
            count_c = 0
            for col in row:
                region = assets[ImageGen._asset_map[col]].crop((0,0,16,16))
                newImage.paste(region, 
                                (count_c*16, count_r*16, (count_c+1)*16, (count_r+1)*16),
                                mask=region)
                count_c += 1
            count_r += 1
        newImage.save(path)