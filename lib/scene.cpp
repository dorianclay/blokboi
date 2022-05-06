#include "scene.h"
#include "effolkronium/random.hpp"
#include <cstdint>
#include <cstdlib>
#include <loguru.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <cmath>

using Random = effolkronium::random_static;

using namespace std;

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
  _init_data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  _relationship = "";
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
  _init_data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  _relationship = "";
}

Scene::Scene(Char3d pregen) {
  _width = pregen.size();
  _height = pregen[0].size();
  _space = Objects(_width, vector<GameObject *>(_height));
  _dist_width = uniform_int_distribution<int>(0, _width - 1);
  _dist_height = uniform_int_distribution<int>(0, _height - 1);
  _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  _relationship = "";
  generate_from_array(pregen);
}

Scene::Scene(Char3d pregen, std::string objective, std::string relationship, Int2d obj_coords, Int2d feature_mask) {
  _width = pregen.size();
  _height = pregen[0].size();
  _space = Objects(_width, vector<GameObject *>(_height));
  _dist_width = uniform_int_distribution<int>(0, _width - 1);
  _dist_height = uniform_int_distribution<int>(0, _height - 1);
  _data = Char3d(_width, vector<vector<char>>(_height, vector<char>(2)));
  _relationship = "";
  generate_from_saved(pregen, objective, relationship, obj_coords, feature_mask);
}

LOCATION *Scene::findObject(GameObject *object) {
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      if (_space[i][j] == object) {
        LOCATION *coords = new LOCATION;
        coords->x = i;
        coords->y = j;
        DLOG_F(5, "found on i=%d, j=%d", i, j);
        return coords;
      }
    }
  }
  return nullptr;
}

/**
 * @brief Set the valid finishing blocks.
 *
 */
void Scene::set_valid() {
  _valid.clear();
  // for each of the targets...
  for (int tgt = 0; tgt < _target_features.size(); tgt++) {
    Blocks objective;
    // check for every block
    for (int i = 0; i < _blocks.size(); i++) {
      Block *temp = _blocks[i];
      // if the block is the same color as the feature we want
      if (temp->color() == _target_features[tgt][0]) {
        objective.push_back(temp);
      // if the block has the same number as the feature we want
      } else if (temp->number() == _target_features[tgt][1]) {
        objective.push_back(temp);
      }
    }
    _valid.push_back(objective);
  }
}


/**
 * @brief Set the objective string.
 *
 */
void Scene::set_string() {
  stringstream ss;
  string demo = "Put the {color0} block {number0} {relation} to the {color1} block {number1}.";
  ss.str("");

  // verb
  if (_relationship == "off") {
    ss << "Take";
  } else {
    ss << "Put";
  }
  // objective 1's features
  ss << " the ";
  if (_target_features[0][0] != -1) {
    ss << colorname(_target_features[0][0]) << " ";
  }
  if (_target_features[0][1] != -1) {
    ss << _target_features[0][1] - '0' << " ";
  }
  ss << "block ";

  // relationship
  if (_relationship == "right" || _relationship == "left") {
    ss << "to the " << _relationship << " of ";
  } else if (_relationship == "side") {
    if (Random::get<bool>())
      ss << "next to ";
    else
      ss << "to the side of ";
  } else if (_relationship == "diagonal") {
    ss << _relationship << " to ";
  } else if (_relationship == "on top") {
    ss << _relationship << " of ";
  } else {
    ss << _relationship << " ";
  }

  ss << "the ";
  if (_target_features[1][0] != -1) {
    ss << colorname(_target_features[1][0]) << " ";
  }
  if (_target_features[1][1] != -1) {
    ss << _target_features[1][1] - '0' << " ";
  }
  ss << "block.";

  _objective = ss.str();
}



/*
 *
 * PUBLIC
 *
 */

void Scene::refresh() {
  generate_from_saved(_init_data, objective(), relationship(), init_coords(), feature_mask());
}

