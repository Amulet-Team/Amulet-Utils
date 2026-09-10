#include "temp.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>

#include <amulet/utils/threading/mutex.hpp>
#include <amulet/utils/threading/thread_safety.hpp>

#include "lock_file.hpp"

namespace Amulet {

class TempDirManager {
private:
    astd::mutex _mutex;
    std::filesystem::path _temp_dir ASTD_GUARDED_BY(_mutex) = "";

    void _clean_temp_dir() ASTD_REQUIRES_UNIQUE(_mutex)
    {
        for (const auto& group : std::filesystem::directory_iterator(_temp_dir)) {
            if (!group.is_directory()) {
                continue;
            }
            for (const auto& dir : std::filesystem::directory_iterator(group.path())) {
                if (!dir.path().filename().string().starts_with("amulettmp-")) {
                    continue;
                }
                try {
                    Amulet::LockFile lock(dir.path() / "lock");
                } catch (const std::runtime_error&) {
                    continue;
                }
                std::filesystem::remove_all(dir.path());
            }
        }
    };

public:
    std::filesystem::path get_temp_dir() ASTD_EXCLUDES_ALL(_mutex)
    {
        astd::lock_guard lock(_mutex);
        if (_temp_dir.empty()) {
            throw std::runtime_error("Temporary directory has not been set.");
        }
        return _temp_dir;
    }

    void set_temp_dir(std::filesystem::path path) ASTD_EXCLUDES_ALL(_mutex)
    {
        if (!std::filesystem::is_directory(path)) {
            throw std::runtime_error("Temporary path is not a directory.");
        }
        astd::lock_guard lock(_mutex);
        if (!_temp_dir.empty()) {
            _clean_temp_dir();
        }
        _temp_dir = std::move(path);
        _clean_temp_dir();
    }
};

TempDirManager& _get_temp_dir_manager()
{
    static TempDirManager _temp_dir_meta;
    return _temp_dir_meta;
}

std::filesystem::path get_temp_dir()
{
    return _get_temp_dir_manager().get_temp_dir();
}

void set_temp_dir(std::filesystem::path path)
{
    _get_temp_dir_manager().set_temp_dir(std::move(path));
}

TempDir::TempDir(const std::string& group)
{
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
                    .count();
    for (size_t i = 0; i < 100; i++) {
        auto path = get_temp_dir() / group / ("amulettmp-" + std::to_string(time) + "-" + std::to_string(i));
        std::filesystem::create_directories(path);
        try {
            _lock = std::make_unique<Amulet::LockFile>(path / "lock");
        } catch (const std::runtime_error&) {
            continue;
        }
        _path = path;
        return;
    }
    throw std::runtime_error("Could not create temporary directory.");
}

TempDir::TempDir(TempDir&&) = default;
TempDir& TempDir::operator=(TempDir&&) = default;

TempDir::~TempDir()
{
    _lock.reset();
    std::filesystem::remove_all(_path);
}

const std::filesystem::path& TempDir::get_path() const
{
    return _path;
}

} // namespace Amulet
