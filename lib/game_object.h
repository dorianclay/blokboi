#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <ostream>

enum Kind
{
    SKY = '.',
    BLOCK = 'X',
    GROUND = '@',
    PLAYER = 'P'
};

typedef struct location
{
    int x;
    int y;
} LOCATION;

class GameObject
{
  protected:
    LOCATION _location;
    Kind _kind;
    bool _movable = false;

    GameObject()
    {
    }

  public:
    // accessors
    virtual int kind() const
    {
        return _kind;
    }
    virtual int number() const
    {
        return 'X';
    }
    const LOCATION location() const
    {
        return _location;
    }
    virtual void repr(std::ostream &ostr) const
    {
    }
    bool movable() const
    {
        return _movable;
    }
    // mutators
    void update(int x, int y)
    {
        _location.x = x;
        _location.y = y;
    }
};

#endif /* GAME_OBJECT_H */
