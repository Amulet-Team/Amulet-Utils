#include <pybind11/pybind11.h>

#include <amulet/utils/python.hpp>

namespace py = pybind11;

void init_numpy(py::module);
void init_event(py::module);
void init_task_manager(py::module);
void init_lock(py::module);
void init_logging(py::module);
void init_image(py::module);
void init_matrix(py::module);
void init_temp(py::module);

void init_amulet_utils(py::module m)
{
    auto py_valid = Amulet::get_py_valid();
    *py_valid = true;
    py::module::import("atexit").attr("register")(
        py::cpp_function([py_valid]() { *py_valid = false; }));

    // Submodules
    init_numpy(m);
    init_event(m);
    init_task_manager(m);
    init_lock(m);
    init_logging(m);
    init_image(m);
    init_matrix(m);
    init_temp(m);
}