void Scene::flush() {
  // Clean out the _space and _data arrays
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      delete _space[i][j];
      _space[i][j] = nullptr;
      update_array(i, j, '.', 'X');
    }
  }
  // Clean out _blocks, _player, _valid, _success, _realtionship
  _blocks.clear();
  _init_data.clear();
  _targets.clear();
  _target_features.clear();
  _valid.clear();
  _success = false;
  _relationship = "";
  _objective = "";
  _init_data.clear();
  _init_obj_coords.clear();
}

/**
 * @brief Verify if the goal has been achieved.
 *
 * @return true if {_targets[0]} {_relationship} {_targets[1]}
 * @return false if not achieved.
 */
bool Scene::verify() {
  bool success = false;
  // Check the first objective's valid options
  for (int i=0; i<_valid[0].size(); i++) {
    // Against every second objective's valid options
    for (int j=0; j<_valid[1].size(); j++) {
      // Let 'a' : target[0]
      // Let 'b' : target[1]
      int ax = _valid[0][i]->location().x;
      int ay = _valid[0][i]->location().y;
      int bx = _valid[1][j]->location().x;
      int by = _valid[1][j]->location().y;

      if (_relationship == "above" || _relationship == "on top") {
        // _target[0] on top of _target[1]
        if (ax == bx) {
          if (ay == by + 1){
            success = true;
            break;
          }
        }
      } else if (_relationship == "below" || _relationship == "under" || _relationship == "beneath") {
        // _target[0] under _target[1]
        if (ax == bx) {
          if (ay == by - 1) {
            success = true;
            break;
          }
        }
      } else if (_relationship == "right") {
        // target[0] directly to the right of target[1]
        if (ay == by) {
          if (ax == bx + 1) {
            success = true;
            break;
          }
        }
      } else if (_relationship == "left") {
        // target[0] directly to the left of target[1]
        if (ay == by) {
          if (ax == bx - 1) {
            success = true;
            break;
          }
        }
      } else if (_relationship == "side") {
        // target[0] directly beside target[1]
        if (ay == by) {
          if (ax == bx + 1) {
            success = true;
            break;
          }
          if (ax == bx - 1) {
            success = true;
            break;
          }
        }
      } else if (_relationship == "off") {
        // target[0] not above target[1]
        if (ax != bx) {
          success = true;
          break;
        }
        if (ax == bx) {
          if (ay < by) {
            success = true;
            break;
          }
        }
      } else if (_relationship == "diagonal") {
        // target[0] at an adjacent diagonal to target[1]
        if (ax == bx - 1 || ax == bx + 1) {
          if (ay == by + 1) {
            success = true;
            break;
          }
          if (ay == by - 1) {
            success = true;
            break;
          }
        }
      } else {
        LOG_F(ERROR, "Handling target relationship '%s' is undefined.", _relationship.c_str());
        throw invalid_argument("I don't know how to handle the target relationship.");
      }
    }
  }

  // If we matched one of the handled relationships but weren't true,
  // then the goal state has not been achieved.
  _success = success;
  if (success) {
    LOG_F(WARNING, "Success! Completed objective.");
  }
  return success;
}

void Scene::targets(Int2d coords, Int2d feature_mask) {
  if (coords.size() != 2 || coords[0].size() != 2 || feature_mask.size() != 2 || feature_mask[0].size() != 2) {
    throw invalid_argument("Must be a 2-D array of size 2x2.");
  }
  int x1, y1;
  x1 = coords[0][0];
  y1 = coords[0][1];
  if (x1 < 0 || x1 >= _width || y1 < 0 || y1 >= _height) {
    throw invalid_argument("Cannot set target to a block outside scene boundary.");
    return;
  }
  int x2, y2;
  x2 = coords[1][0];
  y2 = coords[1][1];
  if (x2 < 0 || x2 >= _width || y2 < 0 || y2 >= _height) {
    throw invalid_argument("Cannot set target to a block outside scene boundary.");
    return;
  }
  _targets.clear();
  _valid.clear();
  _target_features.clear();
  _targets.push_back(dynamic_cast<Block*>(get_object(x1, y1)));
  _init_obj_coords.push_back({x1, y1});
  _targets.push_back(dynamic_cast<Block*>(get_object(x2, y2)));
  _init_obj_coords.push_back({x2, y2});
  for (int i=0; i < 2; i++ ) {
    vector<int> temp;
    if (feature_mask[i][0] == 1)
      temp.push_back(_targets[i]->color());
    else
      temp.push_back(-1);

    if (feature_mask[i][1] == 1)
      temp.push_back(_targets[i]->number());
    else
      temp.push_back(-1);

    _target_features.push_back(temp);
  }
  set_valid();
  set_string();

  DLOG_F(INFO, "Successfully set targets manually. T1: (%d, %d)\tT2: (%d, %d)", x1, y1, x2, y2);
}

