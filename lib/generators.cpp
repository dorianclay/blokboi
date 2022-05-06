#include "scene.h"
#include "effolkronium/random.hpp"
#include <loguru.hpp>
#include <stdexcept>

using Random = effolkronium::random_static;

using namespace std;

#define GENERATOR_ATTEMPTS 1000
#define GEN_WARN_V 2

uniform_int_distribution<int> dist_heightdiff(-2, 2);
uniform_int_distribution<int> dist_heightdiff_restrict(-1, 1);

/**
 * @brief Generate a scene using the default generator.
 *
 */
void Scene::generate() {
  generate_heuristical();
}

/**
 * @brief Place blocks between the player and the closest of two columns.
 *
 * @param player_col The column of the player.
 * @param walk_col The 'walker' column.
 * @param stay_col The 'stayer' column.
 */
void Scene::place_walker_blocks(int player_col, int walk_col, int stay_col) {
  int block_col, block_row;
  for (int i=0; i<_width; i++) {
    // If walker is closer to the player...
    if (abs(player_col - walk_col) < abs(player_col - stay_col)){
      if (walk_col - player_col > 0) {
        // If the walker is right of the player...
        block_col = Random::get(player_col + 1, walk_col - 1);
      } else {
        // If the walker is left of the player...
        block_col = Random::get(player_col - 1, walk_col + 1);
      }
    // Otherwise the stayer is closer to the player...
    } else {
      if (stay_col - player_col > 0) {
        // If the stayer is right of the player...
        block_col = Random::get(player_col + 1, stay_col - 1);
      } else {
        // If the stayer is left of the player...
        block_col = Random::get(player_col - 1, stay_col + 1);
      }
    }
    if (block_col < 0)
      block_col = 0;
    else if (block_col >= _width)
      block_col = _width-1;
    block_row = count_blocks(block_col);
    // Keep randomly picking a column until the block is under the ceiling
    if (block_row < _height-1)
      break;
  }
  if (block_row >= _height - 1)
    return;
  _blocks.push_back(new Block(block_col, block_row));
  _space[block_col][block_row] = _blocks.back();
  update_array(block_col, block_row);
}

/**
 * @brief Generate a scene heuristics to increase playability.
 *
 */
