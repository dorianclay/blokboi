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

ostream &operator<<(ostream &ostr, const GameObject *gameobject) {
  gameobject->repr(ostr);
  return ostr;
}

Scene::Scene() {
  _space = Objects(_width, vector<GameObject *>(_height));
  _dist_width = uniform_int_distribution<int>(0, _width - 1);
  _dist_height = uniform_int_distribution<int>(0, _height - 1);
  _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      update_array(i, j, '.', 'X');
    }
  }
  _init = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
}

Scene::Scene(int x, int y) {
  _width = x;
  _height = y;
  _space = Objects(_width, vector<GameObject *>(_height));
  _dist_width = uniform_int_distribution<int>(0, _width - 1);
  _dist_height = uniform_int_distribution<int>(0, _height - 1);
  _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      update_array(i, j, '.', 'X');
    }
  }
  _init = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
}

Scene::Scene(Char3d pregen) {
  _width = pregen.size();
  _height = pregen[0].size();
  _space = Objects(_width, vector<GameObject *>(_height));
  _dist_width = uniform_int_distribution<int>(0, _width - 1);
  _dist_height = uniform_int_distribution<int>(0, _height - 1);
  _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  generate_from_array(pregen);
}

LOCATION *Scene::findObject(GameObject *object) {
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      if (_space[i][j] == object) {
        LOCATION *coords = new LOCATION;
        coords->x = i;
        coords->y = j;
        DLOG_F(4, "found on i=%d, j=%d", i, j);
        return coords;
      }
    }
  }
  return nullptr;
}

void Scene::fill_ground(int col, int *lastheight, int *priorheight,
                        int *maxheight) {
  // Get a new height that is the prev. height +/- [-2,2]
  int thisheight = *lastheight + Random::get(dist_heightdiff);
  // Make sure the height is no less than 1
  if (thisheight < 1)
    thisheight = 1;
  // Make sure the height is no more than our window
  else if (thisheight >= (_height * 4 / 5))
    thisheight = (_height * 4 / 5) - 1;

  // Fill up to this height with ground:
  for (int y = 0; y <= thisheight; y++) {
    // put a block here...
    _space[col][y] = new Ground(col, y);
    update_array(col, y, _space[col][y]->kind(), _space[col][y]->number());
  }

  *lastheight = thisheight;
}

int Scene::count_blocks(int col) {
  // Find the height of the blocks at x=col
  int height = 0;
  for (int i = 0; i < _height - 1; i++) {
    if (_space[col][i] != nullptr) {
      height += 1;
    } else {
      break;
    }
  }
  return height;
}

void Scene::update_array(int x, int y, char colrval, char numrval) {
  _data[x][y][0] = colrval;
  _data[x][y][1] = numrval;
}

int Scene::make_plains(int xstart, int base, int n)
{
  // __________
  // Make a flat row of ground
  for (int i = xstart; i < xstart + n - 1; i++) {
    _space[i][base] = new Ground(i, base);
    update_array(i, base, _space[i][base]->kind(), _space[i][base]->number());
  }
  return base;
}

int Scene::make_steppes(int xstart, int base, int n)
{
  //       __
  //    __|
  // __|
  // Make "steps" that increase in height linearly with width
  int step = 0;
  for (int i = xstart; i < xstart + n - 1; i++) {
    for (int j = base; j <= base + step; j++) {
      _space[i][j] = new Ground(i, j);
      update_array(i, j, _space[i][j]->kind(), _space[i][j]->number());
    }
    // Only increment the step height if we won't go outside the map
    if (step < _height - 1)
      step++;
  }
  return base + step;
}

int Scene::make_plateau(int xstart, int base, int n, int h)
{
  //       ____________
  //    __|
  // __|
  // Make a couple steps then a flat area
  int step = 0;
  for (int i = xstart; i < xstart + n - 1; i++) {
    for (int j = base; j <= base + step; j++) {
      _space[i][j] = new Ground(i, j);
      update_array(i, j, _space[i][j]->kind(), _space[i][j]->number());
    }
    // Increase the step height if we're less than the plateau or the map height
    if (step < h && step < _height-1)
      step++;
  }
  return base + step;
}

