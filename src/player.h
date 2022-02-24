#ifndef PLAYER_H
#define PLAYER_H

#include "location.h"
#include "block.h"

enum Direction {
    LEFT,
    RIGHT,
};

class Player
{
  protected:
    Location *_location;
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