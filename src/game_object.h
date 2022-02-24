#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "location.h"

class GameObject
{
  protected:
    Location *_location;
    GameObject()
    {
    }

  public:
    ~GameObject()
    {
        delete _location;
    }
    Location *location() const
    {
        return _location;
    }
};

#endif /* GAME_OBJECT_H */