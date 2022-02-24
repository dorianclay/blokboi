#include "map.h"

Map::Map()
{
    for (int i=0; i<_size[0]; i++) {
        for (int j=0; j<_size[1]; j++) {
            _space.at(i).at(j) = Location(i, j);
        }
    }
}