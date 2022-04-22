#include "scene.h"
#include "effolkronium/random.hpp"
#include <loguru.hpp>

using Random = effolkronium::random_static;

using namespace std;

void Scene::generate() {
  generate_heuristical();
}

void Scene::place_walker_blocks(int player_col, int walk_col, int stay_col) {
  int block_col, block_row;
  while (true) {
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
  _blocks.push_back(new Block(block_col, block_row));
  _space[block_col][block_row] = _blocks.back();
  update_array(block_col, block_row);
}

void Scene::generate_heuristical() {
  flush();
  srand(time(NULL));

  DLOG_F(INFO, "Generating scene heuristically...");

  // Keep generating until we've made a satisfactory map
  while (true) {
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
        // TODO: check conditional is actually finding a step "landing" point.
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
        // TODO: check conditional is finding a step landing point
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
      while (true) {
        block_col = Random::get(_dist_width);
        block_row = count_blocks(block_col);
        // Keep getting a random column until we have one that is under the ceiling
        if (block_row < _height -1)
          break;
      }
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

    // TODO: check that map is playable before breaking
    break;
  }

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
