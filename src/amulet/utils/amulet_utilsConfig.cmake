if (NOT TARGET amulet_utils)
    message(STATUS "Finding amulet_utils")

    set(amulet_utils_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")
    find_library(amulet_utils_LIBRARY NAMES amulet_utils PATHS "${CMAKE_CURRENT_LIST_DIR}")
    message(STATUS "amulet_utils_LIBRARY: ${amulet_utils_LIBRARY}")

    add_library(amulet_utils SHARED IMPORTED)
    set_target_properties(amulet_utils PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${amulet_utils_INCLUDE_DIR}"
        IMPORTED_IMPLIB "${amulet_utils_LIBRARY}"
    )
endif()
