#include <cstdlib>
#include <iostream>

#include <amulet/utils/logging/logging.hpp>

#include "signal.hpp"

namespace Amulet {

namespace detail {

    EventLoop::EventLoop()
        : _thread(&EventLoop::_event_loop, this)
    {
        // This class may call debug during shutdown.
        // Ensure the logger outlives the EventLoop.
        Amulet::get_logger();
    }

    EventLoop::~EventLoop()
    {
        std::cout << "EventLoop::~EventLoop() enter" << std::endl;
        debug("EventLoop::~EventLoop() enter");
        exit();
    }

    void EventLoop::exit()
    {
        std::cout << "EventLoop::exit()" << std::endl;
        debug("EventLoop::exit()");
        {
            std::unique_lock lock(_mutex);
            if (_exit) {
                return;
            }
            _exit = true;
            _condition.notify_one();
        }
        std::cout << "EventLoop::exit() join" << std::endl;
        debug("EventLoop::exit() join");
        _thread.join();
        std::cout << "EventLoop::exit() exit" << std::endl;
        debug("EventLoop::exit() exit");
    }

    void EventLoop::_event_loop()
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
                std::cout << std::string("Unhandled exception in event loop: ") + e.what() << std::endl;
                Amulet::error(std::string("Unhandled exception in event loop: ") + e.what());
            } catch (...) {
                std::cout << "Unhandled exception in event loop." << std::endl;
                Amulet::error("Unhandled exception in event loop.");
            }
            lock.lock();
        }
        std::cout << "EventLoop::_event_loop() exit" << std::endl;
        debug("EventLoop::_event_loop() exit");
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
