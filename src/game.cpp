#include "scene.h"
#include <iostream>
#include <loguru.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    loguru::init(argc, argv);
    loguru::add_file("logs/blokboi_latest.log", loguru::Truncate, loguru::Verbosity_1);
    // loguru::add_file("logs/blokboi_all.log", loguru::Append, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = 1;

    LOG_F(INFO, "Hello log!");
    DLOG_F(INFO, "Making a scene...");

    Scene earth = Scene(20, 30);
    earth.generate();
    string repr = earth.representation();
    cout << repr << endl;
    // LOG_F(INFO, earth.representation());
}