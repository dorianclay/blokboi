#include "scene.h"
#include "logger.cpp"
#include <cassert>
#include <cstdio>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

typedef std::mt19937 RNG_ENGINE;
uint32_t seed_val = 0;

RNG_ENGINE rng;

uniform_int_distribution<int> dist_heightdiff(-2, 2);
uniform_int_distribution<int> dist_nums(MIN_NUMBER, MAX_NUMBER);
uniform_int_distribution<int> dist_colors((int)RED, (int)PURPLE);

Scene::Scene()
{
    rng.seed(seed_val);
    _space = Objects(_width, std::vector<GameObject *>(_height));
    flush();
}

Scene::Scene(int x, int y)
{
    _width = x;
    _height = y;
    Scene();
}

LOCATION *Scene::findObject(GameObject *object)
{
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
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

void Scene::generate()
{
    flush();
    srand(time(0));

    unsigned maxheight = rand() % (_height * 2 / 3);
    unsigned startcol = rand() % _width;
    unsigned lastheight = maxheight;

    // Generate blocks right
    for (int i = (int)startcol; i < _width; i++)
    {
        int thisheight = lastheight + dist_heightdiff(rng);
        if (thisheight < 1)
            thisheight = 1;
        else if (thisheight >= _height)
            thisheight = _height - 1;

        // put a block here...
        _space[i][thisheight] = new Block(i, thisheight, (Color)dist_colors(rng), dist_nums(rng), true);

        lastheight = thisheight;
    }

    lastheight = maxheight;
    // Generate blocks left

    for (int i = 0; i < _width; i++)
    {
    }
}

void Scene::generate(const string &str)
{
    flush();
    int x = 0;
    int y = 0;

    stringstream ss;
    ss.str(str);

    // TODO: parse the stream and generate objects.
}

void Scene::refresh()
{
}

void Scene::flush()
{
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            _space[i][j] = nullptr;
        }
    }
}

GameObject *Scene::get_object(int x, int y)
{
    return _space[x][y];
}

void Scene::move(GameObject *object, int dx, int dy)
{
    LOCATION *coords = findObject(object);

    if (coords == nullptr)
    {
        logerror("Object not found.");
        return;
    }

    move(coords->x, coords->y, dx, dy);
}

void Scene::move(int x, int y, int dx, int dy)
{
    if (_space[x][y] == nullptr)
    {
        logerror("No object found at location.");
        return;
    }

    int newx = x + dx;
    int newy = y + dy;

    if (newx < 0)
        newx = 0;
    else if (newx > _width)
        newx = _width;
    if (newy < 0)
        newy = 0;
    else if (newy > _height)
        newy = _height;

    _space[newx][newy] = _space[x][y];
    _space[newx][newy]->update(newx, newy);
    _space[x][y] = nullptr;
}