#pragma once

// errors should be logged at tthe place, not propagated and then logged
// as at point of of occurence there is way more info to offer to user

#define ERR_STR(e) ((char*) Err::str[-e])
#define ERR_PSTR(e) ((char**) &(Err::str[-e]))

namespace Err {
    
    enum Err {
        OK                              = 0,
        MALLOC                          = -1,
        UNEXPECTED_END_OF_FILE          = -2,
        UNEXPECTED_END_OF_EXPRESSION    = -3,
        INVALID_OPERATOR                = -4,
        VARIABLE_ALREADY_DEFINED        = -5,
        FUNCTION_ALREADY_DEFINED        = -6,
        UNEXPECTED_SYMBOL               = -7,
        UNKNOWN_VARIABLE                = -8,
        UNKNOWN_FUNCTION                = -9,
        INVALID_NUMBER_LITERAL          = -10,
        INVALID_TYPE_CONVERSION         = -11,
        MISSING_VARIABLE_NAME           = -12,
        INVALID_TYPE_UNARY_OPERATOR     = -13,
        INVALID_TYPE_BINARY_OPERATOR    = -14,
        CANNOT_ASSIGN_TO_CONST          = -15,
        UNKNOWN_DATA_TYPE               = -16,
        INVALID_DATA_TYPE               = -17,
        COMPILE_TIME_KNOWN_EXPRESSION_REQUARIED = -18,
        TYPE_INIT_ATTRIBUTES_COUNT_MISSMATCH    = -19,
        INVALID_ATTRIBUTE_NAME          = -20,
        OPERATOR_CANNOT_ACT_AS_UNARY    = -21,
        OPERATOR_CANNOT_ACT_AS_BINARY   = -22,
        CANNOT_GET_ADDRESS_OF_ADDRESS   = -23,
    };

    const char* const str[] = {

        "OK",
        "Malloc doesnt feel good...",
        "Unexpected end of file!",
        "Unexpected end of expression!",
        "Invalid operator '%.*s'!",
        "Variable '%.*s' already defined!",
        "Function already defined!",
        "Unexpected symbol! %s expected.",
        "Unknown variable '%.*s'!",
        "Unknown function '%.*s'!",
        "Invalid number literal!",
        "Type conversion between '%.*s' and '%.*s' types is invalid!",
        "Varable name is missing!",
        "Invalid type '%.*s' to unary operator '%.*s'!",
        "Invalid type '%.*s' to binary operator '%.*s'!",
        "Cannot assign to constant variable '%.*s'!",
        "Unknown data type!",
        "Invalid data type!",
        "Compile time known expression requaried!",
        "Attributes count mismatch! %i instead of %i given!",
        "Invalid attribute name '%.*s'!",
        "Operator '%.*s' cannot act as unary!",
        "Operator '%.*s' cannot act as binary!",
        "Cannot get address of address!"
    };

}