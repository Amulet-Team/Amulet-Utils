#include <pybind11/pybind11.h>

#include <filesystem>

#include <amulet/test_utils/test_utils.hpp>
#include <amulet/utils/temp.hpp>

namespace py = pybind11;

static void test_temp()
{
    std::filesystem::path path;
    {
        Amulet::TempDir temp_dir("helloworld");
        path = temp_dir.get_path();
        if (!std::filesystem::is_directory(path)) {
            throw std::runtime_error("Directory does not exist.");
        }
        if (path.parent_path().filename() != "helloworld") {
            throw std::runtime_error("Incorrect path");
        }
    }
    if (std::filesystem::is_directory(path)) {
        throw std::runtime_error("Directory was not deleted.");
    }
}

void init_test_temp(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_temp_");
    m.def("test_temp", &test_temp);
}
