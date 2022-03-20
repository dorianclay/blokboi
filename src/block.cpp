#include "block.h"
#include "effolkronium/random.hpp"
#include <cstdlib>
#include <ctime>

using Random = effolkronium::random_static;

using namespace std;

uniform_int_distribution<int> dist_colors((int)RED, (int)PURPLE);
uniform_int_distribution<int> dist_blocknum(MIN_NUMBER, MAX_NUMBER);

Block::Block()
{
    Block(0, 0);
}

Block::Block(int x, int y)
{
    _location.x = x;
    _location.y = y;
    _color = static_cast<Color>(Random::get(dist_colors));
    _number = Random::get(dist_blocknum);
    _movable = true;
    _kind = BLOCK;
}

Block::Block(int x, int y, Color color, int number, bool movable) : _color(color), _number(number), _movable(movable)
{
    _location.x = x;
    _location.y = y;
    _kind = BLOCK;
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
    _kind = GROUND;
}

void Ground::repr(ostream &ostr) const
{
    ostr << "@";
}