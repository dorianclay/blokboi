#ifndef GAME_H
#define GAME_H

#include "scene.h"
#include "player_controller.h"

class Game
{
private:
    Scene *_scene;
    PlayerController *_player_controller;
public:
    Game();
    void newGame();
    void resetGame();
    PlayerController *player_controller();
};

#endif /* GAME_H */