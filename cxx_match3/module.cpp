
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.h"

namespace py = pybind11;

//PYBIND11_MAKE_OPAQUE(std::vector<unsigned int>);
PYBIND11_MODULE(module_name, m){
    
    py::class_<Game>(m, "Game")
        .def(py::init<unsigned short, unsigned short, unsigned short, bool,
                  unsigned short, bool ,
                  unsigned short, bool ,
                  unsigned short, unsigned short , unsigned int,
                  int, short, short, bool,
                  std::string, bool>())

        .def("start", &Game::start)
        .def("reset", &Game::reset)
        .def("apply_action" ,&Game::apply_action)    
        .def("legal_actions",&Game::legal_actions)
        .def("is_terminal", &Game::is_terminal) 
        .def_property_readonly("simulation_apply_action", &Game::simulation_apply_action)
        .def_property_readonly("current_player", &Game::current_player)
        .def_property_readonly("rewards", &Game::rewards)
        .def_property_readonly("observation_tensor", &Game::observation_tensor)
        .def_property_readonly("to_string", &Game::to_string);
}




