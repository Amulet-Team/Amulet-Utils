#include <pybind11/pybind11.h>

#include <amulet/utils/bytes.hpp>
#include <amulet/utils/bytes.py.hpp>

#include <amulet/test_utils/test_utils.hpp>

namespace py = pybind11;

static void test_cpp()
{
    std::string src_1 = "test1";
    Amulet::Bytes copy_string = src_1;
    ASSERT_EQUAL(std::string, "test1", src_1);
    ASSERT_EQUAL(Amulet::Bytes, "test1", copy_string);

    std::string src_2 = "test2";
    Amulet::Bytes move_string = std::move(src_2);
    ASSERT_EQUAL(std::string, "", src_2);
    ASSERT_EQUAL(Amulet::Bytes, "test2", move_string);

    Amulet::Bytes src_3 = "test3";
    Amulet::Bytes copy_bytes = src_3;
    ASSERT_EQUAL(std::string, "test3", src_3);
    ASSERT_EQUAL(Amulet::Bytes, "test3", copy_bytes);

    Amulet::Bytes src_4 = "test4";
    Amulet::Bytes move_bytes = std::move(src_4);
    ASSERT_EQUAL(std::string, "", src_4);
    ASSERT_EQUAL(Amulet::Bytes, "test4", move_bytes);

    std::string src_5 = "test5";
    copy_string = src_5;
    ASSERT_EQUAL(std::string, "test5", src_5);
    ASSERT_EQUAL(Amulet::Bytes, "test5", copy_string);

    std::string src_6 = "test6";
    move_string = std::move(src_6);
    ASSERT_EQUAL(std::string, "", src_6);
    ASSERT_EQUAL(Amulet::Bytes, "test6", move_string);

    Amulet::Bytes src_7 = "test7";
    copy_bytes = src_7;
    ASSERT_EQUAL(std::string, "test7", src_7);
    ASSERT_EQUAL(Amulet::Bytes, "test7", copy_bytes);

    Amulet::Bytes src_8 = "test8";
    move_bytes = std::move(src_8);
    ASSERT_EQUAL(std::string, "", src_8);
    ASSERT_EQUAL(Amulet::Bytes, "test8", move_bytes);
}

void init_test_bytes(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_bytes_");

    m.def("test_cpp", &test_cpp);
    m.def("bytes_passthrough", [](const Amulet::Bytes& bytes) { return bytes; });
    m.def("get_bytes", []() { return Amulet::Bytes("Hello World"); });
    m.def("get_len", [](const Amulet::Bytes& bytes) { return bytes.size(); });
}
