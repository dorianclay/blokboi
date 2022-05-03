#include "game_manager.h"
#include "game.h"
#include "scene.h"
#include <iostream>

using namespace std;

void execute_demo(Game &demo) {
  bool debug_break = false;
  demo.move(LEFT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.move(LEFT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.jump();
  demo.move(RIGHT);
  demo.jump();
  demo.toggle_hold();
  demo.move(LEFT);
  demo.move(LEFT);
  demo.move(LEFT);
  demo.move(LEFT);
  demo.move(LEFT);
  demo.toggle_hold();
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.jump();
  demo.move(RIGHT);
  demo.jump();
  demo.jump();
  demo.jump();
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.move(RIGHT);
  demo.move(RIGHT);
  debug_break = true;
  cout << demo.representation() << endl;
  demo.toggle_hold();
}

int main(int argc, char *argv[]) {
  Game test = Game(demoscene,
                        "Put the blue block on top of the 9 block.",
                        "on top",
                        {{7,1},{19,4}},
                        {{1, 0}, {0, 1}}
                    );
  // test.newGame();
  cout << test.representation() << endl;

  execute_demo(test);

  cout << test.representation() << endl;

  // test.move(1);
  // test.move(-1);
  // test.toggle_hold();
  // test.move(1);
  // test.move(1);
  // cout << test.representation() << endl;

  // test.resetGame();
  // cout << test.representation() << endl;
}
