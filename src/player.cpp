#include "player.h"

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
}

void Player::move(int x, int y)
{
    if (_facing == LEFT && x > 0)
        _facing = RIGHT;
    if (_facing == RIGHT && x < 0)
        _facing = LEFT;

    // TODO: complete implementation
}

void Player::jump()
{
    move(0, 1);
}

void Player::pick_up()
{
    // If there's already a block held... (nullptr is true)
    if (_held)
        return;
}

void Player::repr(std::ostream &ostr) const
{
    ostr << "P";
}