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
private:
  Blocks _blocks;
  Player *_player;
  unsigned _height = 10;
  unsigned _width = 10;
  Objects _space;
  Char3d _data;
  Blocks _targets;
  Int2d _target_features;
  std::vector<Blocks> _valid;
  bool _success = false;
  std::string _relationship;
  std::string _objective;

  Char3d _init_data;
  Int2d _init_obj_coords;


  static inline std::vector<std::string> relations = {
    "above",
    "below",
    "under",
    "on top",
    "beneath",
    "right",
    "left",
    // "off",
    "side",
    // "diagonal"
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
  void set_valid();
  void set_string();
  void flush();

public:
  // Constructors
  Scene();
  Scene(int x, int y);
  Scene(Char3d pregen);
  Scene(Char3d pregen, std::string objective, std::string relationship, Int2d obj_coords, Int2d feature_mask);

  // Destructors
  ~Scene();

  // Getters
  int height() const { return _height; }
  int width() const { return _width; }
  GameObject *get_object(int x, int y) const ;
  int get_highest_obj_height(int col) const;
  int get_highest_block_height(int col) const;
  int get_lowest_obj_height(int col) const;
  int furthest_block_available(int direction) const;
  Player *get_player() const {return _player; }
  const Blocks *targets() const { return &_targets; }
  const Block *targets(int blocknum) const;
  std::string relationship() const { return _relationship; }
  std::string objective() const { return _objective; }
  bool success() const { return _success; }
  const Int2d init_coords() const { return _init_obj_coords; }
  const Int2d feature_mask() const;
  Char3d array() const { return _data; }
  Char3d init_data() const { return _init_data; }
  std::string representation();

  // Setters
  void targets(Int2d coords, Int2d feature_mask);
  void relate(std::string relationship) { _relationship = relationship; }
  void objective(std::string objective) { _objective = objective; }

  // Mutators
  void generate();
  void generate_modular();
  void generate_heuristical();
  void generate_easy();
  void generate_from_array(Char3d pregen);
  void generate_from_saved(Char3d pregen, std::string objective, std::string relationship, Int2d obj_coords, Int2d feature_mask);
  void refresh();
  bool verify();
  void move(GameObject *object, int dx, int dy);
  void move(int x, int y, int dx, int dy);
};

#endif /* SCENE_H */
