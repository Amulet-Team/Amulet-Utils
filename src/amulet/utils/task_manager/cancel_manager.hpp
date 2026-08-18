#pragma once

#include <functional>
#include <list>
#include <memory>
#include <stdexcept>

#include <amulet/utils/dll.hpp>
#include <amulet/utils/event/event.hpp>
#include <amulet/utils/threading/mutex.hpp>
#include <amulet/utils/threading/thread_safety.hpp>

namespace Amulet {

// Exception to be raised by the callee when a task is cancelled.
class AMULET_UTILS_EXPORT TaskCancelled : public std::exception {
private:
    std::string msg;

public:
    // Constructors
    explicit TaskCancelled(const std::string& msg);
    TaskCancelled();
    const char* what() const noexcept override;
    ~TaskCancelled() noexcept override;
};

using CancelCallback = std::function<void()>;

class AbstractCancelManager {
public:
    virtual ~AbstractCancelManager() = default;

    // Request the operation be cancelled.
    // It is down to the operation to implement support for this.
    // Thread safe.
    virtual void cancel() = 0;

    // Has cancel been called to signal that the operation should be cancelled.
    // Thread safe.
    virtual bool is_cancel_requested() = 0;

    // Register a function to get called when cancel is called.
    // The callback will be called from the thread `cancel` is called in.
    // Thread safe.
    virtual EventToken<> register_cancel_callback(CancelCallback callback) = 0;

    // Unregister a registered function from being called when cancel is called.
    // Thread safe.
    virtual void unregister_cancel_callback(EventToken<> token) = 0;
};

class AMULET_UTILS_EXPORT VoidCancelManager : public AbstractCancelManager {
public:
    VoidCancelManager();

    VoidCancelManager(const VoidCancelManager&);
    VoidCancelManager(VoidCancelManager&&);
    VoidCancelManager& operator=(const VoidCancelManager&);
    VoidCancelManager& operator=(VoidCancelManager&&);

    ~VoidCancelManager() override;

    void cancel() override;
    bool is_cancel_requested() override;
    EventToken<> register_cancel_callback(CancelCallback callback) override;
    void unregister_cancel_callback(EventToken<> token) override;
};

AMULET_UTILS_EXPORT extern VoidCancelManager global_VoidCancelManager;

class AMULET_UTILS_EXPORT CancelManager : public AbstractCancelManager {
private:
    astd::mutex mutex;
    bool cancelled ASTD_GUARDED_BY(mutex) = false;
    Event<> event;

public:
    CancelManager();

    CancelManager(const CancelManager&) = delete;
    CancelManager& operator=(const CancelManager&) = delete;
    CancelManager(CancelManager&&) = delete;
    CancelManager& operator=(CancelManager&&) = delete;

    ~CancelManager() override;

    void cancel() override;
    bool is_cancel_requested() override;
    EventToken<> register_cancel_callback(CancelCallback callback) override;
    void unregister_cancel_callback(EventToken<> token) override;
};

} // namespace Amulet
