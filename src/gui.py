import tkinter as tk
from tkinter import Image, StringVar, ttk
from pathlib import Path
from PIL import ImageTk
import logging

from blokboi import Game
from src.image_gen import ImageGen
from src.map_loader import MapLoader

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
                obj_list = self._container._game_instance.array()[row][col]
                self._draw(
                    row,
                    col,
                    self._assets[
                        ImageGen.get_image_path(
                            obj_list,
                            scale,
                            assetpath,
                        )
                    ],
                    obj_list,
                )

    def _draw(self, row, col, image, obj_list):
        block = 16 * self._container._scale
        tags = []
        if obj_list[0] == ".":
            return
        elif obj_list[0] == "@":
            tags.append("block")
        elif obj_list[0] == "P":
            tags.append("player")
        else:
            tags.append("block")
            tags.append(int(obj_list[1]))
        id = self.canvas.create_image(
            # x
            (block / 2) + block * row,
            # y
            (block / 2) + block * (self._container._sceneheight - 1 - col) + 1,
            # ImageTk.PhotoImage object
            image=image,
            tags=tags,
        )


class ButtonFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self.logger = logging.getLogger("blokboi.app.button")
        self._container = container

        self.rowconfigure(0, weight=1)

        self.__create_widgets()

    def __create_widgets(self):
        ttk.Button(self, text="reset", command=self.reset_clicked).grid(column=1, row=0)
        ttk.Button(self, text="new scene", command=self.new_scene_clicked).grid(
            column=2, row=0
        )
        ttk.Button(self, text="step", command=self.step_heuristic).grid(column=3, row=0)
        ttk.Button(self, text="save", command=self.save_scene).grid(column=0, row=0)

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

    def step_heuristic(self):
        self.logger.debug("Stepping heurisitic")
        success = self._container._game_instance.run_heuristic()
        self.logger.debug(f"    Got success: {'true' if success == 1 else 'false'}")
        self._container.render()

    def save_scene(self):
        self.logger.debug("Saving scene")
        self._container._maploader.save(self._container._game_instance)
        self.logger.info("Scene saved succesfully.")


class LabelFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self.logger = logging.getLogger("blokboi.app.label")
        self._container = container
        self.goal = StringVar()

        self.__create_widgets()

    def __create_widgets(self):
        self.update_goal()
        self.label = ttk.Label(self, textvariable=self.goal)
        self.label.pack()

    def update_goal(self):
        self.goal.set("Goal: " + self._container._game_instance.objective())

    def success(self):
        self.goal.set("SUCCESS!!")


class App(tk.Tk):
    def __init__(
        self,
        game_instance,
        width=20,
        height=15,
        *,
        scale=2,
        assetpath=Path("assets"),
        datapath=Path("data"),
    ):
        super().__init__()
        self.logger = logging.getLogger("blokboi.app")
        self.logger.info("Starting Blokboi app...")

        self._scale = scale
        self._scenewidth = width
        self._sceneheight = height
        self._assetpath = assetpath
        self._datapath = datapath
        self._maploader = MapLoader(datapath=datapath / "mechanical")

        self.title("Blokboi")
        self.tk.call(
            "wm",
            "iconphoto",
            self._w,
            tk.PhotoImage(file=str(assetpath / "2x" / "boi_1_r.png")),
        )
        self.__center(width * 16 * scale, height * 16 * scale + 48)
        self.resizable(False, False)

        self.rowconfigure(0, weight=2 * height)
        self.rowconfigure(0, weight=1)

        self._game_instance = game_instance
        self.logger.debug("Got map:\n" + str(self._game_instance))

        self.__create_widgets()
        self.__bindings()
        self._interrupt = False

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
        self.canvas_frame.grid(row=1, sticky=tk.N)

        self.button_frame = ButtonFrame(self)
        self.button_frame.grid(row=2)

        self.label_frame = LabelFrame(self)
        self.label_frame.grid(row=0)

    def __check_success(self):
        if self._game_instance.success():
            self.label_frame.success()

    def __bindings(self):
        self.bind("<Up>", self.__key_up)
        self.bind("<Left>", self.__key_left)
        self.bind("<Down>", self.__key_down)
        self.bind("<Right>", self.__key_right)
        self.bind("w", self.__key_up)
        self.bind("a", self.__key_left)
        self.bind("s", self.__key_down)
        self.bind("d", self.__key_right)
        self.bind("<Control-c>", self.__key_interrupt)

    def render(self):
        self.canvas_frame.render()
        self.label_frame.update_goal()
        self.__check_success()
        self.update()

    def __key_right(self, event):
        self._interrupt = False
        self.logger.debug(event)
        self._game_instance.move(RIGHT)
        self.render()

    def __key_left(self, event):
        self._interrupt = False
        self.logger.debug(event)
        self._game_instance.move(LEFT)
        self.render()

    def __key_up(self, event):
        self._interrupt = False
        self.logger.debug(event)
        self._game_instance.jump()
        self.render()

    def __key_down(self, event):
        self._interrupt = False
        self.logger.debug(event)
        self._game_instance.toggle_hold()
        self.render()

    def __key_interrupt(self, event):
        self.logger.debug(event)
        if self._interrupt:
            exit()
        self._interrupt = True


if __name__ == "__main__":
    # should never run!!
    app = App()
    app.mainloop()
