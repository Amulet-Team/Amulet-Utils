#include <pybind11/pybind11.h>

#include <map>
#include <memory>
#include <stdexcept>

#include <amulet/utils/view/map_view.hpp>

#include <amulet/test_utils/test_utils.hpp>

namespace py = pybind11;

static void test_view()
{
    using View = Amulet::MapView<std::map<int, std::shared_ptr<int>>, std::shared_ptr<const int>>;
    using It = View::const_iterator;

    std::map<int, std::shared_ptr<int>> m0;
    View view_0(m0);

    std::map<int, std::shared_ptr<int>> m3;
    m3.emplace(1, std::make_shared<int>(2));
    m3.emplace(3, std::make_shared<int>(4));
    m3.emplace(5, std::make_shared<int>(6));
    View view_3(m3);

    // at
    ASSERT_EQUAL(int, 2, *view_3.at(1));
    ASSERT_EQUAL(int, 4, *view_3.at(3));
    ASSERT_EQUAL(int, 6, *view_3.at(5));
    ASSERT_RAISES(std::out_of_range, view_3.at(0));

    // iteration
    ASSERT_EQUAL(It, view_0.end(), view_0.begin());

    auto it = view_3.begin();

    ASSERT_NOT_EQUAL(It, view_3.end(), it);
    ASSERT_EQUAL(int, 1, it->first);
    ASSERT_EQUAL(int, 2, *it->second);

    auto it2 = ++it;
    ASSERT_NOT_EQUAL(It, view_3.end(), it);
    auto item = *it;
    ASSERT_EQUAL(int, 3, item.first);
    ASSERT_EQUAL(int, 4, *item.second);

    auto it3 = it++;
    ASSERT_NOT_EQUAL(It, view_3.end(), it);
    ASSERT_EQUAL(int, 5, it->first);
    ASSERT_EQUAL(int, 6, *it->second);

    it++;
    ASSERT_EQUAL(It, view_3.end(), it);

    ASSERT_EQUAL(It, it2, it3);
    ASSERT_EQUAL(int, 3, it2->first);
    ASSERT_EQUAL(int, 4, *it2->second);

    // empty
    ASSERT_TRUE(view_0.empty());
    ASSERT_FALSE(view_3.empty());

    // size
    ASSERT_EQUAL(size_t, 0, view_0.size());
    ASSERT_EQUAL(size_t, 3, view_3.size());

    // max_size
    ASSERT_LESS(size_t, 1'000'000, view_0.max_size());
    ASSERT_LESS(size_t, 1'000'000, view_3.max_size());

    // count
    ASSERT_EQUAL(size_t, 0, view_3.count(0));
    ASSERT_EQUAL(size_t, 1, view_3.count(1));
    ASSERT_EQUAL(size_t, 0, view_3.count(2));
    ASSERT_EQUAL(size_t, 1, view_3.count(3));
    ASSERT_EQUAL(size_t, 0, view_3.count(4));
    ASSERT_EQUAL(size_t, 1, view_3.count(5));
    ASSERT_EQUAL(size_t, 0, view_3.count(6));

    // find
    auto find_it_0 = view_3.find(0);
    ASSERT_EQUAL(It, view_3.end(), find_it_0);
    auto find_it_1 = view_3.find(1);
    ASSERT_NOT_EQUAL(It, view_3.end(), find_it_1);
    ASSERT_EQUAL(int, 1, find_it_1->first);
    ASSERT_EQUAL(int, 2, *find_it_1->second);

    // contains
    ASSERT_FALSE(view_3.contains(0));
    ASSERT_TRUE(view_3.contains(1));
    ASSERT_FALSE(view_3.contains(2));
    ASSERT_TRUE(view_3.contains(3));
    ASSERT_FALSE(view_3.contains(4));
    ASSERT_TRUE(view_3.contains(5));
    ASSERT_FALSE(view_3.contains(6));
}

void init_test_view(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_view_");
    m.def("test_map_view", &test_view);
}
