#include "game.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Game test_game = Game();
    // test_game.newGame();
    cout << test_game.representation() << endl;

    test_game.move(-1);
    test_game.move(-1);
}
