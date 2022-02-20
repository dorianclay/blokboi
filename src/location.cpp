#include "location.h"

Location::Location(): Location(0, 0)
{
}

Location::Location(int x, int y)
{
    _current[0] = x;
    _current[1] = y;
}

const int* location() const {
    return _current;
}

void move(int x, int y) {
    _current[0] += x;
    _current[1] += y;
}

bool operator==(const Location& rhs) const {
    for (int i=0; i<sizeof(_current); i++) {
        if (_current[i] != rhs._current[i])
            return false;
    }
    return true;
}

bool operator!=(const Location& rhs) const {
    return !operator==(rhs);
}