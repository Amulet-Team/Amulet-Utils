#include <pybind11/pybind11.h>

#include <string>

#include <amulet/utils/logging/logging.hpp>

namespace py = pybind11;

static void logging_tests() {
    Amulet::debug("debug msg");
    Amulet::info("info msg");
    Amulet::warning("warning msg");
    Amulet::error("error msg");
    Amulet::critical("critical msg");

    for (int i = 0; i < 60; i += 5) {
        Amulet::log(i, std::to_string(i));
    }
}

void init_test_logging(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_logging_");
    m.def("logging_tests", &logging_tests);
}
