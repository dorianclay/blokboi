#include "game.h"
#include "player.h"
#include "scene.h"
#include <iostream>
#include <loguru.hpp>
#include <stdexcept>

using namespace std;

Game::Game() {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_2);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   loguru::Verbosity_INFO);
  loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  LOG_F(INFO, "Beginning a new game.");

  _scene = new Scene(20, 15);
  _scene->generate_easy();
  _player_controller = new PlayerController(_scene, _scene->get_player());
  DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}

Game::Game(Char3d pregen) {
  loguru::add_file("logs/blokboi_latest.log", loguru::Truncate,
                   loguru::Verbosity_9);
  loguru::add_file("logs/blokboi_all.log", loguru::Append,
                   loguru::Verbosity_INFO);
  loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
  LOG_F(INFO, "Building game from pre-generated map.");

  _scene = new Scene(pregen);
  _player_controller = new PlayerController(_scene, _scene->get_player());
  DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}

void Game::newGame() {
  LOG_F(INFO, "Making new game.");
  delete _scene;
  delete _player_controller;
  _scene = new Scene(20, 15);
  _scene->generate_easy();
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
