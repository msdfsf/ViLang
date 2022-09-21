#pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"
#include "globals.h"

#define DEFAULT_MESSAGE_LENGTH 256

namespace Logger {

    uint32_t verbosity = INFO | WARNING | ERROR;

    // TODO : place in some utility namespace
    int findLineStart(char* const str, const int idx) {

        for (int i = idx - 1; i >= 0; i--) {
            if (str[i] == EOL) return i + 1;
        }

        return 0;

    }

    // TODO : place in some utility namespace
    int findLineEnd(char* const str, const int idx) {

        for (int i = idx; ; i++) {
            const char ch = str[i];
            if (ch == EOL || ch == EOS) return i - 1;
        }

    }

    // TODO : place in some utility namespace
    int skipWhiteSpacesBack(char* const str, const int idx) {

        for (int i = idx - 1; i >= 0; i--) {
            if (str[i] == EOL) return i + 1;
        }

        return 0;

    }

    void log(const uint32_t type, const char* message, Location* loc, const int len, ...) {

        if (!(verbosity & type)) return;

        char* const body = loc->file.buff;

        // TODO : buggy when \0
        const int lnStartIdx = findLineStart(body, loc->idx);
        const int lnEndIdx = findLineEnd(body, loc->idx);
        const int lnLength = lnEndIdx - lnStartIdx + 1;

        switch (type) {

            case INFO : {
                printf("INFO : ");
                break;
            }

            case WARNING : {
                printf("WARNING : ");
                break;
            }

            case ERROR : {
                printf("\nERROR(%i, %i) : ", loc->line, loc->idx - lnStartIdx + 1);
                break;
            }
        
            default :
                break;
        
        }

        va_list args;
        va_start(args, len);
        vprintf(message, args);
        va_end (args);
        putchar('\n');

        // enough?
        char numbuff[32];
        sprintf(numbuff, "%i | ", loc->line);

        printf("%s%.*s\n", numbuff, lnLength, body + lnStartIdx);
        
        // owful
        int i = lnStartIdx;
        for (; i < loc->idx + strlen(numbuff); i++) putchar(' ');
        for (; i < loc->idx + strlen(numbuff) + len; i++) putchar('^');
        for (; i < lnEndIdx; i++) putchar(' ');
        
        putchar('\n');

    }

    void log(const uint32_t type, const char* message, Location* loc, const int len) {

        if (!(verbosity & type)) return;

        char* const body = loc->file.buff;

        // TODO : buggy when \0
        const int lnStartIdx = findLineStart(body, loc->idx);
        const int lnEndIdx = findLineEnd(body, loc->idx);
        const int lnLength = lnEndIdx - lnStartIdx;

        switch (type) {

            case INFO : {
                printf("INFO : ");
                break;
            }

            case WARNING : {
                printf("WARNING : ");
                break;
            }

            case ERROR : {
                printf("\nERROR(%i, %i) : ", loc->line, loc->idx - lnStartIdx);
                break;
            }
        
            default :
                break;
        
        }
        printf("%s\n", message);

        // enough?
        char numbuff[32];
        sprintf(numbuff, "%i | ", loc->line);

        printf("%s%.*s\n", numbuff, lnLength, body + lnStartIdx);
        
        // owful
        int i = lnStartIdx;
        for (; i < loc->idx + strlen(numbuff); i++) putchar(' ');
        for (; i < loc->idx + strlen(numbuff) + len; i++) putchar('^');
        for (; i < lnEndIdx; i++) putchar(' ');
        
        putchar('\n');

    }

    void log(const uint32_t type, const char* message, Location* loc) {

        if (!(verbosity & type)) return;

        char* const body = loc->file.buff;

        const int startIdx = findLineStart(body, loc->idx);
        const int length = loc->idx - startIdx + 1;

        switch (type) {

            case INFO : {
                printf("INFO : ");
                break;
            }

            case WARNING : {
                printf("WARNING : ");
                break;
            }

            case ERROR : {
                printf("\nERROR(%i, %i) : ", loc->line, length);
                break;
            }
        
            default :
                break;
        
        }
        printf("%s\n", message);

        // enough?
        char numbuff[32];
        sprintf(numbuff, "%i | ", loc->line);

        printf("%s%.*s\n", numbuff, length, body + startIdx);
        for (int i = 0; i < length - 1 + strlen(numbuff); i++) {
            putchar(' ');
        }
        putchar('^');
        putchar('\n');

    }

    void log(const uint32_t type, char* const message) {

        if (!(verbosity & type)) return;

        switch (type) {

            case INFO : {
                printf("INFO : ");
                break;
            }

            case WARNING : {
                printf("WARNING : ");
                break;
            }

            case ERROR : {
                printf("ERROR : ");
                break;
            }
        
            default :
                break;
        
        }
        printf(message);

    }


}