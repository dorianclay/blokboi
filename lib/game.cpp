#include "game.h"
#include "player.h"
#include "scene.h"
#include "effolkronium/random.hpp"
#include <iostream>
#include <loguru.hpp>
#include <stdexcept>

using Random = effolkronium::random_static;
using namespace std;

#define CHECKSTEPS 1000
#define WALKATTEMPTS 100
#ifdef NDEBUG
  #define V_LEVEL_LATEST loguru::Verbosity_INFO
#else
  #define V_LEVEL_LATEST 5
#endif
#define V_LEVEL_ROLLING loguru::Verbosity_INFO
#define V_LEVEL_STDERR loguru::Verbosity_OFF

void report(Game &game) {
  DLOG_F(INFO, "Map generated:");
  DLOG_F(INFO, " - Targets at (%d, %d) and (%d, %d).",
      game.scene()->targets(0)->location().x,
      game.scene()->targets(0)->location().y,
      game.scene()->targets(1)->location().x,
      game.scene()->targets(1)->location().y
  );
  Int2d temp = game.scene()->feature_mask();
  DLOG_F(INFO, " - Features: [%c, %c] and [%c, %c].",
      (temp[0][0] == 0) ? '0' : game.scene()->targets(0)->color(),
      (temp[0][1] == 0) ? '0' : game.scene()->targets(0)->number(),
      (temp[1][0] == 0) ? '0' : game.scene()->targets(1)->color(),
      (temp[1][1] == 0) ? '0' : game.scene()->targets(1)->number()
  );
  DLOG_F(INFO, "\n%s", game.representation().c_str());
}


Game::Game() {

  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   V_LEVEL_LATEST);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   V_LEVEL_ROLLING);
  loguru::g_stderr_verbosity = V_LEVEL_STDERR;
  LOG_F(INFO, "Beginning a new game.");

  _scene = new Scene(20, 15);
  _scene->generate();
  _player_controller = new PlayerController(_scene, _scene->get_player());
  report(*this);
}

Game::Game(Char3d pregen, string objective) {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   V_LEVEL_LATEST);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   V_LEVEL_ROLLING);
  loguru::g_stderr_verbosity = V_LEVEL_STDERR;
  LOG_F(INFO, "Building game from pre-generated map.");

  _scene = new Scene(pregen);
  _player_controller = new PlayerController(_scene, _scene->get_player());
  _scene->objective(objective);
  report(*this);
}

Game::Game(Char3d pregen, string objective, string relationship, Int2d obj_coords, Int2d feature_mask) {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   V_LEVEL_LATEST);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   V_LEVEL_ROLLING);
  loguru::g_stderr_verbosity = V_LEVEL_STDERR;
  LOG_F(INFO, "Building game from pre-generated map.");

  _scene = new Scene(pregen, objective, relationship, obj_coords, feature_mask);
  _player_controller = new PlayerController(_scene, _scene->get_player());
  report(*this);
}

bool Game::success() const {
  bool success = _scene->success();
  DLOG_IF_F(1, success, "%s", steps_taken().c_str());
  return success;
}


void Game::newGame() {
  LOG_F(INFO, "Making new game.");
  delete _scene;
  delete _player_controller;
  _scene = new Scene(20, 15);
  _scene->generate();
  _player_controller = new PlayerController(_scene, _scene->get_player());
  report(*this);
}

void Game::resetGame() {
  LOG_F(INFO, "Resetting scene.");
  _scene->refresh();
  delete _player_controller;
  _player_controller = new PlayerController(_scene, _scene->get_player());
}

int Game::move(int direction) {
  if (direction != LEFT && direction != RIGHT) {
    DLOG_F(ERROR, "Direction provided incorrectly to C++ Game API: %d",
           direction);
    throw invalid_argument("Direction must be -1 (left) or 1 (right).");
  }

  _sstream << ((direction == LEFT) ? "l" : "r");
  return _player_controller->move(direction);
}

int Game::jump() {
  _sstream << "j";
  return _player_controller->jump();
}

int Game::toggle_hold() {
  if (_player_controller->holding())
    return put_down();
  else
    return pick_up();
}

int Game::pick_up() {
  _sstream << "p";
  return _player_controller->pick_up();
}

