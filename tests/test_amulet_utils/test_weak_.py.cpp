#include <pybind11/pybind11.h>

#include <list>
#include <map>
#include <memory>

#include <amulet/test_utils/test_utils.hpp>
#include <amulet/utils/weak.hpp>

namespace py = pybind11;

static void test_weak_list()
{
    auto a = std::make_shared<int>(1);
    auto b = std::make_shared<int>(2);
    auto c = std::make_shared<int>(3);
    auto d = std::make_shared<int>(4);

    Amulet::WeakList<int> weak_list;
    weak_list.push_back(a);
    weak_list.push_back(b);
    weak_list.push_back(c);
    weak_list.push_back(d);
    weak_list.push_back(d);

    auto it = weak_list.begin();
    ASSERT_EQUAL(int, 1, *(it++)->lock())
    ASSERT_EQUAL(int, 2, *(it++)->lock())
    ASSERT_EQUAL(int, 3, *(it++)->lock())
    ASSERT_EQUAL(int, 4, *(it++)->lock())
    ASSERT_EQUAL(int, 4, *(it++)->lock())
    ASSERT_EQUAL(Amulet::WeakList<int>::iterator, weak_list.end(), it)

    ASSERT_EQUAL(size_t, 5, weak_list.size())

    b = nullptr;
    d = nullptr;
    std::list<int> values;
    Amulet::for_each(weak_list, [&values](int& value) { values.push_back(value); });
    std::list<int> expected_values { 1, 3 };
    ASSERT_EQUAL(std::list<int>, expected_values, values)

    ASSERT_EQUAL(size_t, 2, weak_list.size())
}

static void test_weak_set()
{
    auto item_1 = std::make_shared<int>(1);
    auto item_2 = std::make_shared<int>(2);
    auto item_3 = std::make_shared<int>(3);
    auto item_4 = std::make_shared<int>(3);

    Amulet::WeakSet<int> weak_set;
    weak_set.emplace(item_1);
    weak_set.emplace(item_2);
    weak_set.emplace(item_3);
    weak_set.emplace(item_4);
    weak_set.emplace(item_4);
    ASSERT_EQUAL(size_t, 4, weak_set.size())

    using CountMap = std::map<int, size_t>;
    
    CountMap expected_count_1 { { 1, 1 }, { 2, 1 }, { 3, 2 } };
    CountMap count_1;
    Amulet::for_each(weak_set, [&count_1](int& value) {
        auto it = count_1.find(value);
        if (it == count_1.end()) {
            it = count_1.emplace(value, 0).first;
        }
        it->second++;
    });
    ASSERT_EQUAL(CountMap, expected_count_1, count_1)
    ASSERT_EQUAL(size_t, 4, weak_set.size())

    item_2 = nullptr;
    item_3 = nullptr;
    CountMap expected_count_2 { { 1, 1 }, { 3, 1 } };
    CountMap count_2;
    Amulet::for_each(weak_set, [&count_2](int& value) {
        auto it = count_2.find(value);
        if (it == count_2.end()) {
            it = count_2.emplace(value, 0).first;
        }
        it->second++;
    });
    ASSERT_EQUAL(CountMap, expected_count_2, count_2)
    ASSERT_EQUAL(size_t, 2, weak_set.size())
}

void init_test_weak(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_weak_");
    m.def("test_weak_list", &test_weak_list);
    m.def("test_weak_set", &test_weak_set);
}
