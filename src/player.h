#ifndef PLAYER_H
#define PLAYER_H

#include "block.h"
#include "game_object.h"
#include "scene.h"

enum Direction
{
    LEFT = -1,
    RIGHT = 1,
};

class Player : public GameObject
{
  protected:
    GameObject *_held;
    Direction _facing;

  public:
    Player();
    Player(int x, int y);

    void move(int x, int y);
    void jump();
    void pick_up();
    void put_down();
};

#endif /* PLAYER_H */