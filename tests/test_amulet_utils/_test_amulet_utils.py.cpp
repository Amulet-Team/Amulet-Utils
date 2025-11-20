#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/compatibility.hpp>

namespace py = pybind11;
namespace pyext = Amulet::pybind11_extensions;

void init_test_weak(py::module);
void init_test_event(py::module);
void init_test_logging(py::module);
void init_test_task_manager(py::module);
void init_test_lock(py::module);
void init_test_lock_file(py::module);
void init_test_temp(py::module);
void init_test_matrix(py::module);
void init_test_bytes(py::module);
void init_test_view(py::module);

void init_module(py::module m){
    pyext::init_compiler_config(m);
    pyext::check_compatibility(py::module::import("amulet.utils"), m);

    init_test_weak(m);
    init_test_event(m);
    init_test_logging(m);
    init_test_task_manager(m);
    init_test_lock(m);
    init_test_lock_file(m);
    init_test_temp(m);
    init_test_matrix(m);
    init_test_bytes(m);
    init_test_view(m);
}

PYBIND11_MODULE(_test_amulet_utils, m) {
    m.def("init", &init_module, py::arg("m"));
}
