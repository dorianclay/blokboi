#include "block.h"
#include "effolkronium/random.hpp"
#include <cstdlib>
#include <ctime>
#include <loguru.hpp>

using Random = effolkronium::random_static;

using namespace std;

// uniform_int_distribution<int> dist_colors((int)RED, (int)PURPLE);
uniform_int_distribution<int> dist_colors(0, 5);
uniform_int_distribution<int> dist_blocknum(MIN_NUMBER, MAX_NUMBER);

Block::Block() { Block(0, 0); }

Block::Block(int x, int y) {
  _location.x = x;
  _location.y = y;
  _color = colors[Random::get(dist_colors)];
  _number = Random::get(dist_blocknum);
  _movable = true;
  _kind = BLOCK;
}

Block::Block(int x, int y, char color, int number, bool movable)
    : _color(color), _number(number) {
  _location.x = x;
  _location.y = y;
  _movable = movable;
  _kind = BLOCK;
}

void Block::repr(ostream &ostr) const {
  ostr << _color;
}

Ground::Ground(int x, int y) {
  _location.x = x;
  _location.y = y;
  _number = 'X';
  _color = 'X';
  _movable = false;
  _kind = GROUND;
}

void Ground::repr(ostream &ostr) const { ostr << "@"; }

string colorname(char color) {
  switch(color) {
    case 'r':
      return "red";
    case 'o':
      return "orange";
    case 'y':
      return "yellow";
    case 'g':
      return "green";
    case 'b':
      return "blue";
    case 'p':
      return "purple";
    case 'X':
      return "blank";
    default:
      return "";
  }
}
