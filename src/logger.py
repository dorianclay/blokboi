"""
Class:  Logger
Author: Dorian Clay
Info:   This class simply initializes the logger, in order to keep main.py clean.
"""

import os
import logging


file_output = "logs/output.log"
file_debug = "logs/debug.log"


class Logger:
    def log_setup(
        name, detail="info", suppress_datetime=False, console=True, cleanLogs=True
    ):
        detail_map = {
            "debug": 5,
            "info": 4,
            "warning": 3,
            "error": 2,
            "critical": 1,
        }

        # Create clean log files:
        if cleanLogs:
            if os.path.exists(file_output):
                os.remove(file_output)
            if os.path.exists(file_debug):
                os.remove(file_debug)

        # Create logger.
        logger = logging.getLogger(name)
        logger.setLevel(logging.DEBUG)

        # Set format.
        formatter = None
        if suppress_datetime:
            formatter = logging.Formatter("%(name)s|%(levelname)s| %(message)s")
        else:
            formatter = logging.Formatter(
                "%(asctime)s|%(name)s|%(levelname)s| %(message)s"
            )

        # Create runtime log filehandler.
        fh = logging.FileHandler(file_output)
        if detail_map[detail] > 3:
            fh.setLevel(logging.INFO)
        else:
            fh.setLevel(logging.WARNING)
        fh.setFormatter(formatter)
        logger.addHandler(fh)

        if detail_map[detail] > 4:
            # Create debug log filehandler.
            dt = logging.FileHandler(file_debug)
            dt.setLevel(logging.DEBUG)
            dt.setFormatter(formatter)
            logger.addHandler(dt)

        # Create console log handler.
        if console:
            ch = logging.StreamHandler()
            ch.setLevel(logging.INFO)
            ch.setFormatter(formatter)
            logger.addHandler(ch)

        if suppress_datetime:
            logger.info(
                f"Logger created with level [{detail}]. Date and time suppressed."
            )
        else:
            logger.info(f"Logger created with level [{detail}].")

        # Return logger to use in main().
        return logger
