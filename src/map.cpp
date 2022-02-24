#include "map.h"
#include <vector>

using namespace std;

Map::Map()
{
    _space = Objects(_size[0], vector<GameObject *> (_size[1]));

    for (int i=0; i<_size[0]; i++) {
        for (int j=0; j<_size[1]; j++) {
            _space[i][j] = nullptr;
        }
    }
}

Map::Map(int x, int y)
{
    _size[0] = x;
    _size[1] = y;
    Map();
}