int Game::put_down() {
  _sstream << "p";
  return _player_controller->put_down();
}

/**
 * @brief Walk to a column.
 *
 * @param game reference to the game.
 * @param col int: column to walk to.
 * @param steps reference to the step counter.
 * @return int: 1 if successful, -1 if unsuccessful.
 */
int walk_to(Game &game, int col, int &steps) {
  LOG_SCOPE_FUNCTION(4);
  DLOG_F(4, "Attempting to walk to column %d.", col);

  if (col < 0 || col >= game.width()) {
    throw invalid_argument("Column out of map bounds.");
  }
  int err_code = 0;

  if (game.player_location().x < col) {
    // Walk right
    for (int i=0; i<game.width() * 2; i++) {
      steps++;
      err_code = game.move(1);
      if (err_code == -1) {
        err_code = game.jump();
        steps++;
      }
      if (err_code < 0 || game.player_location().x == col)
        break;
    }
  } else if (game.player_location().x > col) {
    // walk left
    for (int i=0; i<game.width() * 2; i++) {
      steps++;
      err_code = game.move(-1);
      if (err_code == -1) {
        err_code = game.jump();
        steps++;
      }
      if (err_code < 0 || game.player_location().x == col)
        break;
    }
  } else {
    err_code = game.move(- game.scene()->get_player()->facing());
    steps++;
  }
  DLOG_F(7, "Map after walking:\n%s", game.representation().c_str());

  if (err_code >= 0)
    return 1;
  else
    return err_code;
}

/**
 * @brief Get the to column given, building steps to get there
 *
 * @param game reference to the game.
 * @param col int: column to walk to.
 * @param steps reference to the step counter.
 * @return int: 1 if successful, -1 if unsuccessful.
 */
int get_to_col(Game &game, int col, int &steps) {
  int attempts = 0;
  int dir = game.player_location().x < col ? 1 : -1;
  if (col == 0)
    dir = -1;
  else if (col == game.width() - 1)
    dir = 1;

  while (attempts < WALKATTEMPTS) {
    attempts++;

    int success = walk_to(game, col, steps);
    if (success >= 0) {
      DLOG_F(3, "Navigated to column %d.", col);
      return 1;
    } else {
      // See if we can't go further because it's a wall or a cliff
      int missedheight = game.scene()->get_highest_obj_height(game.player_location().x + dir);
      int nextcol = game.player_location().x;
      // See if we're already holding a block
      if (game.holding()) {
        // If it's a wall, we need to place the block where we're currently standing...
        if (missedheight > game.player_location().y) {
          nextcol = game.player_location().x - dir;
          walk_to(game, nextcol, steps);
          walk_to(game, game.player_location().x, steps);
          game.toggle_hold();
          steps++;
        // Then it's a cliff and we're already holding a block, so just drop it
        } else {
          game.toggle_hold();
          steps++;
        }
      // If we're not holding a block, we'll need to find one:
      } else {
        // If it's a wall, we need to place the block where we're currently standing...
        if (missedheight > game.player_location().y) {
          nextcol = game.player_location().x - dir;
        // If it's a cliff, check first if we are standing on a block we can pick up
        } else if (game.player_location().y - 1 - game.scene()->get_highest_obj_height(game.player_location().x - dir) == 1) {
          if (game.scene()->get_object(nextcol, game.player_location().y - 1)->isBlock()) {
            walk_to(game, game.player_location().x - dir, steps);
            walk_to(game, game.player_location().x, steps);
            game.toggle_hold();
            steps++;
            // walk_to(game, nextcol, steps);
            // game.toggle_hold();
            // steps++;
            continue;
          }
        }

        // If we didn't get to the column desired, turn around, get a block, and put it there
        int buildblock = game.scene()->furthest_block_available(-game.scene()->get_player()->facing());
        int blockclose = buildblock + dir;
        int blockfar = buildblock - dir;

        DLOG_F(3, "I want a building block at column %d.", buildblock);
        // If furthest block was invalid
        if (buildblock < 0 || buildblock >= game.width())
          break;

        // Get the height of the relevant blocks
        int bbheight = game.scene()->get_highest_obj_height(buildblock);
        int bbcloseheight = game.scene()->get_highest_obj_height(buildblock + dir);
        if (game.scene()->get_player() == game.scene()->get_object(buildblock + dir, bbcloseheight))
          bbcloseheight -= 1;
        // If we're standing on the block we need
        if (buildblock == game.player_location().x) {
          walk_to(game, game.player_location().x - dir, steps);
          walk_to(game, game.player_location().x, steps);
          game.toggle_hold();
          steps++;
          walk_to(game, nextcol, steps);
          game.toggle_hold();
          steps++;
        // If we walk straight to the block
        } else if (bbheight - bbcloseheight == 1) {
          walk_to(game, buildblock + (game.player_location().x > buildblock ? 1 : -1), steps);
          game.toggle_hold();
          steps++;
          walk_to(game, nextcol, steps);
          game.toggle_hold();
          steps++;
        // If we need to cut back to pick up
        // - check for cutting back only if the block we want isn't at the end of the map
        } else if (blockfar >= 0 && blockfar < game.width()) {
          int bbfarheight = game.scene()->get_highest_obj_height(buildblock - dir);
          if (game.scene()->get_player() == game.scene()->get_object(buildblock - dir, bbfarheight))
            bbcloseheight -= 1;
          if (bbheight - bbfarheight == 1) {
            walk_to(game, buildblock - dir, steps);
            walk_to(game, game.player_location().x, steps);
            game.toggle_hold();
            steps++;
            walk_to(game, nextcol, steps);
            game.toggle_hold();
            steps++;
          }
        // None above seems to be true...
        } else {
          throw runtime_error("Not sure how to proceed...It appears I am trying to navigate to a building block I can't access.");
        }
      }
    }
  }

  DLOG_F(WARNING, "Did not build and get to the column desired in less than %d attempts.", WALKATTEMPTS);
  return -1;
}

