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
    Game(Char3d pregen);
    void newGame();
    void resetGame();
    int move(int direction);
    int jump();
    int toggle_hold();
    int pick_up();
    int put_down();
    int width()
    {
        return _scene->width();
    }
    int height()
    {
        return _scene->height();
    }
    Char3d array()
    {
        return _scene->array();
    }
    // const Objects scene_space() const
    // {
    //     return _scene->get_space();
    // }
    // const Scene *scene() const
    // {
    //     return _scene;
    // }
    std::string representation();
    std::string objective();
};

#endif /* GAME_H */
