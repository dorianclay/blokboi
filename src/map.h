#ifndef MAP_H
#define MAP_H

#include "block.h"
#include "player.h"
#include <vector>

typedef std::vector<Block *> Blocks;

class Map
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned size[2];

  public:
    void generate();
    void refresh();
};

#endif /* MAP_H */