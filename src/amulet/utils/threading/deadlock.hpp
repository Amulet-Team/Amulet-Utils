#pragma once

#include <stdexcept>

#include <amulet/utils/dll.hpp>

namespace Amulet {

// An exception raised in some deadlock cases.
class AMULET_UTILS_EXPORT Deadlock : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    Deadlock();
    ~Deadlock() noexcept override;
};

} // namespace Amulet
