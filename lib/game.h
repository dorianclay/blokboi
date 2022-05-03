#ifndef GAME_H
#define GAME_H

#include "player_controller.h"
#include "scene.h"
#include <string>

class Game {
private:
  Scene *_scene;
  PlayerController *_player_controller;

public:
  Game();
  Game(Char3d pregen, std::string objective="");
  Game(Char3d pregen, std::string objective, std::string relationship, Int2d obj_coords, Int2d feature_mask);
  void newGame();
  void resetGame();
  int run_heuristic();
  void set_objective(std::string objective) { _scene->objective(objective); }
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
  bool success() const { return _scene->success(); }
};

#endif /* GAME_H */
