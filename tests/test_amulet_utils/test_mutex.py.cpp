#include <pybind11/pybind11.h>
#include <pybind11/typing.h>

#include <amulet/utils/threading/mutex.hpp>
#include <amulet/utils/threading/shared_mutex.hpp>

namespace py = pybind11;

#pragma optimize( "", off )

void test_mutex(){
    astd::mutex mtx;
    mtx.lock();
    mtx.unlock();
}

void test_lock_guard(){
    astd::mutex mtx;
    astd::lock_guard lock(mtx);
}

void test_unique_lock(){
    astd::mutex mtx;
    astd::lock_guard lock(mtx);
}

void test_shared_mutex(){
    astd::shared_mutex mtx;
    mtx.lock();
    mtx.unlock();
    mtx.lock_shared();
    mtx.unlock_shared();
}

void test_shared_lock(){
    astd::shared_mutex mtx;
    astd::shared_lock lock(mtx);
}

#pragma optimize( "", on )

static py::typing::List<py::typing::Callable<void()>> get_matrix_tests()
{
    py::list tests;

    tests.append(
        py::cpp_function(
            test_mutex,
            py::name("test_mutex")));

    tests.append(
        py::cpp_function(
            test_lock_guard,
            py::name("test_lock_guard")));

    tests.append(
        py::cpp_function(
            test_unique_lock,
            py::name("test_unique_lock")));

    test.append(
        py::cpp_function(
            test_shared_mutex,
            py::name("test_shared_mutex")));

    test.append(
        py::cpp_function(
            test_shared_lock,
            py::name("test_shared_lock")));

    return tests;
}

void init_test_mutex(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_mutex_");
    m.def("get_cpp_tests", &get_matrix_tests);
}
