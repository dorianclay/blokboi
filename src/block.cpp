#include "block.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Block::Block()
{
    Block(0, 0);
}

Block::Block(int x, int y)
{
    _location.x = x;
    _location.y = y;
    _color = static_cast<Color>(rand() % COUNT);
    srand(time(0));
    _number = 1 + (rand() % (MAX_NUMBER - 1));
    _movable = true;
}

Block::Block(int x, int y, Color color, int number, bool movable) : _color(color), _number(number), _movable(movable)
{
    _location.x = x;
    _location.y = y;
}

void Block::repr(ostream &ostr) const
{
    if (_color == RED)
        ostr << "r";
    else if (_color == ORANGE)
        ostr << "o";
    else if (_color == YELLOW)
        ostr << "y";
    else if (_color == GREEN)
        ostr << "g";
    else if (_color == BLUE)
        ostr << "b";
    else if (_color == PURPLE)
        ostr << "p";
}

Ground::Ground(int x, int y)
{
    _location.x = x;
    _location.y = y;
    _number = -1;
    _color = GREY;
    _movable = false;
}

void Ground::repr(ostream &ostr) const
{
    ostr << "@";
}