/**
 * @brief Bring a specific block to a column.
 *
 * @param game reference to the game
 * @param block const reference to the block to be moved
 * @param col the column to move to
 * @param steps reference to the step counter
 *
 * @return 1 if successful, -1 if unsuccessful.
 */
int bring_to(Game &game, const Block &block, int col, bool place, int &steps) {
  LOG_SCOPE_FUNCTION(3);
  DLOG_IF_F(3, !place, "Bringing block from col %d to %d.", block.location().x, col);
  DLOG_IF_F(3, place, "Placing block at col %d on col %d.", block.location().x, col);
  // Get the relative direction of the goal:
  //   Goal is to the RIGHT (1)
  //   Goal is to the LEFT (-1)
  int dir = game.player_location().x < block.location().x ? 1 : -1;
  if (col == 0)
    dir = -1;
  else if (col == game.width() - 1)
    dir = 1;
  int success = 1;

  // Clear a path to the column
  success = get_to_col(game, col, steps);

  // See if we can access the desired block from player-side
  dir = game.player_location().x < block.location().x ? 1 : -1;
  if (block.location().x - dir >= 0 && block.location().x - dir < game.width()) {
    if (block.location().y > game.scene()->get_highest_obj_height(block.location().x - dir)) {
      success = get_to_col(game, block.location().x - dir, steps);
    }
  // If not, access the block from the other direction
  } else {
    int newcol = block.location().x + dir;
    if (newcol < 0 || newcol >= game.width()) {
      return -1;
    }
    success = get_to_col(game, block.location().x + dir, steps);
    get_to_col(game, game.player_location().x , steps);
  }

  if (success == 1) {
    game.toggle_hold();
    steps++;
    dir = game.player_location().x < col ? 1 : -1;
    if (place)
      success = get_to_col(game, col - dir, steps);
    else
      success = get_to_col(game, col, steps);
  }
  if (success == 1 && place) {
    game.toggle_hold();
    steps++;
  }


  return success;
}

int manual = 0;

/**
 * @brief Run the heuristic player
 *
 * @return int: the number of steps taken. (-1 if heuristic failed)
 */