int Scene::make_canyon(int xstart, int base, int n)
{
  // __        __
  //   |      |
  //   |______|
  // Make a 2-block deep canyon

  // Set the canyon bottom height to base-2 or 1, whichever is higher
  int height = base;
  if (base - 2 < 0)
    height = 1;
  else
    height = base - 2;

  // Make the first wall
  _space[xstart][base] = new Ground(xstart, base);
  update_array(xstart, base, _space[xstart][base]->kind(), _space[xstart][base]->number());
  // Make the canyon bottom
  for (int i = xstart+1; i < xstart + n - 2; i++) {
    _space[i][height] = new Ground(i, height);
    update_array(i, height, _space[i][height]->kind(), _space[i][height]->number());
  }
  // Make the second wall
  _space[xstart+n-1][base] = new Ground(xstart+n-1, base);
  update_array(xstart+n-1, base, _space[xstart+n-1][base]->kind(), _space[xstart+n-1][base]->number());
  return base;
}

int Scene::make_cave(int base, int side)
{
}

int Scene::make_spire(int xstart, int base, int n)
{
  //      __
  //     |  |
  // ____|  |____
  // Make a 2-block tall "spire"
  // First, create a flat row of ground
  for (int i = xstart; i < xstart + n - 1; i++) {
    _space[i][base] = new Ground(i, base);
    update_array(i, base, _space[i][base]->kind(), _space[i][base]->number());
  }
  // Randomly pick a column to put the spire in
  int col = Random::get<int>(xstart+1, xstart+n-2);
  // Place the spire, checking for map boundaries
  if (base + 1 < _width - 1) {
<<<<<<< HEAD
    _space[col][base+1] = new Ground(i, base+1);
    update_array(i, base+1, _space[i][base+1]->kind(), _space[i][base+1]->number());
    if (base + 2 < _width - 1) {
      _space[col][base+2] = new Ground(i, base+2);
      update_array(i, base+2, _space[i][base+2]->kind(), _space[i][base+2]->number());
=======
    _space[col][base+1] = new Ground(col, base+1);
    update_array(col, base+1, _space[col][base+1]->kind(), _space[col][base+1]->number());
    if (base + 2 < _width - 1) {
      _space[col][base+2] = new Ground(col, base+2);
      update_array(col, base+2, _space[col][base+2]->kind(), _space[col][base+2]->number());
>>>>>>> Create chunk generators
    }
  }
  return base;
}


void Scene::generate_modular() {
  flush();

  _init = _data;
}

void Scene::generate_easy() {
  flush();
  srand(time(NULL));

  int maxheight = rand() % (_height * 3 / 4);
  int startcol = Random::get(_dist_width);
  int lastheight = maxheight;
  int priorheight = maxheight;
  DLOG_F(INFO, "Start column: %d", startcol);

  // Generate ground right
  for (int i = (int)startcol; i < _width; i++) {
    fill_ground(i, &lastheight, &priorheight, &maxheight);
  }

  lastheight = maxheight;
  priorheight = lastheight;

  // Generate ground  left
  for (int i = (int)startcol - 1; i >= 0; i--) {
    fill_ground(i, &lastheight, &priorheight, &maxheight);
  }

  // Count the 2-steps
  int twosteps = 0;
  int prior = count_blocks(0);
  for (int i = 0; i < _width; i++) {
    int current = count_blocks(i);
    if (abs(prior - current) == 2)
      twosteps++;
    prior = current;
  }
  twosteps--;
  DLOG_F(INFO, "Number of 'two-steps': %d", twosteps);

  // For now, put blocks anywhere randomly
  for (int i = 0; i < twosteps; i++) {
    int block_col, block_row;
    while (true) {
      block_col = Random::get(_dist_width);
      block_row = count_blocks(block_col);
      if (block_row < _height)
        break;
    }
    _space[block_col][block_row] = new Block(block_col, block_row);
    update_array(block_col, block_row, _space[block_col][block_row]->kind(),
                 _space[block_col][block_row]->number());
  }

  // Put a player anywhere above a block.
  int player_col = Random::get(_dist_width);
  int player_height = count_blocks(player_col);
  _player = new Player(player_col, player_height);
  _space[player_col][player_height] = _player;
  update_array(player_col, player_height,
               _space[player_col][player_height]->kind(),
               _space[player_col][player_height]->number());

  _init = _data;
}

void Scene::generate(const string &str) {
  flush();
  int x = 0;
  int y = 0;

  stringstream ss;
  ss.str(str);

  // TODO: parse the stream and generate objects.

  _init = _data;
}

void Scene::generate_from_array(Char3d pregen) {
  flush();
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      char kind = pregen[i][j][0];
      char ident = pregen[i][j][1];
      _data[i][j][0] = kind;
      _data[i][j][1] = ident;
      // If this is a sky:
      if (kind == '.')
        continue;
      // If this is a ground:
      else if (kind == '@') {
        _space[i][j] = new Ground(i, j);
      }
      // If this is a player:
      else if (kind == 'P') {
        _player = new Player(i, j, ident);
        _space[i][j] = _player;
      }
      // If this is a block:
      else {
        _space[i][j] = new Block(i, j, kind, ident - '0', true);
      }
    }
  }

  _init = _data;
}

