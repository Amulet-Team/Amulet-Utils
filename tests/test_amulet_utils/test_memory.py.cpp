#include <memory>

#include <amulet/utils/memory.hpp>

static_assert(Amulet::is_unique_ptr_v<std::unique_ptr<int>>);
static_assert(!Amulet::is_unique_ptr_v<std::shared_ptr<int>>);
static_assert(!Amulet::is_unique_ptr_v<std::weak_ptr<int>>);
static_assert(!Amulet::is_shared_ptr_v<std::unique_ptr<int>>);
static_assert(Amulet::is_shared_ptr_v<std::shared_ptr<int>>);
static_assert(!Amulet::is_shared_ptr_v<std::weak_ptr<int>>);
static_assert(!Amulet::is_weak_ptr_v<std::unique_ptr<int>>);
static_assert(!Amulet::is_weak_ptr_v<std::shared_ptr<int>>);
static_assert(Amulet::is_weak_ptr_v<std::weak_ptr<int>>);
