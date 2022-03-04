#ifndef SCENE_H
#define SCENE_H

#include "block.h"
// #include "game_object.h"
#include "player.h"
#include <string>
#include <vector>

typedef std::vector<Block *> Blocks;
typedef std::vector<GameObject *> ObjectList;
typedef std::vector<std::vector<GameObject *>> Objects;

class Scene
{
  protected:
    Blocks _blocks;
    Blocks _ground;
    Player *_player;
    unsigned _height = 10;
    unsigned _width = 10;
    Objects _space;
    LOCATION *findObject(GameObject *object);
    void fill_ground(int col, int *lastheight, int *priorheight, int *maxheight);

  public:
    Scene();
    Scene(int x, int y);

    void generate_easy();
    void generate(const std::string &str);
    void refresh();
    void flush();
    GameObject *get_object(int x, int y);
    void move(GameObject *object, int dx, int dy);
    void move(int x, int y, int dx, int dy);
    std::string representation();
};

#endif /* SCENE_H */