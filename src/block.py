from enum import Enum
from src.game_object import GameObject

class Color(Enum):
    RED = 1
    ORANGE = 2
    YELLOW = 3
    GREEN = 4
    BLUE = 5
    PURPLE = 6
    GREY = 7 

class Block(GameObject):

    def __init__(self):
        self.color = Color.GREY