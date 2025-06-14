#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

#include <amulet/utils/dll.hpp>
#include <amulet/utils/logging/logging.hpp>
#include <amulet/utils/weak.hpp>

namespace Amulet {

enum class ConnectionMode {
    Direct, // Directly called by the emitter.
    Async, // Called asynchronously.
};

namespace detail {
    AMULET_UTILS_EXPORT void submit_async(std::function<void()> event);

    template <typename... Args>
    class SignalCallbackStorage {
    public:
        std::recursive_mutex mutex;
        std::function<void(Args...)> callback;
        ConnectionMode mode;
        bool disconnected = false;

        SignalCallbackStorage(
            std::function<void(Args...)> callback,
            ConnectionMode mode)
            : callback(std::move(callback))
            , mode(mode)
        {
        }
    };

} // namespace detail

template <typename... Args>
class Signal;

// A token returned when connecting a callback to a signal.
// The token must be kept alive and used to disconnect the callback when it is no longer needed.
template <typename... Args>
class SignalToken {
private:
    std::shared_ptr<detail::SignalCallbackStorage<Args...>> storage;

    // Constructor.
    SignalToken(std::shared_ptr<detail::SignalCallbackStorage<Args...>> storage)
        : storage(storage)
    {
    }

    // Allow Signal to construct SignalToken.
    friend class Signal<Args...>;

public:
    // Default constructor.
    SignalToken() = default;
};

template <typename... Args>
class Signal {
private:
    using storageT = detail::SignalCallbackStorage<Args...>;

    std::mutex _mutex;
    WeakSet<storageT> _callbacks;

public:
    // The callback type for this signal.
    using callbackT = std::function<void(Args...)>;

    // The token type for this signal.
    using tokenT = SignalToken<Args...>;

    // Constructors.
    Signal() = default;
    Signal(const Signal&) = delete;
    Signal(Signal&&) = delete;

    // Connect a callback to this signal and return a token.
    // The token must be kept alive for the callback to work.
    // The token is used to disconnect the callback when it is not needed.
    // Thread safe.
    tokenT connect(callbackT callback, ConnectionMode mode = ConnectionMode::Direct)
    {
        std::lock_guard lock(_mutex);
        auto storage = std::make_shared<storageT>(std::move(callback), mode);
        _callbacks.emplace(storage);
        return storage;
    }

    // Disconnect a callback.
    // Token is the value returned by connect.
    // Thread safe.
    void disconnect(const tokenT& token)
    {
        if (!token.storage) {
            return;
        }
        std::lock_guard lock(_mutex);
        std::lock_guard storage_lock(token.storage->mutex);
        token.storage->disconnected = true;
        _callbacks.erase(token.storage);
    }

    // Call all callbacks with the given arguments from this thread.
    // Blocks until all callbacks are processed.
    // Thread safe.
    void emit(Args... args)
    {
        AmuletLog(5, "emit");
        WeakSet<storageT> temp_callbacks;
        {
            // Copy callbacks
            std::lock_guard lock(_mutex);
            temp_callbacks = _callbacks;
        }

        // Storage elements that were destroyed.
        WeakList<storageT> null_storage;

        std::shared_ptr<std::tuple<Args...>> async_args;

        AmuletLog(5, "calling " + std::to_string(temp_callbacks.size()) + " callbacks");
        for (const auto& ptr : temp_callbacks) {
            auto storage = ptr.lock();
            if (!storage) {
                AmuletLog(5, "skipping destroyed callback");
                // The token was destroyed before calling disconnect.
                null_storage.emplace_back(ptr);
                continue;
            }
            switch (storage->mode) {
            case ConnectionMode::Direct: {
                AmuletLog(5, "calling direct");
                std::lock_guard storage_lock(storage->mutex);
                if (storage->disconnected) {
                    // The callback was disconnected between getting the callback and processing it.
                    continue;
                }
                try {
                    storage->callback(args...);
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
                detail::submit_async([async_args, ptr]() {
                    auto storage = ptr.lock();
                    if (!storage) {
                        return;
                    }
                    std::lock_guard storage_lock(storage->mutex);
                    if (storage->disconnected) {
                        // The callback was disconnected between getting the callback and processing it.
                        return;
                    }
                    try {
                        std::apply(storage->callback, *async_args);
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
            std::lock_guard lock(_mutex);
            for (const auto& ptr : null_storage) {
                _callbacks.erase(ptr);
            }
        }
    }

    // Destructor.
    ~Signal()
    {
        std::lock_guard lock(_mutex);
        for (const auto& ptr : _callbacks) {
            auto storage = ptr.lock();
            if (storage) {
                std::lock_guard storage_lock(storage->mutex);
                storage->disconnected = true;
            }
        }
    }
};

} // namespace Amulet
