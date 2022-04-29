#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "game.h"
#include <cstdint>
#include <string>

PYBIND11_MODULE(blokboi, m) {
  m.doc() = "Blokboi game";

  py::class_<Game>(m, "Game")
      .def(py::init())
      .def(py::init<Char3d>())
      .def(py::init<Char3d, std::string, Int2d, std::string>())
      .def("newGame", &Game::newGame)
      .def("resetGame", &Game::resetGame)
      .def("run_heuristic", &Game::run_heuristic)
      .def("set_objective", &Game::set_objective)
      .def("move", &Game::move)
      .def("jump", &Game::jump)
      .def("toggle_hold", &Game::toggle_hold)
      .def("pick_up", &Game::pick_up)
      .def("put_down", &Game::put_down)
      .def("width", &Game::width)
      .def("height", &Game::height)
      .def("array", &Game::array)
      .def("representation", &Game::representation)
      .def("objective", &Game::objective)
      .def("__repr__", [](Game &a) { return a.representation(); });
}
