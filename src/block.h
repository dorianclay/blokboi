#ifndef BLOCK_H
#define BLOCK_H

#include "location.h"

int MIN_NUMBER = 0;
int MAX_NUMBER = 9;

enum Color
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE,
    GREY,
    COUNT
};

class Block
{
  protected:
    Color _color;
    int _number;
    bool _movable;
    Location *_location;

  public:
    Block();
    Block(Location *location);
    Block(Color color, int number, bool movable, Location *location);
    ~Block();

    void pick_up();

    int color() const
    {
        return _color;
    }
    int number() const
    {
        return _number;
    }
    bool movable() const
    {
        return _movable;
    }
    const Location *location() const
    {
        return _location;
    }
};

class Ground : public Block
{
  protected:
    Ground();
    ~Ground();

  public:
    Ground(Location *location);
};

#endif /* BLOCK_H */