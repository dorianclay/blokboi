#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "block.h"
#include "player.h"
#include "scene.h"

class PlayerController
{
  private:
    Scene *_scene;
    Player *_player;
    Block *_held;

  public:
    PlayerController(Scene *scene, Player *player);
    void move(int direction);
    void jump();
    void pick_up();
    void put_down();
};

#endif /* PLAYER_CONTROLLER_H */