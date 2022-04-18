import logging


class Heuristic:
    def __init__(self, game_instance):
        self.logger = logging.getLogger("blokboi.heuristic")
        self.logger.info("solving map with heuristic...")
