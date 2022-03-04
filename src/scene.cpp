#include "scene.h"
#include <cassert>
#include <cstdio>
#include <loguru.hpp>
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

std::ostream &operator<<(std::ostream &ostr, const GameObject *gameobject)
{
    gameobject->repr(ostr);
    return ostr;
}

Scene::Scene()
{
    rng.seed(seed_val);
    _space = Objects(_width, std::vector<GameObject *>(_height));
    LOG_F(INFO, "Making a Scene now... flushing.");
}

Scene::Scene(int x, int y)
{
    _width = x;
    _height = y;
    rng.seed(seed_val);
    _space = Objects(_width, std::vector<GameObject *>(_height));
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

    // Generate ground right
    for (int i = (int)startcol; i < _width; i++)
    {
        // Get a new height that is the prev. height +/- [-2,2]
        int thisheight = lastheight + dist_heightdiff(rng);
        // Make sure the height is no less than 1
        if (thisheight < 1)
            thisheight = 1;
        // Make sure the height is no more than our window
        else if (thisheight >= _height)
            thisheight = _height - 1;


        // Fill up to this height with ground:
        for (int y = 0; y <= thisheight; y++) {
            // put a block here...
            _space[i][y] = new Ground(i, y);
        }

        lastheight = thisheight;
    }

    lastheight = maxheight;

    // Generate ground  left
    for (int i = (int)startcol - 1; i >= 0; i--)
    {
        // Get a new height that is the prev. height +/- [-2,2]
        int thisheight = lastheight + dist_heightdiff(rng);
        // Make sure the height is no less than 1
        if (thisheight < 1)
            thisheight = 1;
        // Make sure the height is no more than our window
        else if (thisheight >= _height)
            thisheight = _height - 1;

        // Fill up to this height with ground:
        for (int y = 0; y <= thisheight; y++) {
            // put a block here...
            _space[i][y] = new Ground(i, y);
        }

        lastheight = thisheight;
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
        LOG_F(ERROR, "Object not found.");
        return;
    }

    move(coords->x, coords->y, dx, dy);
}

void Scene::move(int x, int y, int dx, int dy)
{
    if (_space[x][y] == nullptr)
    {
        LOG_F(ERROR, "No object found at location.");
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

string Scene::representation()
{
    Objects::iterator iter_x;
    stringstream ss;
    ss.str("");

    for (int y = 0; y < _height; y++)
    {
        for (iter_x = _space.begin(); iter_x != _space.end(); iter_x++)
        {
            if (iter_x->at(y) == nullptr)
            {
                ss << ".";
            }
            else
            {
                ss << (iter_x->at(y));
            }
        }
        ss << endl;
    }
    return ss.str();
}