void Scene::refresh() { generate_from_array(_init); }

void Scene::flush() {
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      _space[i][j] = nullptr;
      update_array(i, j, '.', 'X');
    }
  }
}

GameObject *Scene::get_object(int x, int y) {
  if (x < 0 || x >= _width || y < 0 || y >= _height)
    return nullptr;
  return _space[x][y];
}

int Scene::get_highest_obj_height(int col) {
  for (int y = _height - 1; y >= 0; y--) {
    if (_space[col][y] != nullptr)
      return y;
  }
  return -1;
}

Player *Scene::get_player() { return _player; }

void Scene::move(GameObject *object, int dx, int dy) {
  LOCATION *coords = findObject(object);

  if (coords == nullptr) {
    LOG_F(ERROR, "Object not found.");
    return;
  }
  DLOG_F(3, "Moving an object at (%d, %d)", coords->x, coords->y);
  move(coords->x, coords->y, dx, dy);
}

void Scene::move(int x, int y, int dx, int dy) {
  if (dx > 1 || dx < -1 || dy > 1 || dx < -1) {
    throw invalid_argument("Only allowed to move +/- 1 block at a time.");
  }

  if (_space[x][y] == nullptr) {
    LOG_F(ERROR, "No object found at location.");
    return;
  }

  int newx = x + dx;
  int newy = y + dy;

  if (newx < 0 || newx > _width) {
    DLOG_F(1, "Attempting to move out of map.");
    return;
  }
  if (newy < 0) {
    DLOG_F(1, "Attempting to move below map.");
    return;
  } else if (newy > _height) {
    DLOG_F(1, "Attempting to move above map.");
    return;
  }

  if (dx == 0 && dy == 0)
    update_array(x, y, _space[x][y]->kind(), _space[x][y]->number());
  else {
    _space[newx][newy] = _space[x][y];
    _space[newx][newy]->update(newx, newy);
    update_array(newx, newy, _space[newx][newy]->kind(),
                 _space[newx][newy]->number());
    update_array(x, y, '.', 'X');
    _space[x][y] = nullptr;
  }
}

string Scene::representation() {
  Objects::iterator iter_x;
  stringstream ss;
  ss.str("");

  for (int y = _height - 1; y >= 0; y--) {
    for (iter_x = _space.begin(); iter_x != _space.end(); iter_x++) {
      if (iter_x->at(y) == nullptr) {
        ss << ".";
      } else {
        ss << (iter_x->at(y));
      }
    }
    if (y != 0)
      ss << endl;
  }
  return ss.str();
}
