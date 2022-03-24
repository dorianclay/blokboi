#include "game.h"
#include "player.h"
#include "scene.h"
#include <iostream>
#include <loguru.hpp>
#include <stdexcept>

using namespace std;

Game::Game()
{
    loguru::add_file("logs/blokboi_latest.log", loguru::Truncate, loguru::Verbosity_1);
    loguru::add_file("logs/blokboi_all.log", loguru::Append, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = -2;
    LOG_F(INFO, "Beginning a new game.");

    _scene = new Scene(20, 15);
    _player_controller = new PlayerController(_scene, _scene->get_player());
    _scene->generate_easy();
    DLOG_F(INFO, "Map generated:\n%s", _scene->representation().c_str());
}

void Game::newGame()
{
    LOG_F(INFO, "Making new game.");
    delete _scene;
    delete _player_controller;
    _scene = new Scene(20, 15);
    _player_controller = new PlayerController(_scene, _scene->get_player());
    _scene->generate_easy();
    DLOG_F(INFO, "New map generated:\n%s", _scene->representation().c_str());
}

void Game::resetGame()
{
    _scene->refresh();
}

void Game::move(int direction)
{
    if (direction != LEFT && direction != RIGHT)
    {
        DLOG_F(ERROR, "Direction provided incorrectly to C++ Game API: %d", direction);
        throw invalid_argument("Direction must be -1 (left) or 1 (right).");
    }

    _player_controller->move(direction);
}

void Game::jump()
{
    _player_controller->jump();
}

void Game::pick_up()
{
    _player_controller->pick_up();
}

void Game::put_down()
{
    _player_controller->put_down();
}

string Game::representation()
{
    return _scene->representation();
}

string Game::objective()
{
    return _objective;
}