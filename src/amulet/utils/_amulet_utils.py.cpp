#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/compatibility.hpp>

namespace py = pybind11;
namespace pyext = Amulet::pybind11_extensions;

void init_amulet_utils(py::module);

static void _init_amulet_utils(py::module m)
{
    pyext::init_compiler_config(m);
    init_amulet_utils(m);
}

PYBIND11_MODULE(_amulet_utils, m)
{
    m.def("init", &_init_amulet_utils, py::arg("m"));
}
