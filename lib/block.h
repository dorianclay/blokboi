#ifndef BLOCK_H
#define BLOCK_H

#include "game_object.h"
#include <ostream>

#define MIN_NUMBER 0
#define MAX_NUMBER 9

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

class Block : public GameObject
{
  protected:
    Color _color;
    int _number;

  public:
    Block();
    Block(int x, int y);
    Block(int x, int y, Color color, int number, bool movable);

    int color() const
    {
        return _color;
    }
    int number() const
    {
        return _number;
    }
    void repr(std::ostream &ostr) const;
};

class Ground : public Block
{
  protected:
  public:
    Ground(int x, int y);
    void repr(std::ostream &ostr) const;
};

#endif /* BLOCK_H */