void Scene::generate_heuristical() {
  srand(time(NULL));

  int attempts = 0;
  // Keep generating until we've made a satisfactory map
  while (attempts < GENERATOR_ATTEMPTS) {
    flush();
    attempts++;
    LOG_F(INFO, "Attempt %d to generate a scene heuristically...", attempts);

    fill_ground();

    // Pick a column for the player to go
    int player_col = Random::get(_dist_width);

    // Scan for edges to the right
    // Find an edge (a block where the next col's block >= 2 below)
    int blockneed = 0;
    int stay_col = 0, stay_height = 0, walk_col = 0, walk_height = 0;
    while (stay_col < _width - 1) {
      blockneed = 0;
      stay_height = count_blocks(stay_col);
      walk_col = stay_col + 1;
      while (walk_col < _width) {
        walk_height = count_blocks(walk_col);
        if (stay_height - walk_height <= walk_col - stay_col) {
          walk_col++;
          break;
        }
        blockneed += (stay_height - (walk_col - stay_col)) - count_blocks(walk_col);
        walk_col++;
      }
      // place blocks between the closer point: stay or walked
      while (blockneed > 0) {
        place_walker_blocks(player_col, walk_col, stay_col);
        blockneed--;
      }

      stay_col = walk_col;
    }

    // Scan for edges to the left
    stay_col = _width - 1;
    walk_col = stay_col - 1;
    while (stay_col > 1) {
      blockneed = 0;
      stay_height = count_blocks(stay_col);
      walk_col = stay_col - 1;
      while (walk_col > 0) {
        walk_height = count_blocks(walk_col);
        if (stay_height - walk_height <= stay_col - walk_col) {
          walk_col--;
          break;
        }
        blockneed += (stay_height - (stay_col - walk_col)) - count_blocks(walk_col);
        walk_col--;
      }
      // place blocks between the closer point: stay or walked
      while (blockneed > 0) {
        place_walker_blocks(player_col, walk_col, stay_col);
        blockneed--;
      }

      stay_col = walk_col;
    }

    // Place a random number of random blocks (up to 1/3 the map)
    int rand_blocks = Random::get(0, (int) _width/3);
    for (int i=0; i< rand_blocks; i++) {
      int block_col, block_row;
      block_col = Random::get(_dist_width);
      block_row = count_blocks(block_col);
      // Keep getting a random column until we have one that is under the ceiling
      if (block_row >= _height)
        continue;
      _blocks.push_back(new Block(block_col, block_row));
      _space[block_col][block_row] = _blocks.back();
      update_array(block_col, block_row);
    }

    // Place the player
    int player_height = count_blocks(player_col);
    int buffer = 0;
    // See if the player is surrounded
    if (player_col > 0 && count_blocks(player_col - 1) - player_height > 1) {
      buffer = count_blocks(player_col - 1) - player_height;
      player_height = count_blocks(player_col - 1);
    }
    if (player_col < _width - 1 && count_blocks(player_col + 1) - player_height > 1) {
      buffer = count_blocks(player_col + 1) - player_height;
      player_height = count_blocks(player_col + 1);
    }

    // Fill in the area now below player (if any)
    for (int i = 0; i < buffer; i++) {
      int buffheight = player_height - i - 1;
      _blocks.push_back(new Block(player_col, buffheight));
      _space[player_col][buffheight] = _blocks.back();
      update_array(player_col, buffheight);
    }

    // Place the player
    _player = new Player(player_col, player_height);
    _space[player_col][player_height] = _player;
    update_array(player_col, player_height);

    // Make sure we have at least 2 blocks. If not, generate fresh
    if (_blocks.size() < 2) {
      continue;
    }
    // Pick objective (target) blocks
    int target1 = Random::get(0, (int) _blocks.size() - 1);
    int target2;
    for (int i=0; i<_blocks.size(); i++) {
      target2 = Random::get(0, (int) _blocks.size() - 1);
      // Ensure the target blocks are unique
      if (target1 != target2)
        break;
    }
    // If, for some reason, the target blocks are identitcal, generate fresh
    if (target1 == target2) {
      continue;
    }

    _targets.push_back(_blocks.at(target1));
    _targets.push_back(_blocks.at(target2));
    _relationship = relations.at(Random::get(0, (int) relations.size() - 1));

    // For each of the two targets...
    for (int i=0; i < 2; i++) {
      // Save their starting location
      _init_obj_coords.push_back({_targets[i]->location().x, _targets[i]->location().y});
      vector<int> temp;
      // Randomly decide with 50/50 prob whether to select the color and/or number as the relevant feature
      // Use color
      if (Random::get<bool>()) {
        temp.push_back(_targets[i]->color());
      } else {
        temp.push_back(-1);
      }
      if (Random::get<bool>()) {
        temp.push_back(_targets[i]->number());
      } else {
        temp.push_back(-1);
      }

      // If we didn't get either feature, force a decision between one
      if (temp[0] == -1 && temp[1] == -1) {
        if (Random::get<bool>())
          temp[0] = _targets[i]->color();
        else
          temp[1] = _targets[i]->number();
      }

      _target_features.push_back(temp);
    }


    set_valid();
    set_string();


    if (check_scene()) {
      LOG_F(INFO, "Generated a scene after %d tries.", attempts);
      break;
    }
  }
  if (attempts == GENERATOR_ATTEMPTS) {
    throw runtime_error("Could not generate a good scene in " + to_string(attempts) + " tries.");
  }

  _init_data = _data;
}

/**
 * @brief Generate a scene randomly.
 *
 */
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

  _init_data = _data;
}

/**
 * @brief Generate a scene from an array representation.
 *
 * @param pregen The Char3d-defined array holding the scene representation.
 */
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
        _blocks.push_back(new Block(i, j, kind, ident - '0', true));
        _space[i][j] = _blocks.back();
      }
    }
  }

  _init_data = _data;
}

/**
 * @brief Re-generate a saved scene.
 *
 * @param pregen The Char3d array representing a scene.
 * @param objective The objective string.
 * @param relationship The objective block a:b relationship.
 * @param obj_coords The coordinates of the initial two target blocks.
 * @param feature_mask The relevant features from the target blocks.
 */
void Scene::generate_from_saved(Char3d pregen, string objective, string relationship, Int2d obj_coords, Int2d feature_mask) {
  generate_from_array(pregen);
  targets(obj_coords, feature_mask);
  Scene::objective(objective);
  relate(relationship);
}

