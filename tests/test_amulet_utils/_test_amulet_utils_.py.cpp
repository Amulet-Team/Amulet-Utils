#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_test_weak(py::module);
void init_test_event(py::module);
void init_test_logging(py::module);
void init_test_task_manager(py::module);
void init_test_lock(py::module);
void init_test_lock_file(py::module);
void init_test_temp(py::module);
void init_test_matrix(py::module);
void init_test_mutex(py::module);
void init_test_bytes(py::module);
void init_test_view(py::module);

void init_test_amulet_utils(py::module m){
    init_test_weak(m);
    init_test_event(m);
    init_test_logging(m);
    init_test_task_manager(m);
    init_test_lock(m);
    init_test_lock_file(m);
    init_test_temp(m);
    init_test_matrix(m);
    init_test_mutex(m);
    init_test_bytes(m);
    init_test_view(m);
}
