import os
from pathlib import Path
from matplotlib import pyplot as plt
import numpy as np
import pandas as pd
from stable_baselines.common.env_checker import check_env
from stable_baselines import DQN, PPO2, A2C, ACKTR
from stable_baselines.common.cmd_util import make_vec_env
from stable_baselines.bench import Monitor
import logging

from src.blokboi_env import BlokboiEnv
from src.logger import Logger


class RL:
    def main(**kwargs):
        logger = Logger.log_setup("RL training")
        logger.info("Training the agent...")

        # Instantiate the environment
        env = BlokboiEnv()

        log_dir = "logs"
        monitor_env = Monitor(env, log_dir, allow_early_resets=True)
        # Wrap it
        env = make_vec_env(lambda: monitor_env, n_envs=1)

        monitor_env

        # check_env(env, warn=True)

        model = PPO2("MlpPolicy", env, verbose=0).learn(10000)

        with open(os.path.join(log_dir, "monitor.csv"), "rt") as fh:
            firstline = fh.readline()
            assert firstline[0] == "#"
            df = pd.read_csv(fh, index_col=None)["r"]
        df.rolling(window=1).mean().plot()  # window was 1000
        # plt.show()
        plt.savefig(Path(kwargs["resultpath"]) / "reinforcement.png")

        # RL.test(env, model, ntests=500, logger=logger, **kwargs)

    def quicktest(env, model, logger=logging.getLogger("RL quicktest")):
        logger.warn("Quickly testing...")

        obs = env.reset()
        n_steps = 1000
        for step in range(n_steps):
            action, _ = model.predict(obs, deterministic=True)
            print(f"Step {step+1}")
            print(f"Action: {action}")
            obs, reward, done, info = env.step(action)
            # print('obs=', obs, 'reward=', reward, 'done=', done)
            # env.render(mode='console')
            if done:
                # Note that the VecEnv resets automatically
                # when a done signal is encountered
                print("Goal reached!", "reward=", reward)
                break

    def test(env, model, ntests=200, logger=logging.getLogger("RL test"), **kwargs):
        import os
        from pathlib import Path

        logger.warn("Testing the results...")
        resultpath = Path(kwargs["resultpath"])
        logger.info(f"Will put results in {resultpath}.")

        resultfile = resultpath / "reinforcement.npy"
        if not os.path.exists(resultpath):
            os.makedirs(resultpath)
        if os.path.exists(resultfile):
            os.remove(resultfile)

        n_steps = 1000
        for test in range(ntests):
            obs = env.reset()
            for step in range(n_steps):
                action, _ = model.predict(obs, deterministic=True)
                obs, reward, done, info = env.step(action)
                if done:
                    logger.info("Goal reached!", "reward=", reward)
                    break

            logger.info(f"Recording result {test+1}: score={reward}.")
            with open(resultfile, "wb") as npyfile:
                score = reward
                np.save(npyfile, np.array(score))
