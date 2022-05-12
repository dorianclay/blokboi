from tkinter import Image
import gym
from gym import spaces
import numpy as np
from pathlib import Path

from blokboi import Game
from src.image_gen import ImageGen
from src.map_loader import MapLoader

STEPLIMIT = 1000
GENERATE_ATTEMPTS = 3


def score(game_instance, resultfile):
    """
    Score and save the result of a game instance
    """
    if game_instance is None:
        return
    with open(resultfile, "wb") as npyfile:
        score = game_instance.score()
        np.save(npyfile, np.array(score))


class BlokboiEnv(gym.Env):
    """
    Custom Environment to play blokboi
    """

    metadata = {"render.modes": ["human"]}

    LEFT = 0
    RIGHT = 1
    JUMP = 2
    INTERACT = 3

    def __init__(
        self,
        height=15,
        width=20,
        steplimit=STEPLIMIT,
        datapath=Path("data"),
        resultfile=Path("results/rl.npy"),
    ):
        super(BlokboiEnv, self).__init__()

        self.height = height
        self.width = width
        self.steplimit = steplimit
        self.resultfile = resultfile
        self.loader = MapLoader(datapath / "rl")
        self.game_instance = None
        # Define the action space as a gym.spaces object
        # There are 4 discrete actions: left, right, jump, interact
        n_actions = 4
        self.action_space = spaces.Discrete(n_actions)
        # Define the observation space as images:
        #   let shape be the image shape for (HEIGHT, WIDTH, N_CHANNELS)
        self.observation_space = spaces.Box(
            low=0, high=255, shape=(height * 16, width * 16, 3), dtype=np.uint8
        )
        # self.observation_space = spaces.Dict({
        #     "observation": spaces.Box(low=0, high=255, shape=(height*16, width*16, 3), dtype=np.uint8),
        #     "desired_goal": spaces.Box(low=0, high=255, shape=(100,), dtype=np.uint8),
        #     "achieved_goal": spaces.Box(low=0, high=255, shape=(100,), dtype=np.uint8),
        # })

    def step(self, action):
        """
        Make one step in the environment

        ### Return
        observation, reward, done, info
        """
        # Call the blokboi C++ library for each action
        if action == self.LEFT:
            self.game_instance.move(-1)
        elif action == self.RIGHT:
            self.game_instance.move(1)
        elif action == self.JUMP:
            self.game_instance.jump()
        elif action == self.INTERACT:
            self.game_instance.toggle_hold()
        else:
            raise ValueError(
                f"Received invalid action={action} which is not part of the action space."
            )

        self.steps += 1

        # Check if the scene is done
        # Get the reward for the last step
        if self.steps > self.steplimit:
            done = True
            reward = -self.game_instance.reward()
            score(self.game_instance, self.resultfile)
        else:
            done = self.game_instance.success()
            if done:
                reward = 1
                achieved = "success"
                score(self.game_instance, self.resultfile)
            else:
                reward = -self.game_instance.reward()
                achieved = "walking"

        # Optionally, we can pass additional information (not used right now)
        info = {}

        # Get the observation at this step
        # observation = {"observation": np.array(ImageGen.image(self.game_instance.array())).astype(np.uint8),
        #         "desired_goal": np.array(list(map(ord, self.objective)), shape=(100,)),
        #         "achieved_goal": np.array(list(map(ord, achieved)), shape=(100,))}
        return (
            np.array(ImageGen.image(self.game_instance.array())).astype(np.uint8),
            reward,
            done,
            info,
        )

    def reset(self):
        """
        Reset the environment

        ### Return
        `Dict`
            observation
        """
        if self.game_instance is not None:
            self.loader.save(self.game_instance)

        for _ in range(GENERATE_ATTEMPTS):
            try:
                if self.game_instance is None:
                    self.game_instance = Game()
                else:
                    self.game_instance.newGame()
                break
            except RuntimeError:
                continue
        self.steps = 0
        self.objective = self.game_instance.objective()
        achieved = "started"
        return np.array(ImageGen.image(self.game_instance.array())).astype(np.uint8)
        # return {"observation": np.array(ImageGen.image(self.game_instance.array())).astype(np.uint8),
        #         "desired_goal": np.array(list(map(ord, self.objective)), shape=(100,)),
        #         "achieved_goal": np.array(list(map(ord, achieved)), shape=(100,))}

    def render(self, mode="human") -> Image:
        """
        Render the environment
        """
        if mode == "console":
            represent = self.game_instance.representation()
            for line in represent.split("\n"):
                print(line)
        else:
            return ImageGen.image(self.game_instance.array())

    def close(self):
        """
        Close the environment
        """
        pass
