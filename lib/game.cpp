#include "scene.h"
#include "game.h"
#include <iostream>
#include <loguru.hpp>

using namespace std;

Game::Game()
{
    loguru::add_file("logs/blokboi_latest.log", loguru::Truncate, loguru::Verbosity_1);
    loguru::add_file("logs/blokboi_all.log", loguru::Append, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = 1;
    LOG_F(INFO, "Beginning a new game.");

    _scene = new Scene(20, 15);
    _player_controller = new PlayerController(_scene, _scene->get_player());
    _scene->generate_easy();
    DLOG_F(INFO, "Map generated:\n%s", _scene->representation());
}

void Game::newGame()
{
    LOG_F(INFO, "Making new game.");
    delete _scene;
    delete _player_controller;
    _scene = new Scene(20, 15);
    _player_controller = new PlayerController(_scene, _scene->get_player());
    _scene->generate_easy();
    DLOG_F(INFO, "New map generated:\n%s", _scene->representation());
}

void Game::resetGame()
{
    _scene->refresh();
}

PlayerController *Game::player_controller()
{
    return _player_controller;
}

string Game::strprint()
{
    return _scene->representation();
}




// int main(int argc, char *argv[])
// {
//     loguru::init(argc, argv);
//     loguru::add_file("logs/blokboi_latest.log", loguru::Truncate, loguru::Verbosity_1);
//     loguru::add_file("logs/blokboi_all.log", loguru::Append, loguru::Verbosity_INFO);
//     loguru::g_stderr_verbosity = 1;

//     DLOG_F(INFO, "Making a scene...");

//     Scene earth = Scene(30, 10);
//     earth.generate_easy();
//     string repr = earth.representation();
//     cout << repr << endl;
// }