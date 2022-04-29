#ifndef GAME_H
#define GAME_H

#include "player_controller.h"
#include "scene.h"
#include <string>

class Game {
private:
  Scene *_scene;
  PlayerController *_player_controller;
  std::string _objective = "";

public:
  Game();
  Game(Char3d pregen, std::string objective="");
  Game(Char3d pregen, std::string objective, Int2d obj_coords, std::string relationship);
  void newGame();
  void resetGame();
  int run_heuristic();
  void set_objective(std::string objective) { _objective = objective; }
  int move(int direction);
  int jump();
  int toggle_hold();
  int pick_up();
  int put_down();
  Scene *scene() { return _scene; }
  int width() { return _scene->width(); }
  int height() { return _scene->height(); }
  const LOCATION player_location() { return _scene->get_player()->location(); }
  Char3d array() { return _scene->array(); }
  std::string representation();
  std::string objective();
};

#endif /* GAME_H */
