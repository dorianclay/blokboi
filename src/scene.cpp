#include "scene.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <loguru.hpp>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

typedef std::mt19937 RNG_ENGINE;
random_device randdev;
uint32_t seed_val = randdev();

RNG_ENGINE rng;

uniform_int_distribution<int> dist_heightdiff(-2, 2);
uniform_int_distribution<int> dist_heightdiff_restrict(-1, 1);
uniform_int_distribution<int> dist_nums(MIN_NUMBER, MAX_NUMBER);
uniform_int_distribution<int> dist_colors((int)RED, (int)PURPLE);

std::ostream &operator<<(std::ostream &ostr, const GameObject *gameobject)
{
    gameobject->repr(ostr);
    return ostr;
}

Scene::Scene()
{
    // rng.seed(seed_val);
    _space = Objects(_width, std::vector<GameObject *>(_height));
    _dist_width = uniform_int_distribution<int>(0, _width - 1);
    _dist_height = uniform_int_distribution<int>(0, _height - 1);
}

Scene::Scene(int x, int y)
{
    _width = x;
    _height = y;
    // rng.seed(seed_val);
    _space = Objects(_width, std::vector<GameObject *>(_height));
    _dist_width = uniform_int_distribution<int>(0, _width - 1);
    _dist_height = uniform_int_distribution<int>(0, _height - 1);
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

void Scene::fill_ground(int col, int *lastheight, int *priorheight, int *maxheight)
{
    // Get a new height that is the prev. height +/- [-2,2]
    int thisheight = *lastheight + dist_heightdiff(rng);
    // Make sure the height is no less than 1
    if (thisheight < 1)
        thisheight = 1;
    // Make sure the height is no more than our window
    else if (thisheight >= (_height * 4 / 5))
        thisheight = (_height * 4 / 5) - 1;

    // Fill up to this height with ground:
    for (int y = 0; y <= thisheight; y++)
    {
        // put a block here...
        _space[col][y] = new Ground(col, y);
    }

    *lastheight = thisheight;
}

int Scene::count_blocks(int col)
{
    // Find the height of the blocks at x=col
    int height = 0;
    for (int i = 0; i < _height - 1; i++)
    {
        if (_space[col][i] != nullptr)
        {
            height += 1;
        }
        else
        {
            break;
        }
    }
    return height;
}

void Scene::generate_easy()
{
    flush();
    srand(time(NULL));

    int maxheight = rand() % (_height * 3 / 4);
    int startcol = _dist_width(rng);
    int lastheight = maxheight;
    int priorheight = maxheight;
    DLOG_F(INFO, "Start column: %d", startcol);

    // Generate ground right
    for (int i = (int)startcol; i < _width; i++)
    {
        fill_ground(i, &lastheight, &priorheight, &maxheight);
    }

    lastheight = maxheight;
    priorheight = lastheight;

    // Generate ground  left
    for (int i = (int)startcol - 1; i >= 0; i--)
    {
        fill_ground(i, &lastheight, &priorheight, &maxheight);
    }

    // Count the 2-steps
    int twosteps = 0;
    int prior = count_blocks(0);
    for (int i = 0; i < _width; i++)
    {
        int current = count_blocks(i);
        if (abs(prior - current) == 2)
            twosteps++;
        prior = current;
    }
    twosteps--;
    DLOG_F(INFO, "Number of 'two-steps': %d", twosteps);

    // For now, put blocks anywhere randomly
    for (int i = 0; i < twosteps; i++)
    {
        int block_col, block_row;
        while (true)
        {
            block_col = _dist_width(rng);
            block_row = count_blocks(block_col);
            if (block_row < _height)
                break;
        }
        _space[block_col][block_row] = new Block(block_col, block_row);
    }

    // Put a player anywhere above a block.
    int player_col = _dist_width(rng);
    int player_height = count_blocks(player_col);
    _player = new Player(player_col, player_height);
    _space[player_col][player_height] = _player;
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

    for (int y = _height - 1; y >= 0; y--)
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
        if (y != 0)
            ss << endl;
    }
    return ss.str();
}