#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_test_lock(py::module);
void init_test_mutex(py::module);

void init_test_threading(py::module m_parent){
    auto m = m_parent.def_submodule("test_threading");
    init_test_mutex(m);
    init_test_lock(m);
}
