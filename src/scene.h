#ifndef SCENE_H
#define SCENE_H

#include "block.h"
// #include "game_object.h"
#include "player.h"
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
    typedef std::string string;

  protected:
    Blocks _blocks;
    Blocks _ground;
    Player _player;
    unsigned _height = 10;
    unsigned _width = 10;
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

#endif /* SCENE_H */