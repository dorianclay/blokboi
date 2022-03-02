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
    bool _movable;

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
    bool movable() const
    {
        return _movable;
    }
};

class Ground : public Block
{
  protected:
  public:
    Ground(int x, int y);
};

std::ostream &operator<<(std::ostream &ostr, const Block &block);
std::ostream &operator<<(std::ostream &ostr, const Ground &ground);


#endif /* BLOCK_H */