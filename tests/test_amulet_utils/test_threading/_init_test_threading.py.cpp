#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;

void init_test_lock(py::module);
void init_test_mutex(py::module);

void init_test_threading(py::module m_parent){
    auto m = Amulet::pybind11_extensions::def_subpackage(m_parent, "test_threading");
    init_test_mutex(m);
    init_test_lock(m);
}
