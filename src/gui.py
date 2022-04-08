import tkinter as tk
from tkinter import ttk
from pathlib import Path
import logging

from src.image_gen import ImageGen


class SceneFrame(ttk.Frame):
    def __init__(self, container, game_instance, assetpath: Path = Path("assets")):
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

        row = 0
        col = 0
        path = ImageGen.get_image_path(
            game_instance.array()[row][col],
            container._scale,
            assetpath=assetpath,
        )
        self.logger.info(f"Path: {path}")
        photo = tk.PhotoImage(file=str(path))
        self.image_labelx = ttk.Label(
            self,
            image=photo,
            padding=0,
        )
        self.image_labelx.pack(**options)

        label = ttk.Label(self, text="Hello, owrkd!")
        label.pack(**options)
        self.pack(**options)

        # for row in range(width):
        #     for col in range(height):
        #         # image_labelx = ttk.Label(
        #         #     container,
        #         #     image=tk.PhotoImage(str(ImageGen.get_image_path(
        #         #         game_instance.array()[row][col],
        #         #         container._scale,
        #         #         assetpath=assetpath,
        #         #     ))),
        #         #     padding=0,
        #         # )
        #         # image_labelx.grid(column=col, row=row)
        #         self.label=ttk.Label(self, text='t')
        #         self.label.grid(column=col, row=row)

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
            f"{game_instance.width() * 16 * scale}x{game_instance.height() * 16 * scale}"
        )
        self.resizable(False, False)


if __name__ == "__main__":
    # should never run!!
    app = App()
    app.mainloop()
