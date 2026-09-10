#include <cstdlib>
#include <functional>
#include <list>
#include <thread>

#include <amulet/utils/logging/logging.hpp>
#include <amulet/utils/threading/condition_variable.hpp>
#include <amulet/utils/threading/mutex.hpp>
#include <amulet/utils/threading/thread_safety.hpp>

#include "event.hpp"

namespace Amulet {

namespace {

    class EventLoop {
    private:
        astd::mutex _mutex;
        astd::condition_variable _condition;
        std::thread _thread;
        std::list<std::function<void()>> _events ASTD_GUARDED_BY(_mutex);
        bool _exit ASTD_GUARDED_BY(_mutex) = false;

        void _event_loop();

    public:
        // Construct a new event loop.
        EventLoop();

        // Destroy the event loop.
        ~EventLoop();

        // Exit out of the event loop.
        void exit();

        // Submit a new job to the event loop.
        void submit(std::function<void()> event);
    };

    EventLoop::EventLoop()
        : _thread(&EventLoop::_event_loop, this)
    {
        // This class may call debug during shutdown.
        // Ensure the logger outlives the EventLoop.
        Amulet::get_logger();
    }

    EventLoop::~EventLoop()
    {
        debug("EventLoop::~EventLoop() enter");
        exit();
    }

    void EventLoop::exit() ASTD_EXCLUDES(_mutex)
    {
        debug("EventLoop::exit()");
        {
            astd::lock_guard lock(_mutex);
            if (_exit) {
                return;
            }
            _exit = true;
        }
        _condition.notify_one();
        debug("EventLoop::exit() join");
        _thread.join();
        debug("EventLoop::exit() exit");
    }

    void EventLoop::_event_loop() ASTD_EXCLUDES(_mutex)
    {
        std::function<void()> event;
        while (!_exit) {
            {
                astd::unique_lock lock(_mutex);
                if (_events.empty()) {
                    // If there are no events to process, wait until more are added.
                    _condition.wait(lock);
                    // Re-check the exit condition.
                    continue;
                }
                event = std::move(_events.front());
                _events.pop_front();
            }
            try {
                event();
            } catch (const std::exception& e) {
                Amulet::error(std::string("Unhandled exception in event loop: ") + e.what());
            } catch (...) {
                Amulet::error("Unhandled exception in event loop.");
            }
        }
        debug("EventLoop::_event_loop() exit");
    }

    void EventLoop::submit(std::function<void()> event) ASTD_EXCLUDES(_mutex)
    {
        {
            astd::lock_guard lock(_mutex);
            _events.push_back(std::move(event));
        }
        _condition.notify_one();
    }

    EventLoop& get_global_event_loop()
    {
        static EventLoop global_event_loop;
        static int global_event_loop_atexit_registered = std::atexit([] { global_event_loop.exit(); });
        return global_event_loop;
    }

} // namespace

namespace detail {

    void submit_async(std::function<void()> event)
    {
        get_global_event_loop().submit(std::move(event));
    }

} // namespace detail
} // namespace Amulet
