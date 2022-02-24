#ifndef MAP_H
#define MAP_H

#include "block.h"
#include "game_object.h"
#include "location.h"
#include "player.h"
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<std::vector<GameObject *>> Objects;

class Map
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned _size[2] = {10, 10};
    Objects _space;
    Map();

  public:
    Map(int x, int y);

    void generate();
    void refresh();
};

#endif /* MAP_H */