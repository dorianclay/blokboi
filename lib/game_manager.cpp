#include "game_manager.h"
#include "game.h"
#include "scene.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  Game test_game = Game(demoscene,
                        "Put the blue block on top of the 9 block.",
                        "on top",
                        {{7,1},{14,4}},
                        {{1, 0}, {0, 1}}
                    );
  // test_game.newGame();
  cout << test_game.representation() << endl;

  test_game.move(-1);
  test_game.toggle_hold();
  test_game.toggle_hold();

  cout << test_game.representation() << endl;

  // test_game.move(1);
  // test_game.move(-1);
  // test_game.toggle_hold();
  // test_game.move(1);
  // test_game.move(1);
  // cout << test_game.representation() << endl;

  // test_game.resetGame();
  // cout << test_game.representation() << endl;
}