void Scene::generate_modular() {
  flush();

  vector<int> direction = {-1, 1};

  // Randomly pick the number of modules to use,
  // where each module is at least 3 wide
  int nummods = Random::get(1, (int) _width / 3);
  int blockneed = 0;

  // Randomly pick the module parameters;
  int remaining = _width;
  int col = 0;
  int last_height = Random::get(1, (int) _height * 3 / 4);
  for (int i=0; i < nummods; i++) {
    int maker_idx = Random::get(0, 4); // module
    int n;
    if (nummods - i == 1) {
      n = remaining;
    } else {
      n = Random::get(3, remaining - 3 * (nummods - i - 1)); // n
    }
    int m = Random::get(1, n); // m
    Random::shuffle(direction);
    int dir = direction[0]; // dir

    // Fill the module
    // last_height = _maker_funcs[maker_idx](col, last_height, n, m, dir);
    if (maker_idx == 0)
      last_height = make_plains(col, last_height, n, m, dir);
    else if (maker_idx == 1)
      last_height = make_steppes(col, last_height, n, m, dir);
    else if (maker_idx == 2)
      last_height = make_plateau(col, last_height, n, m, dir);
    else if (maker_idx == 3) {
      last_height = make_canyon(col, last_height, n, m, dir);
      blockneed += 2;
    } else {
      last_height = make_spire(col, last_height, n, m, dir);
      blockneed += 2;
    }

    col += n;
    remaining -= n;
  }

  // Fill ground below
  for (int i=0; i < _width; i++) {
    int y = get_lowest_obj_height(i);
    for (int j=0; j < y; j++) {
      _space[i][j] = new Ground(i,j);
      update_array(i, j, _space[i][j]->kind(), _space[i][j]->number());
    }
  }

  // TODO: generate usable blocks

  _init_data = _data;
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

void Scene::fill_ground() {
  int maxheight = Random::get(1, (int) _height * 3 / 4);
  int startcol = Random::get(_dist_width);
  int lastheight = maxheight;
  int priorheight = maxheight;

  // Generate ground to right
  for (int i = (int) startcol; i < _width; i++) {
    fill_ground(i, &lastheight, &priorheight, &maxheight);
  }

  lastheight = maxheight;
  priorheight = lastheight;

  // Generate ground to left
  for (int i = (int)startcol - 1; i >= 0; i--) {
    fill_ground(i, &lastheight, &priorheight, &maxheight);
  }
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

void Scene::update_array(int x, int y) {
  update_array(x, y, _space[x][y]->kind(), _space[x][y]->number());
}

int Scene::make_plains(int xstart, int base, int n, int m, int dir)
{
  /* Make a flat row of ground
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      the number of blocks in this chunk
   *  m:      (unused)
   *  dir:    (unused)
   */
  // __________
  DLOG_F(2, "Generating plains (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);

  for (int i = xstart; i < xstart + n; i++) {
    _space[i][base] = new Ground(i, base);
    update_array(i, base, _space[i][base]->kind(), _space[i][base]->number());
  }
  return base;
}

int Scene::make_steppes(int xstart, int base, int n, int m, int dir)
{
  /* Make "steps" that increase in height linearly with width
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      the number of blocks in this chunk
   *  m:      (unused)
   *  dir:    -1 if generating feature left, 1 if generating feature right
   */
  //       __
  //    __|
  // __|
  DLOG_F(2, "Generating steppes (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);

  int step = 0;
  for (int i = xstart; i < xstart + n; i++) {
    int j = base + step;
    _space[i][j] = new Ground(i, j);
    update_array(i, j, _space[i][j]->kind(), _space[i][j]->number());
    // If generating left,
    if (dir == -1) {
      // Only increment the step height if we won't go outside the map
      if (base + step < _height * 4 / 5)
        step++;
    // Else generating right,
    } else {
      // Only increment the step height if we won't go outside the map
      if (base + step > 1)
        step--;
    }
  }
  return base + step;
}

int Scene::make_plateau(int xstart, int base, int n, int m, int dir)
{
  /* Make a couple steps then a flat area
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      the number of blocks in this chunk
   *  m:      the number of steps to go up
   *  dir:    -1 if generating feature left, 1 if generating feature right
   */
  //       ____________
  //    __|
  // __|
  DLOG_F(2, "Generating plateau (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);

  assert(n >= 0 && m >= 0 && xstart >= 0 && base > 0);
  int step = 0;
  for (int i = xstart; i < xstart + n; i++) {
    int j = base + step;
    _space[i][j] = new Ground(i, j);
    update_array(i, j, _space[i][j]->kind(), _space[i][j]->number());
    // If generating left feature,
    if (dir == -1) {
      // Increase the step height if we're less than the plateau or the map height
      if (base + step < m && base + step < _height * 4 / 5)
        step++;
    // Else we're generating right feature
    } else {
      // Increase the step height if we're less than the plateau and above the minimum
      if (base + step < m && base + step > 1)
        step--;
    }
  }
  return base + step;
}

int Scene::make_canyon(int xstart, int base, int n, int m, int dir)
{
  /* Make a 2-block deep canyon
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      the number of blocks in this chunk
   *  m:      (unused)
   *  dir:    (unused)
   */
  // __        __
  //   |      |
  //   |______|
  DLOG_F(2, "Generating canyon (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);

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
  for (int i = xstart+1; i < xstart + n - 1; i++) {
    _space[i][height] = new Ground(i, height);
    update_array(i, height, _space[i][height]->kind(), _space[i][height]->number());
  }
  // Make the second wall
  _space[xstart+n-1][base] = new Ground(xstart+n-1, base);
  update_array(xstart+n-1, base, _space[xstart+n-1][base]->kind(), _space[xstart+n-1][base]->number());
  return base;
}

int Scene::make_cave(int xstart, int base, int n, int m, int dir)
{
  /* Make a cave structure
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      (unused)
   *  m:      (unused)
   *  dir:    -1 if generating feature left, 1 if generating feature right
   */
  DLOG_F(2, "Generating cave (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);
  return base;
}

int Scene::make_spire(int xstart, int base, int n, int m, int dir)
{
  /* Make a 2-block tall "spire"
   *  xstart: the column to put the leftmost block in
   *  base:   the height of the leftmost block
   *  n:      the number of blocks in this chunk
   *  m:      (unused)
   *  dir:    (unused)
   */
  //      __
  //     |  |
  // ____|  |____
  DLOG_F(2, "Generating spire (start col: %d, base height: %d, n: %d, m: %d, dir: %d).", xstart, base, n, m, dir);

  // First, create a flat row of ground
  for (int i = xstart; i < xstart + n; i++) {
    _space[i][base] = new Ground(i, base);
    update_array(i, base, _space[i][base]->kind(), _space[i][base]->number());
  }
  // Randomly pick a column to put the spire in
  int col = Random::get<int>(xstart+1, xstart+n-2);
  // Place the spire, checking for map boundaries
  if (base + 1 < _height - 1) {
    _space[col][base+1] = new Ground(col, base+1);
    update_array(col, base+1, _space[col][base+1]->kind(), _space[col][base+1]->number());
    if (base + 2 < _height - 1) {
      _space[col][base+2] = new Ground(col, base+2);
      update_array(col, base+2, _space[col][base+2]->kind(), _space[col][base+2]->number());
    }
  }
  return base;
}



/**
 * @brief Verify that a scene is playable and generate solution.
 *
 * @return true if the scene has a playable solution.
 * @return false if the scene is invalid.
 */
bool Scene::check_scene() {
  // LOG_SCOPE_FUNCTION(INFO);
  // Check a player exists
  if (_player == nullptr) {
    VLOG_F(GEN_WARN_V, "No player was found.");
    return false;
  }

  // Check if the player is in bounds
  if (_player->location().x < 0 || _player->location().x >= _width || _player->location().y < 0 || _player->location().y >= _height) {
    VLOG_F(GEN_WARN_V, "Player generated out of bounds.");
    return false;
  }

  // Check at least one block exists
  if (_blocks.size() == 0) {
    VLOG_F(GEN_WARN_V, "No blocks found.");
    return false;
  }

  // Check there are no blocks too high
  //   (would make blokboi carry out of map)
  for (int i=0; i<_width; i++) {
    if (_space[i][_height - 1] != nullptr) {
      VLOG_F(GEN_WARN_V, "Blocks found in top row.");
      return false;
    }
    if (_space[i][_height - 2] != nullptr) {
      if (_space[i][_height - 2] != _player) {
        VLOG_F(GEN_WARN_V, "Non-player block found in second to top row.");
        return false;
      }
    }
  }

  // Check that the player can at least get started with one block
  if (furthest_block_available(1) < 0 && furthest_block_available(-1) < 0) {
    VLOG_F(GEN_WARN_V, "Player would not be able to pick up any blocks.");
    return false;
  }

  // Check the player isn't on a pillar
  int player_col = _player->location().x;
  int player_height = _player->location().y;
  int pillar = false;
  for (int i=player_col; i<_width; i++) {
    int highest_obj = get_highest_obj_height(i);
    if (highest_obj == -1) {
      return false;
    }
    if (player_height != highest_obj && (abs(player_height - highest_obj) < 2)) {
      pillar = false;
      break;
    }
    if (player_height - highest_obj >= 2) {
      pillar = true;
    }
  }
  for (int i=player_col; i>=0; i--) {
    int highest_obj = get_highest_obj_height(i);
    if (highest_obj == -1) {
      return false;
    }
    if (player_height != highest_obj && (abs(player_height - highest_obj) < 2)) {
      pillar = false;
      break;
    }
    if (player_height - highest_obj >= 2) {
      pillar = true;
    }
  }
  if (pillar) {
    VLOG_F(GEN_WARN_V, "Detected a pillar.");
    return false;
  }

  // Generate the heurisitic
  if (verify()) {
    VLOG_F(GEN_WARN_V, "Scene generated with solution already complete.");
    return false;
  }

  return true;
}
