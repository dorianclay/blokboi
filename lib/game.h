#ifndef GAME_H
#define GAME_H

#include "player_controller.h"
#include "scene.h"

class Game
{
  private:
    Scene *_scene;
    PlayerController *_player_controller;
    std::string _objective = "";

  public:
    Game();
    void newGame();
    void resetGame();
    void move(int direction);
    void jump();
    void pick_up();
    void put_down();
    int width() { return _scene->width(); }
    int height() { return _scene->height(); }
    const Objects scene_space() const;
    std::string representation();
    std::string objective();
};

#endif /* GAME_H */