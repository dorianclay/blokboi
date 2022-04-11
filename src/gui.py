import tkinter as tk
from tkinter import Image, ttk
from pathlib import Path
from PIL import ImageTk
import logging

from src.image_gen import ImageGen
from blokboi import Game

LEFT = -1
RIGHT = 1


class CanvasFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container, takefocus=True)
        self._container = container
        self.logger = logging.getLogger("blokboi.app.canvas")
        self.logger.debug("Initializing canvas to draw on.")

        # Initialize canvas
        scale = self._container._scale
        self.canvas = tk.Canvas(
            self,
            bg=f"#{ImageGen.get_sky_hex()}",
            width=container._scenewidth * 16 * scale,
            height=container._sceneheight * 16 * scale,
        )
        self.canvas.pack()
        self.pack()

        # Set up the dictionary of images
        paths = (container._assetpath / f"{scale}x").glob(f"**/*.png")
        self._assets = {}
        for path in paths:
            self._assets[path] = ImageTk.PhotoImage(file=str(path))

        self.render()

    def render(self):
        width = self._container._scenewidth
        height = self._container._sceneheight
        assetpath = self._container._assetpath
        scale = self._container._scale
        self.canvas.delete("all")
        for row in range(width):
            for col in range(height):
                self._draw(
                    row,
                    col,
                    self._assets[
                        ImageGen.get_image_path(
                            self._container._game_instance.array()[row][col],
                            scale,
                            assetpath,
                        )
                    ],
                )

    def _draw(self, row, col, image):
        block = 16 * self._container._scale
        self.canvas.create_image(
            # x
            (block / 2) + block * row,
            # y
            (block / 2) + block * (self._container._sceneheight - 1 - col) + 1,
            # ImageTk.PhotoImage object
            image=image,
        )


class ButtonFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self.logger = logging.getLogger("blokboi.app.button")
        self._container = container

        self.rowconfigure(0, weight=1)

        self.__create_widgets()

    def __create_widgets(self):
        ttk.Button(self, text="reset", command=self.reset_clicked).grid(column=0, row=0)
        ttk.Button(self, text="new scene", command=self.new_scene_clicked).grid(
            column=1, row=0
        )

        for widget in self.winfo_children():
            widget.grid(padx=16, pady=0)

    def reset_clicked(self):
        self.logger.debug("Reset clicked")
        self._container._game_instance.resetGame()
        self._container.render()

    def new_scene_clicked(self):
        self.logger.debug("New Scene clicked")
        self._container._game_instance.newGame()
        self._container.render()


class App(tk.Tk):
    def __init__(self, width=20, height=15, *, scale=2, assetpath=Path("assets")):
        super().__init__()
        self.logger = logging.getLogger("blokboi.app")
        self.logger.info("Starting Blokboi app...")

        self._scale = scale
        self._scenewidth = width
        self._sceneheight = height
        self._assetpath = assetpath

        self.title("Blokboi")
        self.__center(width * 16 * scale, height * 16 * scale + 48)
        self.resizable(False, False)

        self.rowconfigure(0, weight=2 * height)
        self.rowconfigure(0, weight=1)

        self._game_instance = Game()

        self.__create_widgets()
        self.__bindings()

    def __center(self, width=None, height=None):
        if width is None or height is None:
            width = self.winfo_width()
            height = self.winfo_height()

        center_x = int(self.winfo_screenwidth() / 2 - width / 2)
        center_y = int(self.winfo_screenheight() / 2 - height / 2)

        # set the position of the window to the center of the screen
        self.geometry(f"{width}x{height}+{center_x}+{center_y}")

    def __create_widgets(self):
        self.canvas_frame = CanvasFrame(self)
        self.canvas_frame.grid(row=0, sticky=tk.N)

        self.button_frame = ButtonFrame(self)
        self.button_frame.grid(row=1)

    def __bindings(self):
        self.bind("<Up>", self.__key_up)
        self.bind("<Left>", self.__key_left)
        self.bind("<Down>", self.__key_down)
        self.bind("<Right>", self.__key_right)
        self.bind("w", self.__key_up)
        self.bind("a", self.__key_left)
        self.bind("s", self.__key_right)
        self.bind("d", self.__key_down)

    def render(self):
        self.canvas_frame.render()

    def __key_right(self, event):
        self.logger.debug(event)
        # self._game_instance.move(RIGHT)
        # self.render()

    def __key_left(self, event):
        self.logger.debug(event)
        # self._game_instance.move(LEFT)
        # self.render()

    def __key_up(self, event):
        self.logger.debug(event)
        # self._game_instance.jump()
        # self.render()

    def __key_down(self, event):
        self.logger.debug(event)
        # self._game_instance.pick_up()
        # self.render()


if __name__ == "__main__":
    # should never run!!
    app = App()
    app.mainloop()
