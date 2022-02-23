# include "block.h"
# include <cstdlib>

using namespace std;

Block::Block() 
{
    Block(Location(0, 0));
}

Block::Block(Location location)
    : _location(location)
{
    _color = static_cast<Color>(rand() % COUNT);
    srand(time(0));
    _number = 1 + (rand() % (MAX_NUMBER - 1));
    _movable = true;
}

Block::Block(int color, int number, bool movable, Location location)
    : _color(color), _number(number), _movable(movable), _location(Location)
{
}