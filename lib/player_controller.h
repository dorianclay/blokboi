#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "block.h"
#include "player.h"
#include "scene.h"

class PlayerController {
private:
  Scene *_scene;
  Player *_player;

public:
  PlayerController(Scene *scene, Player *player);
  int move(int direction);
  int jump();
  int pick_up();
  int put_down();
  bool holding();
};

#endif /* PLAYER_CONTROLLER_H */
