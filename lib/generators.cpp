#include "scene.h"
#include "effolkronium/random.hpp"
#include <loguru.hpp>
#include <stdexcept>

using Random = effolkronium::random_static;

using namespace std;

#define GENERATOR_ATTEMPTS 1000
#define GEN_WARN_V 2

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

    // Pick objective blocks
    int target1 = Random::get(0, (int) _blocks.size() - 1);
    int target2;
    for (int i=0; i<_blocks.size(); i++) {
      target2 = Random::get(0, (int) _blocks.size() - 1);
      if (target1 != target2)
        break;
    }
    if (target1 == target2) {
      continue;
    }
    _targets.push_back(_blocks.at(target1));
    _targets.push_back(_blocks.at(target2));
    _relationship = relations.at(Random::get(0, (int) relations.size() - 1));

    if (check_scene()) {
      LOG_F(INFO, "Generated a scene after %d tries.", attempts);
      break;
    }
  }
  if (attempts == GENERATOR_ATTEMPTS) {
    throw runtime_error("Could not generate a good scene in " + to_string(attempts) + " tries.");
  }

  _init = _data;
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

  _init = _data;
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
        _space[i][j] = new Block(i, j, kind, ident - '0', true);
      }
    }
  }

  _init = _data;
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
