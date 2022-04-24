#include "game.h"
#include "player.h"
#include "scene.h"
#include <iostream>
#include <loguru.hpp>
#include <stdexcept>

using namespace std;

#define CHECKSTEPS 10000

Game::Game() {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_2);
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
  _objective = objective;
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

// const Objects Game::scene_space() const
// {
//     return _scene->get_space();
// }

string Game::representation() { return _scene->representation(); }

string Game::objective() { return _objective; }

/**
 * @brief Walk to a column.
 *
 * @param scene* the scene.
 * @param col int: column to walk to
 * @param steps* the step counter
 * @return int: 1 if successful, -1 if unsuccessful
 */
int walk_to(Scene *scene, int col, int *steps) {
  if (col < 0 || col >= scene->width()) {
    throw invalid_argument("Column out of map bounds.");
  }

  if (scene->get_player()->location().x < col) {
    // TODO: Walk right
  } else if (scene->get_player()->location().x > col) {
    // TODO: walk left
  } else {
    return 1;
  }
}

/**
 * @brief Run the heuristic player
 *
 * @return int: the number of steps taken. (-1 if heuristic failed)
 */
int Game::run_heuristic() {
  LOG_SCOPE_FUNCTION(INFO);

  string relationship = _scene->relationship();


  int steps = 0;

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
      LOG_F(ERROR, "Handling target relationship '%s' is undefined.", relationship);
      throw invalid_argument("I don't know how to handle the target relationship.");
    }

    steps++;
  }

  return -1;
}
