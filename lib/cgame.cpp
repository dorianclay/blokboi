#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <cstdint>
#include "game.h"

PYBIND11_MODULE(blokboi, m)
{
    m.doc() = "Blokboi game";

    py::class_<Game>(m, "Game", py::buffer_protocol())
        .def(py::init())
        .def("newGame", &Game::newGame)
        .def("resetGame", &Game::resetGame)
        .def("move", &Game::move)
        .def("jump", &Game::jump)
        .def("pick_up", &Game::pick_up)
        .def("put_down", &Game::put_down)
        .def("width", &Game::width)
        .def("height", &Game::height)
        // // TODO: Make the <vector<vector: GameObject>> an opaque type
        // .def("scene_space", &Game::scene_space)
        // // TODO (END)
        .def("representation", &Game::representation)
        .def("objective", &Game::objective)
        .def("__repr__", [](Game &a) { return a.representation(); })
        .def_buffer([](Game &g) -> py::buffer_info {
            return py::buffer_info(
                g.data(),
                sizeof(char),
                py::format_descriptor<char>::format(),
                3,
                { g.width(), g.height() },
                { sizeof(char) * g.height(), sizeof(char)}
            );
        });
}