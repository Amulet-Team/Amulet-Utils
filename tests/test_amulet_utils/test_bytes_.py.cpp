#include <pybind11/pybind11.h>

#include <amulet/utils/bytes.hpp>
#include <amulet/utils/bytes.py.hpp>

namespace py = pybind11;

void init_test_bytes(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_bytes_");

    m.def("bytes_passthrough", [](const Amulet::Bytes& bytes) { return bytes; });
    m.def("get_bytes", []() { return Amulet::Bytes("Hello World"); });
    m.def("get_len", [](const Amulet::Bytes& bytes) { return bytes.size(); });
}
