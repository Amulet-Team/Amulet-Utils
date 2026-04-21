#pragma once

#ifndef AMULET_UTILS_EXPORT
    #if defined(WIN32) || defined(_WIN32)
        #ifdef ExportAmuletUtils
            #define AMULET_UTILS_EXPORT __declspec(dllexport)
        #else
            #define AMULET_UTILS_EXPORT __declspec(dllimport)
        #endif
    #else
        #define AMULET_UTILS_EXPORT __attribute__((visibility("default")))
    #endif
#endif
