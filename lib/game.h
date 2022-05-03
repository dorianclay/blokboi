#ifndef GAME_H
#define GAME_H

#include "player_controller.h"
#include "scene.h"
#include <string>
#include <sstream>

class Game {
private:
  Scene *_scene;
  PlayerController *_player_controller;
  std::stringstream _sstream;

public:
  // Constructors
  Game();
  Game(Char3d pregen, std::string objective="");
  Game(Char3d pregen, std::string objective, std::string relationship, Int2d obj_coords, Int2d feature_mask);

  // Getters
  int width() const { return _scene->width(); }
  int height() const { return _scene->height(); }
  Scene *scene() const { return _scene; }
  const LOCATION player_location() const { return _scene->get_player()->location(); }
  Char3d array() const { return _scene->array(); }
  Char3d init_data() const { return _scene->init_data(); }
  std::string representation() const { return _scene->representation(); }
  std::string objective() const { return _scene->objective(); }
  std::string relationship() const { return _scene->relationship(); }
  const Int2d init_coords() const { return _scene->init_coords(); }
  const Int2d feature_mask() const { return _scene->feature_mask(); }
  bool success() const;
  std::string steps_taken() const { return _sstream.str(); }


  // Setters
  void set_objective(std::string objective) { _scene->objective(objective); }


  // Mutators
  void newGame();
  void resetGame();
  int run_heuristic();
  int move(int direction);
  int jump();
  int toggle_hold();
  int pick_up();
  int put_down();
};

#endif /* GAME_H */
