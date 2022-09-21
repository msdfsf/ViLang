#pragma once

#include "utils.h"

#include <ctype.h>

namespace Utils {

    // ASCII string
    long str2int(char* const str, const int len, int* endIdx) {

        int i = 0;
        int num = 0;
        while(i < len && isdigit(str[i])) {

            const char ch = str[i];
            if (!isdigit(ch)) break;

            num = num * 10 + (ch - '0');

            i++;

        }

        *endIdx = i;
        return num;

    }

}