int Game::run_heuristic() {
  LOG_SCOPE_FUNCTION(INFO);

  string relationship = _scene->relationship();
  DLOG_F(2, "Running heurisitic for relationship: %s", relationship.c_str());

  int steps = 0;

  while (steps < CHECKSTEPS) {
    // Bring the first target (_targets[0]) to the second target (_targets[1])
      // If can't reach the target, turn around and pick up the farthest available block

      // Place the block at the obstacle, and try to walk towards the first target again

    // Arrange the target blocks as defined by their relationship
    if (relationship == "above" || relationship == "on top") {
      // _target[0] on _target[1]
      try {
        bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x, true, steps);
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "below" || relationship == "under" || relationship == "beneath") {
      // _target[0] under _target[1]
      try {
        // Randomly attempt to put the first block either to the right or left of the second
        int side = Random::get<bool>()? 1 : -1;
        // If that side is out of bounds, use the other
        if (_scene->targets(1)->location().x + side < 0 || _scene->targets(1)->location().x + side >= width())
          side = -side;
        bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x + side, true, steps);
        // Put the second block on the first one
        bring_to(*this, *_scene->targets(1), _scene->targets(0)->location().x, true, steps);
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "right") {
      // t[0] to the right of t[1]
      try {
        // Randomly decide whether to move first or second block
        int block = Random::get<int>(0,1);
        if (block == 0) {
          if (_scene->targets(1)->location().x + 1 < 0)
            continue;
          bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x + 1, true, steps);
        } else {
          if (_scene->targets(0)->location().x - 1 < 0)
            continue;
          bring_to(*this, *_scene->targets(1), _scene->targets(0)->location().x - 1, true, steps);
        }
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "left") {
      // t[0] to the left of t[1]
      try {
        // Randomly decide whether to move first or second block
        int block = Random::get<int>(0,1);
        if (block == 0) {
          if (_scene->targets(1)->location().x - 1 < 0)
            continue;
          bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x - 1, true, steps);
        } else {
          if (_scene->targets(0)->location().x + 1 < 0)
            continue;
          bring_to(*this, *_scene->targets(1), _scene->targets(0)->location().x + 1, true, steps);
        }
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "side") {
      // t[0] directly beside t[1]
      try {
        // Randomly attempt to put the first block either to the right or left of the second
        int side = Random::get<bool>()? 1 : -1;
        // If that side is out of bounds, use the other
        if (_scene->targets(1)->location().x + side < 0 || _scene->targets(1)->location().x + side >= width())
          side = -side;
        bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x + side, true, steps);
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "off") {
      // t[0] not above t[1]
      try {
        // Randomly attempt to put the first block either to the right or left of the second
        int side = Random::get<bool>()? 1 : -1;
        // If that side is out of bounds, use the other
        if (_scene->targets(1)->location().x + side < 0 || _scene->targets(1)->location().x + side >= width())
          side = -side;
        bring_to(*this, *_scene->targets(0), _scene->targets(1)->location().x + side, true, steps);
      }
      catch (exception& e) {
        DLOG_F(4, "Caught exception: %s", e.what());
      }
    } else if (relationship == "diagonal") {
      // t[0] at an adjacent diagonal to t[1]
      // Randomly pick a side
      int side = Random::get<bool>()? 1 : -1;
      const Block *stat = dynamic_cast<const Block*>(_scene->targets(1));
      const Block *move = dynamic_cast<const Block*>(_scene->targets(0));
      // See if one of the blocks has blocks next to it
      if (_scene->targets(0)->location().y <= _scene->get_highest_block_height(_scene->targets(0)->location().x) + 1 ||
          _scene->targets(0)->location().y <= _scene->get_highest_block_height(_scene->targets(0)->location().x) - 1) {
            stat = dynamic_cast<const Block*>(_scene->targets(0));
            move = dynamic_cast<const Block*>(_scene->targets(1));
          }
      if (_scene->targets(1)->location().y <= _scene->get_highest_block_height(_scene->targets(1)->location().x) + 1 ||
          _scene->targets(1)->location().y <= _scene->get_highest_block_height(_scene->targets(1)->location().x) - 1) {
            stat = dynamic_cast<const Block*>(_scene->targets(1));
            move = dynamic_cast<const Block*>(_scene->targets(0));
          }
      bring_to(*this, *move, stat->location().x + side, true, steps);
    } else {
      LOG_F(INFO, "Handling target relationship '%s' is undefined.", relationship.c_str());
      throw invalid_argument("I don't know how to handle the target relationship.");
    }

    if (success()) {
      return 1;
    }
  }

  LOG_F(WARNING, "Did not achieve objective in less than %d steps.", CHECKSTEPS);
  return -1;
}
