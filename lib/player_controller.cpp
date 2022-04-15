#include "player_controller.h"
#include <loguru.hpp>
#include <stdexcept>

using namespace std;

PlayerController::PlayerController(Scene *scene, Player *player) : _scene(scene), _player(player)
{
}

int PlayerController::move(int direction)
{
    LOCATION current = _player->location();
    int hasblock = 0;
    if (direction != LEFT && direction != RIGHT)
    {
        DLOG_F(ERROR, "Trying to move in invalid direction: %d", direction);
        throw invalid_argument("Direction must be -1 (left) or 1 (right).");
    }
    DLOG_F(4, "Commanded to move %s", (direction == LEFT) ? "left" : "right");
    DLOG_F(4, "Currently facing: %d", _player->facing());

    // If we're not facing in the direction trying to move,
    // then just flip directions
    if (_player->facing() != direction)
    {
        DLOG_F(3, "Switching directions.");
        _player->face(direction);
        _scene->move(current.x, current.y, 0, 0);
    }

    // Otherwise, we're facing the right way, so try to move one block over
    // See if the spot at same height is occupied.
    GameObject *spot = _scene->get_object(current.x + direction, current.y);
    if (spot != nullptr)
    {
        DLOG_F(1, "Attempted to move to occupied location.");
        return -1;
    }

    // See if the spot one block down is occupied (the "ground")
    spot = _scene->get_object(current.x + direction, current.y - 1);
    if (spot != nullptr)
    {
        // If there is a ground, then we can move normally.
        // (move the block first if held)
        if (_player->held() != nullptr)
            _scene->move(current.x, current.y + 1, direction, 0);
            hasblock = 1;
        _scene->move(current.x, current.y, direction, 0);
        return hasblock;
    }
    else
    {
        // Otherwise, we need to see if we'll fall off a cliff.
        spot = _scene->get_object(current.x + direction, current.y - 2);
        if (spot != nullptr)
        {
            // If the ground below exists, we'll move and drop down a block
            // (move the block first if held)
            if (_player->held() != nullptr)
                _scene->move(current.x, current.y + 1, direction, -1);
                hasblock = 1;
            _scene->move(current.x, current.y, direction, -1);
            return hasblock;
        }
        else
        {
            // Otherwise, don't move anywhere or we'll die
            DLOG_F(1, "Attempted to jump off a cliff.");
            return -1;
        }
    }
}

int PlayerController::jump()
{
    LOCATION current = _player->location();
    int facing = _player->facing();
    int hasblock = 0;

    GameObject *spot = _scene->get_object(current.x + facing, current.y);

    // See if there's even an object to jump onto
    if (spot == nullptr)
    {
        DLOG_F(1, "Can't jump onto thin air.");
        return -1;
    }

    // There's a block, so see if there's more block we can't jump over
    spot = _scene->get_object(current.x + facing, current.y + 1);
    if (spot != nullptr)
    {
        DLOG_F(1, "Can't jump up more than one block.");
        return -1;
    }

    // We're all good to jump
    // (move the block first if held)
    if (_player->held() != nullptr)
        _scene->move(current.x, current.y + 1, facing, 1);
        hasblock = 1;
    _scene->move(current.x, current.y, facing, 1);
    return hasblock;
}

int PlayerController::pick_up()
{
    LOCATION current = _player->location();
    int facing = _player->facing();

    GameObject *spot = _scene->get_object(current.x + facing, current.y);

    // See if there's a block that we're facing
    if (spot == nullptr)
    {
        DLOG_F(1, "Can't pick up air.");
        return -1;
    }

    // If the block we're facing isn't movable, don't try to pick it up
    if (!spot->movable())
    {
        DLOG_F(1, "Block on isn't movable.");
        return -1;
    }

    if (facing == LEFT)
        _scene->move(spot, 1, 1);
    else
        _scene->move(spot, -1, 1);

    // Pick up the block
    _player->hold(spot);
    DLOG_F(1, "Picked up a block!");
    return 1;
}

int PlayerController::put_down()
{
    LOCATION current = _player->location();
    int facing = _player->facing();

    // Make sure we're not attempting to place off the map
    if (current.x == 0 && facing == -1 || current.x == _scene->width() -1 && facing == 1)
    {
        DLOG_F(1, "Can't place a block off the map.");
        return -1;
    }

    // Get the height of the highest object in the column we're facing
    int col_max = _scene->get_highest_obj_height(current.x + facing);

    // If the column is to the ceiling, we won't be able to place another block
    if (col_max == _scene->height() - 1)
    {
        DLOG_F(1, "Can't place block above ceiling.");
        return -1;
    }
    // If there are no blocks in the column, just put down on the bottom
    if (col_max == -1)
    {
        DLOG_F(1, "Placing block on the bottom.");
        _scene->move(current.x, current.y + 1, facing, -current.y - 1);
        _player->hold(nullptr);
        return 1;
    }

    // Make sure we're placing on a block level with us or below
    if (col_max - current.y > 0)
    {
        DLOG_F(1, "Can't place block more than one block above us.");
        return -1;
    }

    // Place the block above the object at the current max height
    DLOG_F(1, "Placing block on top of object at height %d", col_max);
    _scene->move(current.x, current.y + 1, facing, -current.y + col_max);
    _player->hold(nullptr);
    return 1;
}

bool PlayerController::holding()
{
    if (_player->held() == nullptr)
        return false;
    return true;
}
