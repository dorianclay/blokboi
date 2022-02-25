#include "world.h"
#include <cassert>
#include <cstdio>
#include <vector>
#include "logger.cpp"

LOCATION *World::findObject(GameObject *object)
{
    for (int i = 0; i < _size[0]; i++)
    {
        for (int j = 0; j < _size[1]; j++)
        {
            if (_space[i][j] = object)
            {
                LOCATION *coords = new LOCATION;
                coords->x = i;
                coords->y = j;
                return coords;
            }
        }
    }
    return nullptr;
}

World::World()
{
    _space = Objects(_size[0], std::vector<GameObject *>(_size[1]));

    for (int i = 0; i < _size[0]; i++)
    {
        for (int j = 0; j < _size[1]; j++)
        {
            _space[i][j] = nullptr;
        }
    }
}

World::World(int x, int y)
{
    _size[0] = x;
    _size[1] = y;
    World();
}

void World::generate()
{
}

void World::refresh()
{
}

GameObject *World::get_object(int x, int y)
{
    return _space[x][y];
}

void World::move(GameObject *object, int dx, int dy)
{
    LOCATION *coords = findObject(object);

    if (coords == nullptr) {
        logerror("Object not found.");
        return;
    }

    move(coords->x, coords->y, dx, dy);
}

void World::move(int x, int y, int dx, int dy)
{
    if (_space[x][y] == nullptr) {
        logerror("No object found at location.");
        return;
    }
    
    int newx = x + dx;
    int newy = y + dy;

    if (newx < 0)
        newx = 0;
    else if (newx > _size[0])
        newx = _size[0];
    if (newy < 0)
        newy = 0;
    else if (newy > _size[1])
        newy = _size[1];

    _space[newx][newy] = _space[x][y];
    _space[x][y] = nullptr;

}