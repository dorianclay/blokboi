#include "block.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Block::Block()
{
    Block(nullptr);
}

Block::Block(Location *location) : _location(location)
{
    _color = static_cast<Color>(rand() % COUNT);
    srand(time(0));
    _number = 1 + (rand() % (MAX_NUMBER - 1));
    _movable = true;
}

Block::Block(Color color, int number, bool movable, Location *location)
    : _color(color), _number(number), _movable(movable), _location(location)
{
}

Block::~Block()
{
    delete _location;
}

Ground::Ground()
{
    Ground(new Location(0, 0));
}

Ground::Ground(Location *location)
{
    _location = location;
    _number = 0;
    _color = GREY;
    _movable = false;
}

Ground::~Ground()
{
    delete _location;
}