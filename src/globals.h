#pragma once

#include "stdint.h"



struct Location;
struct LogInfo;
struct File;
struct INamed;

const int EOL = '\n';
const int EOS = '\0'; // dont know if it is meaningful to do so...

const char SCOPE_BEGIN = '{';
const char SCOPE_END = '}';
const char FUNCTION_START = '(';
const char FUNCTION_END = ')';
const char STRING_LITERAL = '"';

const char POINTER_SYMBOL = '*';
const char ADDRESS_SYMBOL = '&';

const char KWS_VOID[]       = "void";
const char KWS_INT[]        = "int";
const char KWS_INT_32[]     = "i32";
const char KWS_INT_64[]     = "i64";
const char KWS_FLOAT_32[]   = "f32";
const char KWS_FLOAT_64[]   = "f64";
const char KWS_STRING[]     = "string";
const char KWS_POINTER[]    = "*";

const char KWS_CONST[]      = "const";
const char KWS_FUNCTION[]   = "fcn";
const char KWS_ENUM[]       = "enum";
const char KWS_TYPE_DEF[]   = "def";

const char KWS_IF[]     = "if";
const char KWS_ELSE[]   = "else";
const char KWS_FOR[]    = "for";
const char KWS_WHILE[]  = "while";
const char KWS_GOTO[]   = "goto";

const char IFS_PRINTF[] = "printf";

// TODO : better name
enum State : uint64_t {
    IS_CONST    = 1 << 0,
    IS_CMPTIME  = 1 << 1,
    IS_EMBEDED  = 1 << 2,
    
    IS_UNARY    = 1 << 3,
    IS_BINARY   = 1 << 4,
    IS_TERNARY  = 1 << 5,

    /*

    IS_RESERVED_5   = 1 << 7,
    IS_RESERVED_6   = 1 << 8,
    IS_RESERVED_7   = 1 << 9,
    IS_RESERVED_8   = 1 << 10,
    IS_RESERVED_9   = 1 << 11,
    IS_RESERVED_10  = 1 << 12,
    IS_RESERVED_11  = 1 << 13,
    IS_RESERVED_12  = 1 << 14,
    IS_RESERVED_13  = 1 << 15,
    IS_RESERVED_14  = 1 << 16,
    IS_RESERVED_15  = 1 << 17,
    IS_RESERVED_16  = 1 << 18,
    IS_RESERVED_17  = 1 << 19,
    IS_RESERVED_18  = 1 << 20,
    IS_RESERVED_19  = 1 << 21,
    IS_RESERVED_20  = 1 << 22,
    IS_RESERVED_21  = 1 << 23,
    IS_RESERVED_22  = 1 << 24,
    IS_RESERVED_23  = 1 << 25,
    IS_RESERVED_24  = 1 << 26,
    IS_RESERVED_25  = 1 << 27,
    IS_RESERVED_26  = 1 << 28,
    IS_RESERVED_27  = 1 << 29,
    IS_RESERVED_28  = 1 << 30,

    */

    IS_PROCESSED = 1 << 31 // delete?
};

struct File {
    char* name;
    char* buff;
};

struct Location {
    File file;
    int line; // to track current line
    int idx; // current position in buffer
};

struct LogInfo {
    Location loc;
};

struct INamed {
	    
    char* name;
    int nameLen;

    INamed() {};

    constexpr INamed(char* const nm, const int nmLn) : 
        name(nm), 
        nameLen(nmLn)
    {

    };

};