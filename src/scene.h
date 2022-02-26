#ifndef WORLD_H
#define WORLD_H

#include "block.cpp"
#include "game_object.h"
#include "player.cpp"
#include <string>
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<std::vector<GameObject *>> Objects;

typedef union location {
    int x;
    int y;
} LOCATION;

class Scene
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned _size[2] = {10, 10};
    Objects _space;
    LOCATION *findObject(GameObject *object);

  public:
    Scene();
    Scene(int x, int y);

    void generate();
    void generate(const string &str);
    void refresh();
    void flush();
    GameObject *get_object(int x, int y);
    void move(GameObject *object, int dx, int dy);
    void move(int x, int y, int dx, int dy);
};

#endif /* WORLD_H */