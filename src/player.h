#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"
#include <ostream>

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

std::ostream &operator<<(std::ostream &ostr, const Player &player);


#endif /* PLAYER_H */