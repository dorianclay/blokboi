#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

// #include "world.h"

class World;

class GameObject
{
  protected:
    int _location[2];
    World *_world;

    GameObject()
    {
    }

  public:
    const int *location() const
    {
        return _location;
    }
};

#endif /* GAME_OBJECT_H */