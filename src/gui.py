import tkinter as tk
from tkinter import ttk
from pathlib import Path
from PIL import ImageTk
import logging

from src.image_gen import ImageGen


class CanvasFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self._scale = container._scale

        # Initialize canvas
        self.canvas = tk.Canvas(
            self,
            bg=f"#{ImageGen.get_sky_hex()}",
            width=container._scenewidth * 16 * self._scale,
            height=container._sceneheight * 16 * self._scale,
        )
        self.canvas.pack()
        self.pack()

        # Set up the dictionary of images
        paths = (container._assetpath / f"{self._scale}").glob(f"**/*.png")
        self._assets = {}
        for path in paths:
            self._assets[path] = ImageTk.PhotoImage(file=str(path))

    def _draw(self, row, col, image):
        block = 16 * self._scale
        self.canvas.create_image(
            (block / 2) + block * row, (block / 2) + block * col, image=image
        )


class ButtonFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)

        self.rowconfigure(0, weight=1)

        self.__create_widgets()

    def __create_widgets(self):
        ttk.Button(self, text="reset").grid(column=0, row=0)
        ttk.Button(self, text="new scene").grid(column=1, row=0)

        for widget in self.winfo_children():
            widget.grid(padx=8, pady=0)


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
        #         #         container.scale,
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
    def __init__(self, width=20, height=15, *, scale=2, assetpath=Path("assets")):
        super().__init__()
        self.logger = logging.getLogger("blokboi.app")
        self.logger.info("Starting GUI app.")

        self._scale = scale
        self._scenewidth = width
        self._sceneheight = height
        self._assetpath = assetpath

        self.title("Blokboi")
        self.__center(width * 16 * scale, height * 16 * scale + 8)
        self.resizable(False, False)

        self.rowconfigure(0, weight=2 * height)
        self.rowconfigure(0, weight=1)

        self.__create_widgets()

    def __center(self, width=None, height=None):
        if width is None or height is None:
            width = self.winfo_width()
            height = self.winfo_height()

        center_x = int(self.winfo_screenwidth() / 2 - width / 2)
        center_y = int(self.winfo_screenheight() / 2 - height / 2)

        # set the position of the window to the center of the screen
        self.geometry(f"{width}x{height}+{center_x}+{center_y}")

    def __create_widgets(self):
        canvas_frame = CanvasFrame(self)
        canvas_frame.grid(row=0)

        button_frame = ButtonFrame(self)
        button_frame.grid(row=1)


if __name__ == "__main__":
    # should never run!!
    app = App()
    app.mainloop()
