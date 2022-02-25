#ifndef WORLD_H
#define WORLD_H

#include "block.cpp"
#include "game_object.h"
#include "player.cpp"
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<std::vector<GameObject *>> Objects;

class World
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned _size[2] = {10, 10};
    Objects _space;

  public:
    World();
    World(int x, int y);

    void generate();
    void refresh();
    GameObject *get_object(int x, int y);
    void move(GameObject *object, int x, int y, int dx, int dy);
};

#endif /* WORLD_H */