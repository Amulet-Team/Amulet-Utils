#include <pybind11/pybind11.h>

#include <string>

#include <amulet/utils/event/event.hpp>
#include <amulet/utils/event/event.py.hpp>

namespace py = pybind11;

namespace AmuletTest {

class EventTest {
public:
    Amulet::Event<> event_0;
    Amulet::Event<int> event_1;
    Amulet::Event<int, float> event_2;
    Amulet::Event<int, float, std::string, int> event_3;

    void dispatch()
    {
        event_0.dispatch();
        event_1.dispatch(1);
        event_2.dispatch(1, 1.5);
        event_3.dispatch(1, 1.5, "Hello World", 2);
    }
};

} // namespace AmuletTest

void init_test_event(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_event_");

    py::class_<AmuletTest::EventTest> EventTest(m, "EventTest");
    EventTest.def(py::init<>());
    Amulet::def_event(EventTest, "event_0", &AmuletTest::EventTest::event_0);
    Amulet::def_event(EventTest, "event_1", &AmuletTest::EventTest::event_1);
    Amulet::def_event(EventTest, "event_2", &AmuletTest::EventTest::event_2);
    Amulet::def_event(EventTest, "event_3", &AmuletTest::EventTest::event_3);
    EventTest.def("dispatch", &AmuletTest::EventTest::dispatch);
}
