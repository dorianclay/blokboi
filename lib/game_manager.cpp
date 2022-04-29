#include "game_manager.h"
#include "game.h"
#include "scene.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  Game test_game = Game(demoscene, "", {{7,1},{14,4}}, "on top");
  // Game test_game = Game(demoscene);
  // test_game.newGame();
  cout << test_game.representation() << endl;

  test_game.run_heuristic();

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
