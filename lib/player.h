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

    Direction facing()
    {
        return _facing;
    }
    void face(int direction);
    GameObject *held()
    {
        return _held;
    }
    void hold(GameObject *block);
    void repr(std::ostream &ostr) const;
    int number() const override;
};

#endif /* PLAYER_H */
