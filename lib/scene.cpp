#include "scene.h"
#include "effolkronium/random.hpp"
#include <cstdint>
#include <cstdlib>
#include <loguru.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

using Random = effolkronium::random_static;

using namespace std;

uniform_int_distribution<int> dist_heightdiff(-2, 2);
uniform_int_distribution<int> dist_heightdiff_restrict(-1, 1);
// uniform_int_distribution<int> dist_nums(MIN_NUMBER, MAX_NUMBER);
// uniform_int_distribution<int> dist_colors((int)RED, (int)PURPLE);

ostream &operator<<(ostream &ostr, const GameObject *gameobject)
{
    gameobject->repr(ostr);
    return ostr;
}

Scene::Scene()
{
    _space = Objects(_width, vector<GameObject *>(_height));
    _dist_width = uniform_int_distribution<int>(0, _width - 1);
    _dist_height = uniform_int_distribution<int>(0, _height - 1);
    _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            update_array(i, j, '.', 'X');
        }
    }
}

Scene::Scene(int x, int y)
{
    _width = x;
    _height = y;
    _space = Objects(_width, vector<GameObject *>(_height));
    _dist_width = uniform_int_distribution<int>(0, _width - 1);
    _dist_height = uniform_int_distribution<int>(0, _height - 1);
    _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            update_array(i, j, '.', 'X');
        }
    }
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
    int thisheight = *lastheight + Random::get(dist_heightdiff);
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
        update_array(col, y, _space[col][y]->kind(), _space[col][y]->number());
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

void Scene::update_array(int x, int y, char colrval, char numrval)
{
    _data[x][y][0] = colrval;
    _data[x][y][1] = numrval;
}

void Scene::generate_easy()
{
    flush();
    srand(time(NULL));

    int maxheight = rand() % (_height * 3 / 4);
    int startcol = Random::get(_dist_width);
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
            block_col = Random::get(_dist_width);
            block_row = count_blocks(block_col);
            if (block_row < _height)
                break;
        }
        _space[block_col][block_row] = new Block(block_col, block_row);
        update_array(block_col, block_row, _space[block_col][block_row]->kind(), _space[block_col][block_row]->number());
    }

    // Put a player anywhere above a block.
    int player_col = Random::get(_dist_width);
    int player_height = count_blocks(player_col);
    _player = new Player(player_col, player_height);
    _space[player_col][player_height] = _player;
    update_array(player_col, player_height, _space[player_col][player_height]->kind(), _space[player_col][player_height]->number());
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
            update_array(i, j, '.', 'X');
        }
    }
}

GameObject *Scene::get_object(int x, int y)
{
    if (x < 0 || x > _width || y < 0 || y > _height)
        return nullptr;
    return _space[x][y];
}

int Scene::get_highest_obj_height(int col)
{
    for (int y = _height - 1; y >= 0; y--)
    {
        if (_space[col][y] != nullptr)
            return y;
    }
    return -1;
}

Player *Scene::get_player()
{
    return _player;
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
    if (dx > 1 || dx < -1 || dy > 1 || dx < -1)
    {
        throw invalid_argument("Only allowed to move +/- 1 block at a time.");
    }

    if (_space[x][y] == nullptr)
    {
        LOG_F(ERROR, "No object found at location.");
        return;
    }

    int newx = x + dx;
    int newy = y + dy;

    if (newx < 0 || newx > _width)
    {
        DLOG_F(1, "Attempting to move out of map.");
        return;
    }
    if (newy < 0)
    {
        DLOG_F(1, "Attempting to move below map.");
        return;
    }
    else if (newy > _height)
    {
        DLOG_F(1, "Attempting to move above map.");
        return;
    }

    _space[newx][newy] = _space[x][y];
    _space[newx][newy]->update(newx, newy);
    update_array(newx, newy, _space[newx][newy]->kind(), _space[newx][newy]->number());
    update_array(x, y, '.', 'X');
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
