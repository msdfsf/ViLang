#pragma once

#include <stdint.h>
//#include <stdarg.h>
#include "globals.h"

namespace Logger {

    enum Level {
        INFO    = 0x1,
        WARNING = 0x2,
        ERROR   = 0x4
    };

    // use Level enum to define bits
    extern uint32_t verbosity;

    void log(const uint32_t type, const char* message, Location* loc, const int len, ...);
    //void log(const uint32_t type, const char* message, Location* loc, char* const body, const int len);
    void log(const uint32_t type, const char* message, Location* loc);
    void log(const uint32_t type, char* const message);

}