#ifndef BLOCK_H
#define BLOCK_H

#include "game_object.h"
#include "world.h"

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

class Block : public GameObject
{
  protected:
    Color _color;
    int _number;
    bool _movable;

  public:
    Block();
    Block(World *world, int x, int y);
    Block(World *world, int x, int y, Color color, int number, bool movable);

    void pick_up();
    void move(int x, int y);

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
    Ground(World *world, int x, int y);
};

#endif /* BLOCK_H */