import tkinter as tk
from tkinter import ttk
from pathlib import Path
import logging
import os

from src.image_gen import ImageGen


class SceneFrame(ttk.Frame):
    def __init__(self, game_instance, container, assetpath: Path = Path("assets")):
        super().__init__(container)
        self.logger = logging.getLogger("blokboi.app.sceneframe")
        self.logger.info("Starting GUI scene frame.")

        options = {"padx": 0, "pady": 0}

        width = game_instance.width()
        height = game_instance.height()

        for col in range(width):
            self.columnconfigure(col, weight=1)

        for row in range(height):
            self.rowconfigure(row, weight=1)

        for row in range(width):
            for col in range(height):
                path = ImageGen.get_image_path(
                    game_instance.array()[row][col],
                    container._scale,
                    assetpath=assetpath,
                )
                self.logger.info(f"({row},{col}) image path: {str(path)}")
                self.logger.info("Opening..: " + str(os.path.exists(path)))
                continue

                image_labelx = ttk.Label(
                    container,
                    image=ImageGen.get_image_path(
                        game_instance.array()[row][col],
                        container._scale,
                        assetpath=assetpath,
                    ),
                    padding=0,
                )
                image_labelx.grid(column=col, row=row)

        self.pack(**options)

    def arrow_clicked(self):
        self.showinfo(title="Success", message="An arrow was clicked!")


class App(tk.Tk):
    def __init__(self, game_instance, scale=2):
        super().__init__()
        self.logger = logging.getLogger("blokboi.app")
        self.logger.info("Starting GUI app.")

        self._scale = scale

        self.title("Blokboi")
        self.geometry(
            f"{game_instance.width() * scale}x{game_instance.height() * scale}"
        )
        self.resizable(False, False)