GameObject *Scene::get_object(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height)
    return nullptr;
  return _space[x][y];
}

int Scene::get_highest_obj_height(int col) const {
  for (int y = _height - 1; y >= 0; y--) {
    if (_space[col][y] != nullptr)
      return y;
  }
  return -1;
}

int Scene::get_highest_block_height(int col) const {
  for (int y = _height - 1; y >= 0; y--) {
    if (_space[col][y] != nullptr && _space[col][y] != _player)
      return y;
    else if (_space[col][y] != nullptr && _space[col][y] == _player)
      return y - 1;
  }
  return -1;
}

int Scene::get_lowest_obj_height(int col) const {
  for (int y = 0; y < _height; y++) {
    if (_space[col][y] != nullptr)
      return y;
  }
  return -1;
}

/**
 * @brief Get the column of the furthest block that can be picked up.
 *
 * @param direction The side to search (1 = right, -1 = left)
 * @return int: The column of the furthest block available.
 */
int Scene::furthest_block_available(int direction) const {
  if (direction != 1 && direction != -1) {
    throw invalid_argument("Direction must be +/- 1.");
    return -1;
  }
  int i = _player->location().x;
  int available = -1;
  Block* blockavail = nullptr;
  int lastheight = _player->location().y - 1;

  // While we're in map bounds...
  while (i > 0 && i < _width-1) {
    int thiscol = i + direction;
    int thisheight = get_highest_obj_height(thiscol);
    // If this block is more than one block below the last, we just went off a cliff
    if (abs(lastheight - thisheight) > 1) {
      if (available == -1 && blockavail != nullptr)
        return blockavail->location().x;
      return available;
    }
    // If this block is one higher than the last block...
    if (thisheight - lastheight == 1) {
      // Check if it's a playable block
      GameObject *temp = get_object(thiscol, thisheight);
      if (temp->isBlock()) {
        // See if this block is one of the targets
        if (temp == _targets[0] || temp == _targets[1]) {
          blockavail = dynamic_cast<Block*>(temp);
        } else {
          available = thiscol;
        }
      }
    // If this block is one lower than the last block...
    } else if (lastheight - thisheight == 1) {
      // Check if the prior block was a playable block
      GameObject *temp = get_object(thiscol - direction, lastheight);
      if (temp->isBlock()) {
        // And pick it up only if we won't make a wall
        if (get_highest_block_height(thiscol - 2*direction) - temp->location().y != 1) {
          // See if this block is one of the targets
          if (temp == _targets[0] || temp == _targets[1]) {
            blockavail = dynamic_cast<Block*>(temp);
          } else {
            available = thiscol - direction;
          }
        }
      }
    }
    lastheight = thisheight;
    i = thiscol;
  }
  if (available == -1 && blockavail != nullptr)
    return blockavail->location().x;
  return available;
}

const Block *Scene::targets(int blocknum) const {
  return _targets.at(blocknum);
}

const Int2d Scene::feature_mask() const {
  Int2d feature_mask;
  for (int i=0; i< _target_features.size(); i++) {
    vector<int> temp;
    for (int j=0; j < _target_features[0].size(); j++) {
      if (_target_features[i][j] == -1)
        temp.push_back(0);
      else
        temp.push_back(1);
    }
    feature_mask.push_back(temp);
  }
  return feature_mask;
}

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
