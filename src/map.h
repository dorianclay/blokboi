#ifndef MAP_H
#define MAP_H

#include "block.h"
#include "player.h"
#include "location.h"
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<vector<Location *>> Locations;

class Map
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned _size[2] = {10, 10};
    Locations _space;
    

  public:
    Map();
    Map(int x, int y);
    
    void generate();
    void refresh();
    Location *assign_location(int x, int y);
};

#endif /* MAP_H */