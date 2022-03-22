#ifndef GAME_H
#define GAME_H

#include "player_controller.h"
#include "scene.h"

class Game
{
  private:
    Scene *_scene;
    PlayerController *_player_controller;
    std::string objective = "";

  public:
    Game();
    void newGame();
    void resetGame();
    PlayerController *player_controller();
    std::string strprint();
};

#endif /* GAME_H */