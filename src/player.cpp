#include "player.h"

Player::Player()
{
    Player(nullptr, 0, 0);
}

Player::Player(World *world)
{
    Player(world, 0, 0);
}

Player::Player(World *world, int x, int y)
{
    _world = world;
    _held = nullptr;
    _facing = RIGHT;
}

void Player::move(int x, int y)
{
    if (_facing == LEFT && x > 0)
        _facing = RIGHT;
    if (_facing == RIGHT && x < 0)
        _facing = LEFT;

    _world->move(this, _location[0], _location[1], x, y);
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

    _held = _world->get_object(_location[0] + _facing, _location[1]);
}