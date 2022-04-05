#include "game.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Game test_game = Game();
    // test_game.newGame();
    cout << test_game.representation() << endl;

    printf("Hellow World %x\n", test_game.data());
    for (int i=0; i<test_game.width(); i++) {
        for (int j=0; j<test_game.height(); j++) {
            cout << "[" << test_game.data()[i] << ",";
                //  << test_game.data()[i] << "] ";
        }
        cout << endl;
    }
}