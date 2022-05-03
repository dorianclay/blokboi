#include "game.h"
#include "player.h"
#include "scene.h"
#include <iostream>
#include <loguru.hpp>
#include <stdexcept>

using namespace std;

#define CHECKSTEPS 10000
#define WALKATTEMPTS 100

Game::Game() {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_4);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   loguru::Verbosity_INFO);
  loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  LOG_F(INFO, "Beginning a new game.");

  _scene = new Scene(20, 15);
  _scene->generate();
  _player_controller = new PlayerController(_scene, _scene->get_player());
  DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}

Game::Game(Char3d pregen, string objective) {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_9);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   loguru::Verbosity_INFO);
  loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  LOG_F(INFO, "Building game from pre-generated map.");

  _scene = new Scene(pregen);
  _player_controller = new PlayerController(_scene, _scene->get_player());
  _scene->objective(objective);
  DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}

Game::Game(Char3d pregen, string objective, string relationship, Int2d obj_coords, Int2d feature_mask) {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_9);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   loguru::Verbosity_INFO);
  loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  LOG_F(INFO, "Building game from pre-generated map.");

  _scene = new Scene(pregen, objective, relationship, obj_coords, feature_mask);
  _player_controller = new PlayerController(_scene, _scene->get_player());
  DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}


void Game::newGame() {
  LOG_F(INFO, "Making new game.");
  delete _scene;
  delete _player_controller;
  _scene = new Scene(20, 15);
  _scene->generate();
  _player_controller = new PlayerController(_scene, _scene->get_player());
  DLOG_F(INFO, "New map generated:\n%s", _scene->representation().c_str());
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

  return _player_controller->move(direction);
}

int Game::jump() { return _player_controller->jump(); }

int Game::toggle_hold() {
  if (_player_controller->holding())
    return put_down();
  else
    return pick_up();
}

int Game::pick_up() { return _player_controller->pick_up(); }

int Game::put_down() { return _player_controller->put_down(); }

string Game::representation() { return _scene->representation(); }

string Game::objective() { return _scene->objective(); }

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
      }
      if (err_code < 0 || game.player_location().x == col)
        break;
    }
  } else {
    err_code = game.move(- game.scene()->get_player()->facing());
  }
  DLOG_F(7, "Map after walking:\n%s", game.representation().c_str());

  if (err_code >= 0)
    return 1;
  else
    return err_code;
  // if (game.player_location().x == col)
  //   return 1;
  // else
  //   return -1;
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
      // If it's a wall, we need to place the block where we're currently standing...
      if (missedheight > game.player_location().y) {
        nextcol = game.player_location().x - dir;
      // If it's a cliff, check first if we are standing on a block we can pick up
      } else if (game.player_location().y - 1 - game.scene()->get_highest_obj_height(game.player_location().x - dir) == 1) {
        if (game.scene()->get_object(nextcol, game.player_location().y - 1)->isBlock()) {
          walk_to(game, game.player_location().x - dir, steps);
          walk_to(game, game.player_location().x, steps);
          game.toggle_hold();
          walk_to(game, nextcol, steps);
          game.toggle_hold();
          continue;
        }
      }



      // If we didn't get to the column desired, turn around, get a block, and put it there
      int buildblock = game.scene()->furthest_block_available(-game.scene()->get_player()->facing());
      DLOG_F(3, "I want a building block at column %d.", buildblock);
      // If furthest block was invalid
      if (buildblock < 0)
        break;
      int bbheight = game.scene()->get_highest_obj_height(buildblock);
      int bbcloseheight = game.scene()->get_highest_obj_height(buildblock + dir);
      if (game.scene()->get_object(buildblock + dir, bbcloseheight) == game.scene()->get_player())
        bbcloseheight -= 1;
      int bbfarheight = game.scene()->get_highest_obj_height(buildblock - dir);
      if (game.scene()->get_object(buildblock - dir, bbfarheight) == game.scene()->get_player())
        bbcloseheight -= 1;
      // If we're standing on the block we need
      else if (buildblock == game.player_location().x) {
        walk_to(game, game.player_location().x - dir, steps);
        walk_to(game, game.player_location().x, steps);
        game.toggle_hold();
        walk_to(game, nextcol, steps);
        game.toggle_hold();
      // If we walk straight to the block
      } else if (bbheight - bbcloseheight == 1) {
        walk_to(game, buildblock + (game.player_location().x > buildblock ? 1 : -1), steps);
        game.toggle_hold();
        walk_to(game, nextcol, steps);
        game.toggle_hold();
      // If we need to cut back to pick up
      } else if (bbheight - bbfarheight == 1) {
        walk_to(game, buildblock - dir, steps);
        walk_to(game, game.player_location().x, steps);
        game.toggle_hold();
        walk_to(game, nextcol, steps);
        game.toggle_hold();
      // None above seems to be true...
      } else {
        throw runtime_error("Not sure how to proceed...It appears I am trying to navigate to a building block I can't access.");
      }
    }
  }

  LOG_F(WARNING, "Did not build and get to the column desired in less than %d attempts.", WALKATTEMPTS);
  return -1;
}

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

  // TODO: REMOVE DEBUG COMMANDS
  try {
    get_to_col(*this, width() - 1, steps);
  }
  catch (exception& e) {
    DLOG_F(ERROR, "Caught exception: %s", e.what());
  }

  return 1;

  while (steps < CHECKSTEPS) {
    // Bring the first target (_targets[0]) to the second target (_targets[1])
      // If can't reach the target, turn around and pick up the farthest available block

      // Place the block at the obstacle, and try to walk towards the first target again

    // Arrange the target blocks as defined by their relationship
    if (relationship == "above" || relationship == "on top") {
      // TODO: _target[0] on _target[1]
    } else if (relationship == "below" || relationship == "under" || relationship == "beneath") {
      // TODO: _target[0] under _target[1]
    } else if (relationship == "right") {
      // TODO: t[0] to the right of t[1]
    } else if (relationship == "left") {
      // TODO: t[0] to the left of t[1]
    } else if (relationship == "side") {
      // TODO: t[0] directly beside t[1]
    } else if (relationship == "off") {
      // TODO: t[0] not above t[1]
    } else if (relationship == "diagonal") {
      // TODO: t[0] at an adjacent diagonal to t[1]
    } else {
      LOG_F(INFO, "Handling target relationship '%s' is undefined.", relationship.c_str());
      throw invalid_argument("I don't know how to handle the target relationship.");
    }

    steps++;
  }

  return -1;
}
