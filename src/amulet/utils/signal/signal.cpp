#include <cstdlib>
#include <iostream>

#include <amulet/utils/logging/logging.hpp>

#include "signal.hpp"

namespace Amulet {

namespace detail {

    EventLoop::EventLoop()
        : _thread(&EventLoop::_event_loop, this)
    {
    }

    EventLoop::~EventLoop()
    {
        if (get_min_log_level() <= 10) {
            std::cout << "EventLoop::~EventLoop() enter" << std::endl;
        }
        exit();
    }

    void EventLoop::exit()
    {
        if (get_min_log_level() <= 10) {
            std::cout << "EventLoop::exit()" << std::endl;
        }
        {
            std::unique_lock lock(_mutex);
            if (_exit) {
                return;
            }
            _exit = true;
            _condition.notify_one();
        }
        if (get_min_log_level() <= 10) {
            std::cout << "EventLoop::exit() join" << std::endl;
        }
        _thread.join();
        if (get_min_log_level() <= 10) {
            std::cout << "EventLoop::exit() exit" << std::endl;
        }
    }

    void EventLoop::_event_loop()
    {
        {
            std::unique_lock lock(_mutex);
            while (!_exit) {
                if (_events.empty()) {
                    // If there are no events to process, wait until more are added.
                    _condition.wait(lock);
                    // Re-check the exit condition.
                    continue;
                }
                auto event = _events.front();
                _events.pop_front();
                lock.unlock();
                try {
                    event();
                } catch (const std::exception& e) {
                    Amulet::error(std::string("Unhandled exception in event loop: ") + e.what());
                } catch (...) {
                    Amulet::error("Unhandled exception in event loop.");
                }
                lock.lock();
            }
        }
        if (get_min_log_level() <= 10) {
            std::cout << "Exiting event loop" << std::endl;
        }
    }

    void EventLoop::submit(std::function<void()> event)
    {
        std::unique_lock lock(_mutex);
        _events.push_back(event);
        _condition.notify_one();
    }

    EventLoop& get_global_event_loop()
    {
        static EventLoop global_event_loop;
        static int global_event_loop_atexit_registered = std::atexit([] { global_event_loop.exit(); });
        return global_event_loop;
    }

} // namespace detail

} // namespace Amulet
