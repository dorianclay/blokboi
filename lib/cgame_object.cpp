#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "block.h"
#include "game_object.h"

using namespace std;

PYBIND11_MODULE(game_object, m)
{
    m.doc() = "Blokboi game object";

    py::class_<location>(m, "location")
        .def_readonly("x", &location::x)
        .def_readonly("y", &location::y)
        .def("__repr__", [](location &a) { return "<location (" + to_string(a.x) + ", " + to_string(a.y) + ")>"; });

    py::class_<GameObject>(m, "GameObject")
        // .def("kind", &Block::kind)
        // .def("update", &Block::update)
        .def("location", &Block::location)
        .def("movable", &Block::movable)
        .def("__repr__", [](GameObject &a) {
            return "<game_object at (" + to_string(a.location().x) + ", " + to_string(a.location().y) + ")>";
        });

    py::class_<Block, GameObject>(m, "Block")
        .def(py::init())
        .def(py::init<int, int>())
        // .def(py::init<int, int, Color, int, bool>())
        .def("color", &Block::color)
        .def("number", &Block::number)
        .def("__repr__", [](Block &a) {
            return "<game_object.Block at (" + to_string(a.location().x) + ", " + to_string(a.location().y) +
                   ") with color " + to_string(a.color()) + " and number " + to_string(a.number()) + ">";
        });

    py::class_<Ground, GameObject>(m, "Ground").def(py::init<int, int>()).def("__repr__", [](Ground &a) {
        return "<game_object.Ground at (" + to_string(a.location().x) + ", " + to_string(a.location().y) + ")>";
    });
}
