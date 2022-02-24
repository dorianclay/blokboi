#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"
#include "block.h"
#include "location.h"

enum Direction
{
    LEFT,
    RIGHT,
};

class Player : public GameObject
{
  protected:
    Block _held;
    Direction _facing;

  public:
    Player();
    Player(Location *location);
    ~Player();

    void move(int x, int y);
    void jump();
    void pick_up();
    void put_down();
};

#endif /* PLAYER_H */