# include "player.h"

Player::Player()
{
    Player(nullptr);
}

Player::Player(Location *location) : _location(location)
{
    _held = nullptr;
    _facing = RIGHT;
}

Player::~Player() 
{
    delete _location;
}

void Player::move(int x, int y)
{
    if (_facing == LEFT && x > 0)
        _facing = RIGHT;
    if (_facing == RIGHT && x < 0)
        _facing = LEFT;

    _location->move(x, y);
}

void Player::jump()
{
    _location->move(0, 1);
}