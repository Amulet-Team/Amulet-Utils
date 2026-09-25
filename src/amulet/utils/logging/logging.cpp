#include <atomic>
#include <iostream>
#include <mutex>
#include <string>

#include "logging.hpp"

namespace Amulet {

class LogState {
public:
    std::atomic_int min_log_level = 20;
    std::mutex cout_mutex;
    Amulet::EventToken<int, std::string> default_log_handler_token;
    Amulet::Event<int, std::string> logger;

    void register_default_log_handler()
    {
        default_log_handler_token = logger.connect(
            static_cast<void (*)(int, const std::string&)>(print));
    }

    LogState()
    {
        register_default_log_handler();
    }
};

static LogState& _get_log_state()
{
    static LogState logger;
    return logger;
}

int get_min_log_level()
{
    return _get_log_state().min_log_level;
}

void set_min_log_level(int level)
{
    _get_log_state().min_log_level = level;
}

static Amulet::EventToken<int, std::string>& get_default_log_handler_token()
{
    return _get_log_state().default_log_handler_token;
}

void print(const std::string& msg)
{
    std::lock_guard lock(_get_log_state().cout_mutex);
    std::cout << msg << std::endl;
}

void print(int level, const std::string& msg)
{
    AmuletPrint(level, msg);
}

Amulet::Event<int, std::string>& get_logger()
{
    return _get_log_state().logger;
}

void log(int level, const std::string& msg)
{
    AmuletLog(level, msg);
}

void debug(const std::string& msg)
{
    log(10, msg);
}

void info(const std::string& msg)
{
    log(20, msg);
}

void warning(const std::string& msg)
{
    log(30, msg);
}

void error(const std::string& msg)
{
    log(40, msg);
}

void critical(const std::string& msg)
{
    log(50, msg);
}

void register_default_log_handler()
{
    _get_log_state().register_default_log_handler();
}

void unregister_default_log_handler()
{
    get_logger().disconnect(get_default_log_handler_token());
}

} // namespace Amulet
