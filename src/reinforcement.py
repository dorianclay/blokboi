import numpy as np
from stable_baselines.common.env_checker import check_env
from stable_baselines import DQN, PPO2, A2C, ACKTR
from stable_baselines.common.cmd_util import make_vec_env
import logging

from src.blokboi_env import BlokboiEnv
from src.logger import Logger


class RL:
    def main(**kwargs):
        logger = Logger.log_setup("RL training")
        logger.info("Training the agent...")

        # Instantiate the environment
        env = BlokboiEnv()
        # Wrap it
        env = make_vec_env(lambda: env, n_envs=1)

        # check_env(env, warn=True)

        model = PPO2("MlpPolicy", env, verbose=0).learn(100)

        RL.test(env, model, ntests=500, logger=logger, **kwargs)
        # n_steps = 20
        # for step in range(n_steps):
        #     print(f"Step {step+1}")
        #     obs, reward, done, info = env.step(0)
        #     print('obs=', obs, 'reward=', reward, 'done=', done)
        #     env.render(mode="console")
        #     if done:
        #         print("Goal reached!", "reward=", reward)
        #         break
        # print(f"Finished {n_steps} steps. reward={reward}")

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

    def test(env, model, ntests=2500, logger=logging.getLogger("RL test"), **kwargs):
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
