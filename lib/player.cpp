#include "player.h"
#include <cassert>

Player::Player()
{
    Player(0, 0);
}

Player::Player(int x, int y)
{
    _location.x = x;
    _location.y = y;
    _held = nullptr;
    _facing = RIGHT;
    _kind = PLAYER;
}

void Player::face(int direction)
{
    assert(direction == LEFT || direction == RIGHT);

    _facing = (Direction)direction;
}

void Player::hold(GameObject *block)
{
    assert(block->movable());

    _held = block;
}

void Player::repr(std::ostream &ostr) const
{
    ostr << "P";
}
