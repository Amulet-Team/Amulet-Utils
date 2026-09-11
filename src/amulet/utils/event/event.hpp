#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <amulet/utils/dll.hpp>
#include <amulet/utils/logging/logging.hpp>
#include <amulet/utils/weak.hpp>
#include <amulet/utils/threading/thread_safety.hpp>
#include <amulet/utils/threading/mutex.hpp>

namespace Amulet {

enum class ConnectionMode {
    Direct, // Directly called by the emitter.
    Async, // Called asynchronously.
};

namespace detail {
    AMULET_UTILS_EXPORT void submit_async(std::function<void()> event);

    template <typename... Args>
    class EventCallbackStorage {
    public:
        astd::recursive_mutex mutex;
        std::function<void(Args...)> callback ASTD_GUARDED_BY(mutex);
        const ConnectionMode mode;
        bool disconnected ASTD_GUARDED_BY(mutex) = false;

        EventCallbackStorage(
            std::function<void(Args...)> callback,
            ConnectionMode mode)
            : callback(std::move(callback))
            , mode(mode)
        {
        }
    };

} // namespace detail

template <typename... Args>
class Event;

// A token returned when connecting a callback to an event.
// The token must be kept alive and used to disconnect the callback when it is no longer needed.
template <typename... Args>
class EventToken {
private:
    std::shared_ptr<detail::EventCallbackStorage<Args...>> storage_ptr;

    // Constructor.
    EventToken(std::shared_ptr<detail::EventCallbackStorage<Args...>> storage_ptr)
        : storage_ptr(std::move(storage_ptr))
    {
    }

    // Allow Event to construct EventToken.
    friend class Event<Args...>;

public:
    // Default constructor.
    EventToken() = default;
};

template <typename... Args>
class Event {
private:
    using storageT = detail::EventCallbackStorage<Args...>;

    astd::mutex _mutex;
    WeakSet<storageT> _callbacks ASTD_GUARDED_BY(_mutex);

public:
    // The callback type for this event.
    using callbackT = std::function<void(Args...)>;

    // The token type for this event.
    using tokenT = EventToken<Args...>;

    // Constructors.
    Event() = default;
    Event(const Event&) = delete;
    Event(Event&&) = delete;

    // Connect a callback to this event and return a token.
    // The token must be kept alive for the callback to work.
    // The token is used to disconnect the callback when it is not needed.
    // Thread safe.
    tokenT connect(callbackT callback, ConnectionMode mode = ConnectionMode::Direct) ASTD_EXCLUDES_ALL(_mutex)
    {
        astd::lock_guard lock(_mutex);
        auto storage = std::make_shared<storageT>(std::move(callback), mode);
        _callbacks.emplace(storage);
        return storage;
    }

    // Disconnect a callback.
    // Token is the value returned by connect.
    // Thread safe.
    void disconnect(const tokenT& token) ASTD_EXCLUDES_ALL(_mutex)
    {
        if (!token.storage_ptr) {
            return;
        }
        astd::lock_guard lock(_mutex);
        auto& storage = *token.storage_ptr;
        astd::lock_guard storage_lock(storage.mutex);
        storage.disconnected = true;
        _callbacks.erase(token.storage_ptr);
    }

    // Call all callbacks with the given arguments from this thread.
    // Blocks until all callbacks are processed.
    // Thread safe.
    void dispatch(Args... args) ASTD_EXCLUDES_ALL(_mutex)
    {
        AmuletLog(5, "dispatch");
        WeakSet<storageT> temp_callbacks;
        {
            // Copy callbacks
            astd::lock_guard lock(_mutex);
            temp_callbacks = _callbacks;
        }

        // Storage elements that were destroyed.
        WeakList<storageT> null_storage;

        std::shared_ptr<std::tuple<Args...>> async_args;

        AmuletLog(5, "calling " + std::to_string(temp_callbacks.size()) + " callbacks");
        for (const auto& storage_weak_ptr : temp_callbacks) {
            auto storage_ptr = storage_weak_ptr.lock();
            if (!storage_ptr) {
                AmuletLog(5, "skipping destroyed callback");
                // The token was destroyed before calling disconnect.
                null_storage.emplace_back(storage_weak_ptr);
                continue;
            }
            auto& storage = *storage_ptr;
            switch (storage.mode) {
            case ConnectionMode::Direct: {
                AmuletLog(5, "calling direct");
                astd::lock_guard storage_lock(storage.mutex);
                if (storage.disconnected) {
                    // The callback was disconnected between getting the callback and processing it.
                    continue;
                }
                try {
                    storage.callback(args...);
                } catch (const std::exception& e) {
                    AmuletLog(40, "Error in callback: " << e.what());
                } catch (...) {
                    AmuletLog(40, "Error in callback.");
                }
            } break;
            case ConnectionMode::Async: {
                AmuletLog(5, "calling async");
                if (!async_args) {
                    async_args = std::make_shared<std::tuple<Args...>>(args...);
                }
                detail::submit_async([async_args, storage_weak_ptr]() {
                    auto storage_ptr = storage_weak_ptr.lock();
                    if (!storage_ptr) {
                        return;
                    }
                    auto& storage = *storage_ptr;
                    astd::lock_guard storage_lock(storage.mutex);
                    if (storage.disconnected) {
                        // The callback was disconnected between getting the callback and processing it.
                        return;
                    }
                    try {
                        std::apply(storage.callback, *async_args);
                    } catch (const std::exception& e) {
                        AmuletLog(40, "Error in async callback: " << e.what());
                    } catch (...) {
                        AmuletLog(40, "Error in async callback.");
                    }
                });
            } break;
            }
        }

        if (!null_storage.empty()) {
            // Remove null storage pointers.
            astd::lock_guard lock(_mutex);
            for (const auto& storage_weak_ptr : null_storage) {
                _callbacks.erase(storage_weak_ptr);
            }
        }
    }

    // Destructor.
    ~Event()
    {
        astd::lock_guard lock(_mutex);
        for (const auto& storage_weak_ptr : _callbacks) {
            auto storage_ptr = storage_weak_ptr.lock();
            if (storage_ptr) {
                auto& storage = *storage_ptr;
                astd::lock_guard storage_lock(storage.mutex);
                storage.disconnected = true;
            }
        }
    }
};

} // namespace Amulet
