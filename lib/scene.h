#ifndef SCENE_H
#define SCENE_H

#include "block.h"
// #include "game_object.h"
#include "player.h"
#include <random>
#include <string>
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<GameObject *> ObjectList;
typedef std::vector<std::vector<GameObject *>> Objects;
typedef std::vector<std::vector<std::vector<char>>> Char3d;
typedef std::vector<std::vector<int>> Int2d;

/* DATA 3D array specification
    Dim(0) are the rows.
    Dim(1) are the columns.
    Dim(2) is a vector of [object type, block number value]
*/


class Scene {
protected:
  Blocks _blocks;
  Player *_player;
  unsigned _height = 10;
  unsigned _width = 10;
  Objects _space;
  Char3d _data;
  Char3d _init;
  Blocks _targets;
  std::string _relationship;
  std::vector<std::string> relations = {
    "above",
    "below",
    "under",
    "on top",
    "beneath",
    "right",
    "left",
    "off",
    "side",
    "diagonal"
  };
  std::uniform_int_distribution<int> _dist_width;
  std::uniform_int_distribution<int> _dist_height;
  LOCATION *findObject(GameObject *object);
  void fill_ground(int col, int *lastheight, int *priorheight, int *maxheight);
  void fill_ground();
  int count_blocks(int col);
  void place_walker_blocks(int player_col, int walk_col, int stay_col);
  bool check_scene();
  void update_array(int x, int y, char colrval, char numrval);
  void update_array(int x, int y);
  int make_plains(int xstart, int base, int n, int m, int dir);
  int make_steppes(int xstart, int base, int n, int m, int dir);
  int make_plateau(int xstart, int base, int n, int m, int dir);
  int make_canyon(int xstart, int base, int n, int m, int dir);
  int make_cave(int xstart, int base, int n, int m, int dir);
  int make_spire(int xstart, int base, int n, int m, int dir);

public:
  Scene();
  Scene(int x, int y);
  Scene(Char3d pregen);
  Scene(Char3d pregen, std::string objective, Int2d obj_coords);

  void generate();
  void generate_modular();
  void generate_heuristical();
  void generate_easy();
  void generate_from_array(Char3d pregen);
  void refresh();
  void flush();
  bool verify();
  void targets(Int2d coords);
  void relate(std::string relationship) { _relationship = relationship; }
  GameObject *get_object(int x, int y);
  int get_highest_obj_height(int col);
  int get_lowest_obj_height(int col);
  Player *get_player();
  const Blocks *targets() { return &_targets; }
  const Block *targets(int blocknum);
  std::string relationship() { return _relationship; }
  char *data() { return &_data[0][0][0]; }
  Char3d array() { return _data; }
  void move(GameObject *object, int dx, int dy);
  void move(int x, int y, int dx, int dy);
  std::string representation();
  int height() { return _height; }
  int width() { return _width; }
};

#endif /* SCENE_H */
