#ifndef BLOCK_H
#define BLOCK_H

#include "effolkronium/random.hpp"
#include "game_object.h"
#include <ostream>

#define MIN_NUMBER 0
#define MAX_NUMBER 9

// enum Color
// {
//     RED = 'r',
//     ORANGE = 'o',
//     YELLOW = 'y',
//     GREEN = 'g',
//     BLUE = 'b',
//     PURPLE = 'p',
//     GREY = 'q',
//     COUNT
// };

class Block : public GameObject
{
  protected:
    char colors[7] = {'r', 'o', 'y', 'g', 'b', 'p', 'G'};
    char _color;
    int _number;

  public:
    Block();
    Block(int x, int y);
    Block(int x, int y, char color, int number, bool movable);

    int color() const
    {
        return _color;
    }
    int number() const
    {
        return _number;
    }
    int kind() const
    {
        return _color;
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
