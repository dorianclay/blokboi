#ifndef PLAYER_H
#define PLAYER_H

#include "block.h"
#include "game_object.h"
#include "world.h"

enum Direction
{
    LEFT,
    RIGHT,
};

class Player : public GameObject
{
  protected:
    Block *_held;
    Direction _facing;

  public:
    Player();
    Player(World *world);
    Player(World *world, int x, int y);
    ~Player();

    void move(int x, int y);
    void jump();
    void pick_up();
    void put_down();
};

#endif /* PLAYER_H */