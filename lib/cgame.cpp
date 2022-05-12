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
      .def(py::init<Char3d, std::string, std::string, Int2d, Int2d>())
      .def("width", &Game::width)
      .def("height", &Game::height)
      .def("array", &Game::array)
      .def("init_data", &Game::init_data)
      .def("representation", &Game::representation)
      .def("objective", &Game::objective)
      .def("relationship", &Game::relationship)
      .def("init_coords", &Game::init_coords)
      .def("feature_mask", &Game::feature_mask)
      .def("success", &Game::success)
      .def("steps_taken", &Game::steps_taken)
      .def("reward", &Game::reward)
      .def("set_objective", &Game::set_objective)
      .def("newGame", &Game::newGame)
      .def("resetGame", &Game::resetGame)
      .def("run_heuristic", &Game::run_heuristic)
      .def("move", &Game::move)
      .def("jump", &Game::jump)
      .def("toggle_hold", &Game::toggle_hold)
      .def("pick_up", &Game::pick_up)
      .def("put_down", &Game::put_down)
      .def("verify", &Game::verify)
      .def("score", &Game::score)
      .def("__repr__", [](Game &a) { return a.representation(); });
}
