#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "game.h"

PYBIND11_MODULE(blokboi, m) {
    m.doc() = "Blokboi game";

    py::class_<Game>(m, "Game")
        .def(py::init())
        .def("newGame", &Game::newGame)
        .def("resetGame", &Game::resetGame)
        .def("move", &Game::move)
        .def("jump", &Game::jump)
        .def("pick_up", &Game::pick_up)
        .def("put_down", &Game::put_down)
        .def("representation", &Game::representation)
        .def("objective", &Game::objective)
        .def("__repr__",
            [](const Game &a) {
                return a.representation();
            }
        );
}