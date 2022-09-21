#pragma once

#include <ctype.h>
#include <vector>

#include "globals.h"
#include "parser.h"
#include "syntax.h"
#include "file_driver.h"
#include "utils.h"
#include "logger.h"
#include "error.h"

#define IS_VAR_START ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
#define IS_NUM_START ((ch >= '0' && ch <= '9') || ch == '.')

#define NEW(x) (x*) malloc(sizeof(x));

namespace Parser {





    // functions
    int cmpOneChar(Operator* op, uint32_t word);
    int cmpTwoChars(Operator* op, uint32_t word);

    int findWordEnd(char* const str);
    int findWordEndOrChar(char* const str, const char ch);
    int findVarEnd(char* const str);
    int findNumberEnd(char* const str);
    
    int skipWhiteSpaces(char* const str, int* const idx);
    int selectKeyWord(char* const str, int* const idx);

    int parseScope(Scope* scope, char* const str, Location* const loc, const int global = 0);
    int parseKeyWord(KeyWordType keyWord, Scope* scope, char* const str, Location* const loc, uint64_t param = 0);
    int parseVariable(Scope* scope, char* const str, Location* const loc, Variable** outVar);
    int parseExpression(Operand* var, char* const str, Location* const loc, const uint16_t endChar = ';');
    int parseStringLiteral(Scope* scope, char* const str, Location* const loc);
    int parseTypeInitialization(Scope* scope, char* const str, Location* loc, TypeInitialization** dtypeInit);
    DataTypeEnum parseNumberLiteral(char* const str, int* idx, uint64_t* out);

    int processDataType(const DataTypeEnum dtype, Scope* scope, char* const str, Location* const loc, uint64_t param = 0);

    DataTypeEnum str2dtype(char* const str, const int len);
    void* dtype2val(DataTypeEnum dataType, char* const str, const int len);

    Variable* findVariable(Scope* scope, char* const name, const int nameLen);
    int findVariable(std::vector<Variable*> vars, char* const name, const int nameLen);
    Function* findFunction(Scope* scope, char* const name, const int nameLen);
    uint32_t findDataType(char* const name, const int nameLen);
    TypeDefinition* findCustomDataType(Scope* scope, char* const name, const int nameLen);

    void printDataType(DataTypeEnum dtype);

    Location* getLocationStamp(Location* loc);

    int cast(Operand* const op, const DataTypeEnum dtype);
    int cast(Operand* opTo, Operand* opFrom);
    int castBasicDataType(DataTypeEnum dtypeTo, DataTypeEnum dtypeFrom, uint64_t* valueTo, uint64_t valueFrom);

    void copyValue(Operand* const dest, Operand* const src);
    void copy(Variable* dest, Variable* src);
    void copy(Operand* dest, Variable* src);

    int validateCustomTypeInitialization(TypeDefinition* dtype, TypeInitialization* dtypeInit);


    int applyOperator(Operator* op, Operand* operand);
    int applyOperator(Operator* op, Operand* a, Operand* b);

    int applyUnaryOperatorPlusI32(Operand* operand);
    int applyUnaryOperatorPlusI64(Operand* operand);
    int applyUnaryOperatorPlusF32(Operand* operand);
    int applyUnaryOperatorPlusF64(Operand* operand);

    int applyUnaryOperatorMinusI32(Operand* operand);
    int applyUnaryOperatorMinusI64(Operand* operand);
    int applyUnaryOperatorMinusF32(Operand* operand);
    int applyUnaryOperatorMinusF64(Operand* operand);

    int applyBinaryOperatorAdditionI32(Operand* a, Operand* b);
    int applyBinaryOperatorAdditionI64(Operand* a, Operand* b);
    int applyBinaryOperatorAdditionF32(Operand* a, Operand* b);
    int applyBinaryOperatorAdditionF64(Operand* a, Operand* b);
    
    int applyBinaryOperatorSubtractionI32(Operand* a, Operand* b);
    int applyBinaryOperatorSubtractionI64(Operand* a, Operand* b);
    int applyBinaryOperatorSubtractionF32(Operand* a, Operand* b);
    int applyBinaryOperatorSubtractionF64(Operand* a, Operand* b);

    int applyBinaryOperatorMultiplicationI32(Operand* a, Operand* b);
    int applyBinaryOperatorMultiplicationI64(Operand* a, Operand* b);
    int applyBinaryOperatorMultiplicationF32(Operand* a, Operand* b);
    int applyBinaryOperatorMultiplicationF64(Operand* a, Operand* b);
    
    int applyBinaryOperatorDivisionI32(Operand* a, Operand* b);
    int applyBinaryOperatorDivisionI64(Operand* a, Operand* b);
    int applyBinaryOperatorDivisionF32(Operand* a, Operand* b);
    int applyBinaryOperatorDivisionF64(Operand* a, Operand* b);

    int applyBinaryOperatorModuloI32(Operand* a, Operand* b);
    int applyBinaryOperatorModuloI64(Operand* a, Operand* b);

    int applyUnaryOperatorAddress(Operand* op);



    std::vector<Operand*> fcnCalls;
    std::vector<VariableDefinition*> customDataTypesReferences;

    const KeyWord keyWords[] = {
        {0, KWS_VOID},
        {1, KWS_INT},
        {2, KWS_INT_32},
        {3, KWS_INT_64},
        {4, KWS_FLOAT_32},
        {5, KWS_FLOAT_64},
        {6, KWS_STRING},
        {7, KWS_CONST},
        {8, KWS_FUNCTION},
        {9, KWS_IF},
        {10, KWS_FOR},
        {11, KWS_WHILE},
        {12, KWS_GOTO},
        {13, KWS_ENUM},
        {14, KWS_TYPE_DEF},
    };

    const int KEY_WORDS_COUNT = sizeof(keyWords) / sizeof(KeyWord);

    std::vector<TypeDefinition*> customDataTypes;

    const int DATA_TYPES_COUNT = sizeof(dataTypes) / sizeof(DataType);

    Operator operators[] = {
        
        // +
        {
            '+',
            4,
            IS_UNARY | IS_BINARY,
            &cmpOneChar,
        },

        // -
        {
            '-',
            4,
            IS_UNARY | IS_BINARY,
            &cmpOneChar
        },

        // *
        {
            '*',
            3,
            IS_UNARY | IS_BINARY,
            &cmpOneChar
        },

        // %
        {
            '/',
            3,
            IS_BINARY,
            &cmpOneChar
        },

        // /
        {
            '%',
            3,
            IS_BINARY,
            &cmpOneChar
        },

        // get address
        {
            '&',
            4,
            IS_UNARY | IS_BINARY,
            &cmpOneChar
        },

    };

    const int OPERATORS_COUNT = sizeof(operators) / sizeof(Operator);



    enum InternalFunction {
        IF_PRINTF = 1,
    };

    // TODO : make it 'static' if its even posible
    Function internalFunctions[] = {
        Function(
            SyntaxNode::root,
            (char*) IFS_PRINTF,
            sizeof(IFS_PRINTF) - 1,
            std::vector<Variable*> ({
                new Variable(SyntaxNode::root, DT_STRING), 
                new Variable(SyntaxNode::root, DT_MULTIPLE_TYPES)
            }),
            std::vector<DataTypeEnum>(DT_VOID),
            IF_PRINTF
        )
    };



    int parse(char* const flname) {

        const int verbose = 1;
        Logger::log(Logger::INFO, "Parsing...\n");

        SyntaxNode::root = new Scope;
        SyntaxNode::root->scope = NULL;

        

        char* buffer;
        if (FileDriver::readFile(flname, &buffer)) {
            return 1;
        }

        Location location = {
            {
                flname,
                buffer
            },
            1,
            0           
        };



        const int err = parseScope(SyntaxNode::root, buffer, &location, 1);
        if (err != Err::OK) return err;



        // after everything is done, we can actualy try to link functions with
        // function calls and custom types with correspoding definitions
        
        // functions
        //

        // TODO : errors!!!!
        for (int i = 0; i < (int) fcnCalls.size(); i++) {

            FunctionCall* fcnCall = (FunctionCall*) (fcnCalls[i]->expression);

            Function* fcn = findFunction(fcnCalls[i]->scope, fcnCall->name, fcnCall->nameLen);
            if (!fcn) {
                Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_FUNCTION), &location, fcnCall->nameLen, fcnCall->nameLen, fcnCall->name);
                return Err::UNKNOWN_FUNCTION;
            }

            // verify arguments types
            const int fcnInCount = fcn->inArgs.size();
            const int fcnCallInCount = fcnCall->inArgs.size();
            if (fcnInCount - 1 > fcnCallInCount) {
                printf("\n\nERROR\n\n");
                return 0;
            }

            for (int i = 0; i < fcnInCount - 1; i++) {

                const int fcnDtype = fcn->inArgs[i]->dataTypeEnum;
                const int fcnCallDtype = fcnCall->inArgs[i]->dataTypeEnum;

                if (fcnDtype != fcnCallDtype) {
                    // try to cast
                    if (cast(fcn->inArgs[i], fcnCall->inArgs[i])) {
                        // error
                        printf("\n\nERROR\n\n");
                        return 0;
                    }
                }
            
            }

            const int multipleTypes = fcnInCount > 0 && fcn->inArgs[fcnInCount - 1]->dataTypeEnum == DT_MULTIPLE_TYPES;

            if (fcnInCount <= 0) {
                
                if (fcnCallInCount > 0) {
                    printf("\n\nERROR\n\n");
                    return 0;
                }
            
            } else if (fcn->inArgs[fcnInCount - 1]->dataTypeEnum != DT_MULTIPLE_TYPES) {
                
                if (fcnInCount != fcnCallInCount) {
                    printf("\n\nERROR\n\n");
                    return 0;
                }

                const int fcnDtype = fcn->inArgs[fcnInCount - 1]->dataTypeEnum;
                const int fcnCallDtype = fcnCall->inArgs[fcnInCount - 1]->dataTypeEnum;

                if (fcnDtype != fcnCallDtype) {
                    // try to cast
                    if (cast(fcn->inArgs[fcnInCount - 1], fcnCall->inArgs[fcnInCount - 1])) {
                        // error
                        printf("\n\nERROR\n\n");
                        return 0;
                    }
                }
            
            }

            fcnCall->fcn = fcn;

        }

        // custom types
        //

        for (int i = 0; i < (int) customDataTypesReferences.size(); i++) {

            VariableDefinition* const varDef = customDataTypesReferences[i];

            TypeDefinition* dtype = findCustomDataType(varDef->scope, varDef->dtypeName, varDef->dtypeNameLen);
            if (!dtype) {
                Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_DATA_TYPE), varDef->loc, varDef->dtypeNameLen);
                return Err::UNKNOWN_DATA_TYPE;
            }

            TypeInitialization* const dtypeInit = (TypeInitialization*) varDef->var->expression;
            if (dtypeInit) {

                const int err = validateCustomTypeInitialization(dtype, dtypeInit);
                if (err < 0) return err;
            
            }

            varDef->var->dtype = dtype;

        }

        Scope* scope = SyntaxNode::root;
        while (scope) {

            std::vector<Variable*> vars = scope->vars;
            for (int i = 0; i < (int) vars.size(); i++) {
                
                Variable* var = vars[i];

                if (var->expression) {
                    Operand* ans = new Operand;
                    Operand* varPart = NULL;
                    Operand* constPart = NULL;
                    DataTypeEnum dtype = (DataTypeEnum) var->expression->evaluate((Operand**) &var, &constPart);
                    //printDataType(dtype);
                }

            }

            scope = scope->scope;
        
        }

        return 0;

    }

    // TypeDefinition and TypeInitialization has to be valid (contain only unique names and so on)
    int validateCustomTypeInitialization(TypeDefinition* dtype, TypeInitialization* dtypeInit) {

        if (dtype->vars.size() != dtypeInit->attributes.size()) {
            Logger::log(Logger::ERROR, ERR_STR(Err::TYPE_INIT_ATTRIBUTES_COUNT_MISSMATCH), dtype->loc, dtype->nameLen, dtypeInit->attributes.size(), dtype->vars.size());
            return Err::TYPE_INIT_ATTRIBUTES_COUNT_MISSMATCH;
        }

        const int count = dtypeInit->attributes.size();

        // TODO : is there a way to get rid of this malloc
        Variable** unnamedBuff = (Variable**) malloc(2 * count * sizeof(Variable*));
        int unnamedIdx = 0;

        for (int i = 0; i < count; i++) {
            unnamedBuff[i * 2 + 1] = dtype->vars[i];
        }

        // named ones
        //

        for (int i = 0; i < count; i++) {

            // TODO : better naming!!!!
            Variable* attribute = dtypeInit->attributes[i];
            if (!attribute->name) {
                unnamedBuff[unnamedIdx] = attribute;
                unnamedIdx++;
                continue;
            }
                
            int idx = findVariable(dtype->vars, attribute->name, attribute->nameLen);
            unnamedBuff[2 * idx + 1] = NULL;

            Variable* const var = dtype->vars[idx];
            if (!var) {
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_ATTRIBUTE_NAME), attribute->loc, attribute->nameLen, attribute->nameLen, attribute->name);
                return Err::INVALID_ATTRIBUTE_NAME;
            }

            // typecheck
            if (!attribute->expression) {
                continue;
            }
            
            // TODO : better branching!!!!
            Operand* varPart = NULL;
            Operand* constPart = NULL;

            const int attributeDtype = attribute->expression->evaluate((Operand**) &attribute, &constPart);
            if (attributeDtype == DT_UNDEFINED) {
                // undefined means - type is not specified, so it is basicly unitilized value, 
                // so it's compatible with anything
                
            
            } else if (attributeDtype == DT_CUSTOM && var->dataTypeEnum == DT_CUSTOM) {
                // custom dtype means we have to check alingment of attributes as we dont know

                if (!var->dtype) {
                // if no dtype, we have to find appropriate dtype and 'cash it'

                    TypeDefinition* dtype = findCustomDataType(var->scope, var->def->dtypeName, var->def->dtypeNameLen);
                    if (!dtype) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_DATA_TYPE), var->def->loc, var->def->dtypeNameLen);
                        return Err::UNKNOWN_DATA_TYPE;
                    }

                    var->dtype = dtype;
                
                }

                const int err = validateCustomTypeInitialization((TypeDefinition*) var->dtype, (TypeInitialization*)((WrapperExpression*)attribute->expression)->operand->expression);
                if (err < 0) return err;

            } else if (attributeDtype >= DT_MULTIPLE_TYPES) {
                // error
                
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_DATA_TYPE), attribute->loc, attribute->nameLen);
                return Err::INVALID_DATA_TYPE;
            
            } else {
                // basicly, if its any standard type, we should be able to do a cast
            }

        }


        // unnamed ones
        //

        unnamedIdx = 0;
        for (int i = 0; i < count; i++) {

            Variable* const var = unnamedBuff[2 * i + 1];
            if (!var) continue;

            Variable* attribute = unnamedBuff[unnamedIdx];
            unnamedIdx++;
            
            if (!attribute->expression) {
                continue;
            }
            
            // TODO : better branching!!!!
            const int attributeDtype = attribute->expression->evaluate((Operand**) &attribute, NULL);
            if (attributeDtype == DT_UNDEFINED) {
                // undefined means type is not specified, so it is basicly unitilized value, 
                // so it's compatible with anything
                
            
            } else if (attributeDtype == DT_CUSTOM && var->dataTypeEnum == DT_CUSTOM) {
                // custom dtype means we have to check alingment of attributes as we dont know

                if (!var->dtype) {
                // if no dtype, we have to find appropriate dtype and 'cash it'

                    TypeDefinition* dtype = findCustomDataType(var->scope, var->def->dtypeName, var->def->dtypeNameLen);
                    if (!dtype) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_DATA_TYPE), var->def->loc, var->def->dtypeNameLen);
                        return Err::UNKNOWN_DATA_TYPE;
                    }

                    var->dtype = dtype;
                
                }

                const int err = validateCustomTypeInitialization((TypeDefinition*) var->dtype, (TypeInitialization*)((WrapperExpression*)attribute->expression)->operand->expression);
                if (err < 0) return err;

            } else if (attributeDtype >= DT_MULTIPLE_TYPES) {
                // error
                
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_DATA_TYPE), attribute->loc, attribute->nameLen);
                return Err::INVALID_DATA_TYPE;
            
            } else {
                // basicly, if its any standard type, we should be able to do a cast
            }

        }

        return Err::OK;

    }

    // the begin char has to be already skipped
    int parseScope(Scope* scope, char* const str, Location* const loc, const int global) {

        while(1) {

            mainLoop:

            int linesSkiped = skipWhiteSpaces(str, &(loc->idx));
            if (linesSkiped < 0) {
                
                if (global) return Err::OK;

                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                return Err::UNEXPECTED_END_OF_FILE;
            
            }

            loc->line += linesSkiped;

            const char ch = str[loc->idx];

            if (ch == ';') {
				
                loc->idx++;

            } else if (ch == SCOPE_BEGIN) {

                Scope* newScope = new Scope;
                newScope->scope = scope;
                
                loc->idx++;
                const int err = parseScope(newScope, str, loc);
                if (err < 0) return err;

                scope->children.push_back(newScope);

            } else if (ch == SCOPE_END) {

                if (!global) {
                    loc->idx++;
                    return Err::OK;
                }

                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, 1, "");
                return Err::UNEXPECTED_SYMBOL;

            } else if (ch == STRING_LITERAL) {
                // two things can happen : print to stdout OR expression

                loc->idx++;
                const int err = parseStringLiteral(scope, str, loc);
                if (err < 0) {

                    Variable* var = new Variable(loc);
                    var->scope = scope;
                    
                    const int err = parseExpression(var, str, loc);
                    if (err < 0) return err;
                    
                    // LOOK AT : maybe add to vars as well
                    Scope::root->children.push_back(var);

                }

            } else if (ch == '/') {

                const char nextCh = str[loc->idx + 1];
                if (nextCh == '/') {
                    // line comment
                    
                    loc->idx += 2;
                    while (1) {
                        const char ch = str[loc->idx];
                        if (ch == EOL || ch == EOS) break;
                        loc->idx++;
                    }

                    loc->idx++;
                    loc->line++;

                } else if (nextCh == SCOPE_BEGIN) {
                    // block comment

                    int toClose = 1;

                    loc->idx += 2;
                    while (1) {

                        const char ch = str[loc->idx];

                        if (ch == '\n') {
                            loc->line++;
                        } else if (ch == '/') {

                            loc->idx++;
                            const char nextCh = str[loc->idx];
                            
                            if (nextCh == SCOPE_BEGIN) {
                                toClose++;
                            } else if (nextCh == SCOPE_END) {
                                toClose--;
                                if (toClose <= 0) break;
                            }
                        
                        } else if (ch == '\0') {
                            Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                            return Err::UNEXPECTED_END_OF_FILE;
                        }
                        
                        loc->idx++;

                    }

                    loc->idx++;

                } else {

                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, 1, "Line comment '//' or block comment start '/{'");
                    return Err::UNEXPECTED_SYMBOL;

                }

            } else {

                const int keyWordIdx = selectKeyWord(str, &(loc->idx));
                if (keyWordIdx >= 0) {

                    const int err = parseKeyWord((KeyWordType) keyWords[keyWordIdx].type, scope, str, loc); 
                    if (err < 0) return err;
                
                } else {
                    // if not keyword maybe expression, variable assignment or custom dataType

                    int err;
                    int linesSkipped = 0;
                    Location* const startLoc = getLocationStamp(loc);

                    // lets try custom dataType

                    char* const word = str + loc->idx;
                    const int wordLen = findWordEnd(word);

                    loc->idx += wordLen;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkiped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    const char tmpCh = str[loc->idx];
                    if (tmpCh == '=') {
                        // assignment

                        const int saveIdx = loc->idx;
                        loc->idx -= wordLen + 1;
                        
                        // Variable* var = findVariable(scope, word, wordLen);
                        Variable* newVar;
                        err = parseVariable(scope, str, loc, &newVar);
                        if (err < 0) return err;

                        if (newVar->def->flags & IS_CONST) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::CANNOT_ASSIGN_TO_CONST), loc, wordLen, wordLen, word);
                            return Err::CANNOT_ASSIGN_TO_CONST;
                        }

                        //Variable* newVar = new Variable(loc);
                        //copy(newVar, var);
                        //newVar->expression = NULL;

                        VariableAssignment* varAssignment = new VariableAssignment(loc);
                        varAssignment->scope = scope;
                        varAssignment->var = newVar;

                        loc->idx = saveIdx + 1;

                        err = parseExpression(newVar, str, loc);
                        if (err < 0) return err;

                        Scope::root->children.push_back(varAssignment);

                        goto mainLoop;

                    } else if (!((tmpCh >= 'a' && tmpCh <= 'z') || (tmpCh >= 'A' && tmpCh >= 'Z'))) {
                        // expression

                        Operand* operand = new Operand;
                        operand->scope = scope;
                        operand->expression = NULL;
                        operand->unrollExpression = 1;
                        operand->value = NULL;

                        ExpressionWrapper* exWrapper = new ExpressionWrapper;
                        exWrapper->operand = operand;

                        const int err = parseExpression(operand, str, loc);
                        if (err < 0) return err;

                        // LOOK AT : maybe add to vars as well
                        Scope::root->children.push_back(exWrapper);

                        goto mainLoop;

                    }

                    // seems like custom dtype

                    char* const varName = str + loc->idx;
                    const int varNameLen = findWordEnd(varName);
                    
                    loc->idx += varNameLen;

                    if (findVariable(scope, varName, varNameLen)) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, varNameLen, varNameLen, varName);
                        return Err::VARIABLE_ALREADY_DEFINED;
                    }

                    VariableDefinition* newVarDef = new VariableDefinition(startLoc);                    
                    Variable* newVar = new Variable(scope, DT_CUSTOM, loc);

                    newVarDef->flags = 0;
                    newVarDef->var = newVar;
                    newVarDef->dtypeName = word;
                    newVarDef->dtypeNameLen = wordLen;
                    newVarDef->scope = scope;
                    
                    newVar->name = varName;
                    newVar->nameLen = varNameLen;
                    newVar->def = newVarDef;
                    newVar->scope = scope;

                    scope->vars.push_back(newVar);
                    scope->children.push_back(newVarDef);
                    customDataTypesReferences.push_back(newVarDef);

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkiped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    const char ch = str[loc->idx];
                    if (ch == '=') {

                        loc->idx++;

                        linesSkipped = skipWhiteSpaces(str, &loc->idx);
                        if (linesSkipped < 0) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                            return Err::UNEXPECTED_END_OF_FILE;
                        }
                        loc->line += linesSkipped;

                        if (str[loc->idx] != SCOPE_BEGIN) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                            return Err::UNEXPECTED_SYMBOL;
                        }

                        loc->idx++;

                        TypeInitialization* dtypeInit = new TypeInitialization();
                        newVar->expression = dtypeInit;
                        
                        while (1) {

                            linesSkipped = skipWhiteSpaces(str, &loc->idx);
                            if (linesSkiped < 0) {
                                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                                return Err::UNEXPECTED_END_OF_FILE;
                            }
                            loc->line += linesSkipped;

                            int i = loc->idx;
                            while (1) {

                                const char ch = str[i];
                                if (ch == '=') {
                                    
                                    int len = 0;
                                    for (int j = i - 1; j >= 0; j--) {
                                        if (str[j] > ' ') {
                                            len = j - loc->idx + 1;
                                            break;
                                        }
                                    }

                                    if (len <= 0) {
                                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                                        return Err::UNEXPECTED_SYMBOL;
                                    }

                                    const char tmpCh = str[loc->idx];
                                    if (!((tmpCh >= 'a' && tmpCh <= 'z') || (tmpCh >= 'A' && tmpCh <= 'Z'))) {
                                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, 1, "Variable name");
                                        return Err::UNEXPECTED_SYMBOL;
                                    }

                                    if (len != findVarEnd(str + loc->idx)) {
                                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                                        return Err::UNEXPECTED_SYMBOL;
                                    }

                                    char* const varName = str + loc->idx;

                                    if (findVariable(dtypeInit->attributes, varName, len) >= 0) {
                                        Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, len, len, varName);
                                        return Err::VARIABLE_ALREADY_DEFINED;
                                    }

                                    Variable* var = new Variable(scope, DT_UNDEFINED, loc);
                                    var->name = varName;
                                    var->nameLen = len; // TODO : trim
                                    var->scope = scope;
                                    var->unrollExpression = 0;

                                    dtypeInit->attributes.push_back(var);

                                    loc->idx = i + 1;
                                    const int err = parseExpression(var, str, loc, ',' + (SCOPE_END << 8));
                                    if (err < 0) return err;

                                    const int ch = str[loc->idx - 1];
                                    if (ch == SCOPE_END) goto mainLoop;
                                    if (ch == ',') break;

                                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                                    return Err::UNEXPECTED_SYMBOL;
                                    
                                } else if (ch == '\0') {
                                    
                                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                                    return Err::UNEXPECTED_END_OF_FILE;
                                
                                } else if (ch == ',' || ch == SCOPE_END) {

                                    const int len = i - loc->idx;

                                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                                    if (linesSkiped < 0) {
                                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                                        return Err::UNEXPECTED_END_OF_FILE;
                                    }

                                    uint64_t value;
                                    const int tmpIdx = loc->idx;
                                    DataTypeEnum dtype = parseNumberLiteral(str + loc->idx, &loc->idx, &value);
                                    if (tmpIdx == loc->idx) {

                                        loc->idx = i + 1;
                                        if (ch == SCOPE_END) goto mainLoop;
                                        if (ch == ',') {

                                            Variable* var = new Variable(scope, DT_UNDEFINED, loc);
                                            var->scope = scope;
                                            
                                            dtypeInit->attributes.push_back(var);

                                            break;
                                        
                                        }

                                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                                        return Err::UNEXPECTED_SYMBOL;
                                    
                                    }

                                    Variable* var = new Variable(scope, dtype, loc);
                                    var->scope = scope;
                                    var->value = (void*) value;

                                    dtypeInit->attributes.push_back(var);

                                    loc->idx = i + 1;
                                    if (ch == SCOPE_END) goto mainLoop;
                                    if (ch == ',') break;

                                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                                    return Err::UNEXPECTED_SYMBOL;
                                
                                }

                                i++;

                            }                         

                        }

                    } else if (ch != ';') {

                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;    
                    
                    }

                }

                loc->idx += 1;

            }
        
        }

    }

    int parseVariable(Scope* scope, char* const str, Location* const loc, Variable** outVar) {

        Variable* parentStruct = NULL;

        const int idx = loc->idx;
        char* const varName = str + idx;
        const int varNameLen = findVarEnd(varName);

        /*
        int wordLen;
        Pointer* mainPtr = NULL; // TODO : rename
        while (1) {

            loc->line += skipWhiteSpaces(str, &loc->idx);
            wordLen = findWordEnd(str + loc->idx);

            if (str[loc->idx] != POINTER_SYMBOL) break;
                
            Pointer* ptr = new Pointer();
                
            if (mainPtr) {

                ptr->pointsTo = (DataType*) mainPtr;  // dataTypes + DT_POINTER;
                ptr->pointsToEnum = DT_POINTER;

            } else {
                    
                ptr->pointsTo = dataTypes + dtype;
                ptr->pointsToEnum = dtype;
                var->dataTypeEnum = DT_POINTER;
                // var->dtype = (DataType*) ptr;       
                
            }

            var->dtype = (DataType*) ptr;


            loc->idx++;
                
            // loc->line += skipWhiteSpaces(str, &loc->idx);
            // wordLen = findWordEnd(str + loc->idx);

            mainPtr = ptr;
        
        }
        */

        Variable* const prevVar = findVariable(scope, varName, varNameLen);
        if (!prevVar) {
            Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_VARIABLE));
            return Err::UNKNOWN_VARIABLE;
        }

        VariableDefinition* varDef = prevVar->def;

        Variable* var = new Variable(scope, prevVar->dataTypeEnum, loc);
        var->name = varName;
        var->nameLen = varNameLen;
        var->def = varDef;
        var->parentStruct = parentStruct;;
                
        // TODO : any whitespaces before and after
        if (str[idx + varNameLen] != '.') {
            loc->idx += varNameLen;
            *outVar = var;
            return Err::OK;
        }

        loc->idx += varNameLen + 1;
        parentStruct = var;

        while (1) {

            char* const varName = str + loc->idx;
            const int varNameLen = findVarEnd(varName);

            Variable* var = new Variable(scope, prevVar->dataTypeEnum, loc);
            var->name = varName;
            var->nameLen = varNameLen;
            var->def = varDef;
            var->parentStruct = parentStruct;

            // TODO : any whitespaces before and after
            if (str[loc->idx + varNameLen] != '.') {
                loc->idx += varNameLen;
                *outVar = var;
                return Err::OK;
            }

            loc->idx += varNameLen + 1;
            parentStruct = var;
        
        }

    }

    // already without starting '"'
    int parseStringLiteral(Scope* scope, char* const str, Location* const loc) {

        // "asdasd";
        // "%i\n" 3;

        const int startIdx = loc->idx;

        while (1) {

            const char ch = str[loc->idx];
            if (ch == '"') {

                Function* const fcn = internalFunctions + (IF_PRINTF - 1);

                FunctionCall* fcnCall = new FunctionCall;
                fcnCall->fcn = fcn;
                fcnCall->name = fcn->name;
                fcnCall->nameLen = fcn->nameLen;

                Variable* strVar = new Variable(scope, DT_STRING, loc);
                const int strVarNameLen = loc->idx - startIdx;
                strVar->value = malloc(strVarNameLen + 1);
                if (!strVar->value) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::MALLOC));
                    return Err::MALLOC;
                }
                memcpy(strVar->value, str + startIdx, strVarNameLen);
                ((char*) (strVar->value))[strVarNameLen] = '\0';

                fcnCall->inArgs.push_back(strVar);

                Operand* operand = new Operand;
                operand->scope = scope;
                operand->expression = fcnCall;
                operand->unrollExpression = 1;
                operand->value = NULL;

                ExpressionWrapper* exWrapper = new ExpressionWrapper;
                exWrapper->operand = operand;
                exWrapper->operand->expression = fcnCall;

                scope->children.push_back(exWrapper);

                loc->idx++;

                while (1) {

                    const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    char ch = str[loc->idx];
                    if (ch == ';') {
                        loc->idx++;
                        return Err::OK;
                    }

                    // LOOK AT : is Variable needed? maybe use just Operand
                    // LOOK AT : separate function?
                    Variable* newOperand = new Variable(scope, DT_UNDEFINED, loc);
                    /*
                    newOperand->name = NULL;
                    newOperand->nameLen = 0;
                    newOperand->expression = NULL;
                    newOperand->scope = scope;
                    newOperand->unrollExpression = 1;
                    newOperand->dataTypeEnum = DT_UNDEFINED;
                    */

                    int err = parseExpression(newOperand, str, loc, ',' + (';' << 8));
                    if (err < 0) return err;

                    fcnCall->inArgs.push_back(newOperand);

                    loc->idx--;
                    ch = str[loc->idx]; // LOOK AT : could be zero?
                    if (ch == ',') {
                        loc->idx++;
                    } else if (ch == ';') {
                        loc->idx++;
                        return Err::OK;
                    } else {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    }

                }

            } else if (ch == '\0') {

                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE), loc);
                return Err::UNEXPECTED_END_OF_FILE;
            
            }

            loc->idx++;

        }

        return Err::OK;

    }

    // null-terminated
    int skipWhiteSpaces(char* const str, int* const idx) {

        int i = *idx;
        int lines = 0;
        while(1) {

            const char ch = str[i];

            if (ch > 32) {
                // assuming valid char
                *idx = i;
                return lines;
            } else if (ch == EOL) {
                lines++;
            } else if (ch == EOS) {
                *idx = i;
                return -1;
            }

            i++;

        }
        
    }

    // TODO: optimize - use precompile hash or something
    // null-terminated
    int selectKeyWord(char* const str, int* const idx) {

        const int startStrIdx = *idx;
        for (int i = 0; i < KEY_WORDS_COUNT; i++) {

            int strIdx = startStrIdx;
            const char* keyWord = keyWords[i].str;
            while (1) {
                
                const char ch = *keyWord;

                if (ch == '\0') {
                    *idx = strIdx;
                    return i;
                }

                if (ch != str[strIdx]) {
                    *idx = strIdx;
                    break;
                }
                
                strIdx++;
                keyWord++;
            
            }

        }

        return -1;

    }

    int parseTypeInitialization(Scope* scope, char* const str, Location* loc, TypeInitialization** dtypeInit) {
        
        *dtypeInit = new TypeInitialization();

        while (1) {

            const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
            if (linesSkipped < 0) {
                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                return Err::UNEXPECTED_END_OF_FILE;
            }
            loc->line += linesSkipped;

            int i = loc->idx;
            while (1) {

                const char ch = str[i];
                if (ch == '=') {
                    
                    int len = 0;
                    for (int j = i - 1; j >= 0; j--) {
                        if (str[j] > ' ') {
                            len = j - loc->idx + 1;
                            break;
                        }
                    }

                    if (len <= 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    const char tmpCh = str[loc->idx];
                    if (!((tmpCh >= 'a' && tmpCh <= 'z') || (tmpCh >= 'A' && tmpCh <= 'Z'))) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, 1, "Variable name");
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    if (len != findVarEnd(str + loc->idx)) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    char* const varName = str + loc->idx;

                    if (findVariable((*dtypeInit)->attributes, varName, len) >= 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, len, len, varName);
                        return Err::VARIABLE_ALREADY_DEFINED;
                    }

                    Variable* var = new Variable(scope, DT_UNDEFINED, loc);
                    var->name = varName;
                    var->nameLen = len; // TODO : trim
                    var->scope = scope;
                    var->unrollExpression = 0;

                    (*dtypeInit)->attributes.push_back(var);

                    loc->idx = i + 1;
                    const int err = parseExpression(var, str, loc, ',' + (SCOPE_END << 8));
                    if (err < 0) return err;

                    const int ch = str[loc->idx - 1];
                    if (ch == SCOPE_END) return Err::OK;
                    if (ch == ',') break;

                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                    return Err::UNEXPECTED_SYMBOL;
                    
                } else if (ch == '\0') {
                    
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                
                } else if (ch == ',' || ch == SCOPE_END) {

                    const int len = i - loc->idx;

                    const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    uint64_t value;
                    const int tmpIdx = loc->idx;
                    DataTypeEnum dtype = parseNumberLiteral(str + loc->idx, &loc->idx, &value);
                    if (tmpIdx == loc->idx) {

                        loc->idx = i + 1;
                        if (ch == SCOPE_END) return Err::OK;
                        if (ch == ',') {

                            Variable* var = new Variable(scope, DT_UNDEFINED, loc);
                            var->scope = scope;
                            
                            (*dtypeInit)->attributes.push_back(var);

                            break;
                        
                        }

                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;
                    
                    }

                    Variable* var = new Variable(scope, dtype, loc);
                    var->scope = scope;
                    var->value = (void*) value;

                    (*dtypeInit)->attributes.push_back(var);

                    loc->idx = i + 1;
                    if (ch == SCOPE_END) return Err::OK;
                    if (ch == ',') break;

                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                    return Err::UNEXPECTED_SYMBOL;
                
                }

                i++;

            }                         

        }

        return Err::OK;

    }

    int processDataType(const DataTypeEnum dtype, Scope* scope, char* const str, Location* const loc, uint64_t param) {
        
        // LOOK AT : consider Operand instead of variable
        VariableDefinition* varDef = new VariableDefinition(loc);
        Variable* var = new Variable(scope, dtype, loc);
        
        varDef->var = var;
        varDef->flags = param;

        var->def = varDef;
        var->unrollExpression = 0;

        int wordLen;
        Pointer* mainPtr = NULL; // TODO : rename
        while (1) {

            loc->line += skipWhiteSpaces(str, &loc->idx);
            wordLen = findWordEnd(str + loc->idx);

            if (str[loc->idx] != POINTER_SYMBOL) break;
                
            Pointer* ptr = new Pointer();
                
            if (mainPtr) {

                ptr->pointsTo = (DataType*) mainPtr;  // dataTypes + DT_POINTER;
                ptr->pointsToEnum = DT_POINTER;
                //mainPtr->pointsToEnum = DT_POINTER;
                //mainPtr->pointsTo = (DataType*) ptr;    
                // var->dtype = (DataType*)ptr;

            } else {
                    
                ptr->pointsTo = dataTypes + dtype;
                ptr->pointsToEnum = dtype;
                var->dataTypeEnum = DT_POINTER;
                // var->dtype = (DataType*) ptr;       
                
            }

            var->dtype = (DataType*) ptr;


            loc->idx++;
                
            // loc->line += skipWhiteSpaces(str, &loc->idx);
            // wordLen = findWordEnd(str + loc->idx);

            mainPtr = ptr;
        
        }

        if (!mainPtr) {
            var->dtype = dataTypes + dtype;
        }

        var->name = str + loc->idx;
        var->nameLen = wordLen;

        Variable* prevDefVar = findVariable(scope, var->name, var->nameLen);
        if (prevDefVar) {
            // var allready defined
            // TODO : store in variable last position to report
            
            delete var;

            Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, var->nameLen, var->nameLen, var->name);
            return Err::VARIABLE_ALREADY_DEFINED;
        }
        
        int i = loc->idx + wordLen;
        while (1) {
            
            const char ch = str[i];

            if (ch == '=') {
                
                i++;
                loc->idx = i;

                const int err = parseExpression(var, str, loc);
                if (err < 0) {
                    return err;                    
                }

                scope->children.push_back(varDef);
                scope->vars.push_back(var);
                
                return Err::OK;

            }
            
            if (ch == ';') {

                loc->idx = i;
                
                scope->children.push_back(varDef);
                scope->vars.push_back(var);
                
                break;
            
            }
            
            if (ch == '\0') {

                delete var;
                loc->idx = i;
                
                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                return Err::UNEXPECTED_END_OF_FILE;
            
            }

            i++;

        }

        return Err::OK;

    }

    int parseKeyWord(KeyWordType keyWord, Scope* scope, char* const str, Location* loc, uint64_t param) {

        switch (keyWord) {

            case KW_INT : return processDataType(DT_INT, scope, str, loc, param);

            case KW_INT_32 : return processDataType(DT_INT_32, scope, str, loc, param);

            case KW_INT_64 : return processDataType(DT_INT_64, scope, str, loc, param);

            case KW_FLOAT_32 : return processDataType(DT_FLOAT_32, scope, str, loc, param);

            case KW_FLOAT_64 : return processDataType(DT_FLOAT_64, scope, str, loc, param);

            case KW_CONST : {
                
                // var or function

                const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }
                loc->line += linesSkipped;

                int wordLen = loc->idx;

                const int keyWordIdx = selectKeyWord(str, &(loc->idx));
                wordLen = loc->idx - wordLen + 1;

                if (keyWordIdx < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, wordLen, "Variable or function definition");
                    return Err::UNEXPECTED_SYMBOL;
                }

                const KeyWordType ktype = (KeyWordType) (keyWords + keyWordIdx)->type;
                if (ktype > KW_STRING && ktype != KW_FUNCTION) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, wordLen, "Variable or function definition");
                    return Err::UNEXPECTED_SYMBOL;
                }

                const int err = parseKeyWord(ktype, scope, str, loc, IS_CONST);
                if (err < 0) return err;

                return Err::OK;

            }

            case KW_FUNCTION : {

                Function* fcn = new Function;

                loc->line += skipWhiteSpaces(str, &loc->idx);
                const int wordLen = findVarEnd(str + loc->idx); // TODO : search till char or space

                fcn->name = str + loc->idx;
                fcn->nameLen = wordLen;

                Function* prevDefFcn = findFunction(scope, fcn->name, fcn->nameLen);
                if (prevDefFcn) {
                    // fcn allready defined
                    // TODO : store in variable last position to report
                    
                    delete fcn;

                    Logger::log(Logger::ERROR, ERR_STR(Err::FUNCTION_ALREADY_DEFINED), loc, fcn->nameLen);
                    return Err::FUNCTION_ALREADY_DEFINED;

                }

                loc->idx += wordLen;

                int withoutClosure = 0;

                if (str[loc->idx] != FUNCTION_START) {

                    const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    if (str[loc->idx] != FUNCTION_START) {
                        
                        if (str[loc->idx] == SCOPE_BEGIN) {
                            goto fcnParseScope;
                        }
                        
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    
                    }
                
                }

                loc->idx++;

                int linesSkipped = 0;

                // parse input

                while (1) {

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    // LOOK AT : process this out of the loop for the first time
                    if (str[loc->idx] == FUNCTION_END) {                        
                        loc->idx++;
                        break;
                    }

                    // data type
                    char* const dtypeName = str + loc->idx;
                    const int dtypeLen = findWordEnd(dtypeName);
                    const int dtype = findDataType(dtypeName, dtypeLen);

                    loc->idx += dtypeLen;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    // var name
                    char* const varName = str + loc->idx;
                    const int varLen = findVarEnd(varName);
                    if (varLen <= 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::MISSING_VARIABLE_NAME), loc);
                        return Err::MISSING_VARIABLE_NAME;
                    }

                    const int idx = findVariable(fcn->inArgs, varName, varLen);
                    // Variable* const var = fcn->inArgs[idx];
                    if (idx >= 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, varLen, varLen, varName);
                        return Err::VARIABLE_ALREADY_DEFINED;
                    }

                    loc->idx += varLen;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    Variable* newVar = new Variable;
                    newVar->unrollExpression = 0;
                    newVar->dataTypeEnum = (DataTypeEnum) dtype;
                    newVar->name = varName;
                    newVar->nameLen = varLen;
                    newVar->expression = NULL;
                    newVar->value = NULL;

                    fcn->inArgs.push_back(newVar);

                    const char ch = str[loc->idx];
                    if (ch == ',') {
                        loc->idx++;
                    } else if (ch == FUNCTION_END){
                        loc->idx++;
                        break;                        
                    } else {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    //loc->idx++;
                
                }

                // =>
                linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }
                loc->line += linesSkipped;

                uint16_t tmp = *((uint16_t*) (str + loc->idx));
                if (tmp != ('=' | ('>' << 8))) {
                    
                    if ((tmp & 0xFF) == SCOPE_BEGIN) {
                        goto fcnParseScope;
                    }

                    // LOOK AT : maybe better name for this situation, something like "unexpected char sequence"
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                    return Err::UNEXPECTED_SYMBOL;
                }       

                loc->idx += 2;

                if (str[loc->idx] != FUNCTION_START) {

                    const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    if (str[loc->idx] != FUNCTION_START) {
                        withoutClosure = 1;
                        // Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        // return Err::UNEXPECTED_SYMBOL;
                    } else {
                        loc->idx++;
                    }

                } else {
                    loc->idx++;
                }

                // parse output

                while (1) {

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    loc->line += linesSkipped;

                    // LOOK AT : process this out of the loop for the first time
                    if (str[loc->idx] == FUNCTION_END) {

                        if (withoutClosure) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                            return Err::UNEXPECTED_SYMBOL;
                        }

                        loc->idx++;
                        break;
                    
                    } else if (withoutClosure && str[loc->idx] == SCOPE_BEGIN) {

                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    
                    }

                    // data type
                    char* const dtypeName = str + loc->idx;
                    const int dtypeLen = findVarEnd(dtypeName);
                    const int dtype = findDataType(dtypeName, dtypeLen);

                    loc->idx += dtypeLen;

                    fcn->outArgs.push_back((DataTypeEnum) dtype);

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    const char ch = str[loc->idx];
                    if (ch == ',') {
                        // TODO : handle ERROR
                        loc->idx++;
                    } else if (ch == (withoutClosure ? SCOPE_BEGIN : FUNCTION_END)) {
                        loc->idx++;
                        break;
                    } else {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    }

                }

                fcnParseScope:
                // scope

                if (!withoutClosure) {
                    
                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    loc->line += linesSkipped;

                    if (str[loc->idx] != SCOPE_BEGIN) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    loc->idx++;
                
                }

                Scope* newScope = new Scope;
                newScope->scope = scope;

                const int err = parseScope(newScope, str, loc);
                if (err < 0) return err;

                fcn->bodyScope = newScope;

                scope->children.push_back(fcn);
                scope->fcns.push_back(fcn);
                
                break;

            }

            case KW_IF : {
                
                int err = 0;
                int linesSkipped = 0;

                Scope* newScope = new Scope();
                newScope->scope = scope;

                Operand* newOperand = new Operand(newScope);

                err = parseExpression(newOperand, str, loc, '{');
                if (err < 0) return err;

                err = parseScope(newScope, str, loc);
                if (err < 0) return err;

                Branch* branch = new Branch();
                branch->scope = scope;
                branch->scopes.push_back(newScope);
                branch->expressions.push_back(newOperand);

                scope->children.push_back(branch);

                const int ifWordLen = strlen(KWS_IF);
                const int elseWordLen = strlen(KWS_ELSE);
                while (1) {

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) return Err::OK;

                    char* const word = str + loc->idx;
                    const int wordLen = findWordEnd(word);

                    if (wordLen != elseWordLen) return Err::OK;

                    for (int i = 0; i < wordLen; i++) {
                        if (word[i] != KWS_ELSE[i]) return Err::OK;
                    }

                    loc->idx += wordLen;
                    loc->line += linesSkipped;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    loc->line += linesSkipped;

                    if (str[loc->idx] == SCOPE_BEGIN) {
                        // else case

                        loc->idx++;

                        Scope* newScope = new Scope();
                        newScope->scope = scope;

                        err = parseScope(newScope, str, loc);
                        if (err < 0) return err;

                        branch->scopes.push_back(newScope);

                        return Err::OK;
                    
                    } else {
                        // else if case

                        char* const word = str + loc->idx;
                        const int wordLen = findWordEnd(word);

                        if (wordLen != ifWordLen) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, wordLen);
                            return Err::UNEXPECTED_SYMBOL;
                        }

                        for (int i = 0; i < wordLen; i++) {
                            if (word[i] != KWS_IF[i]) {
                                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc, wordLen);
                                return Err::UNEXPECTED_SYMBOL;
                            }
                        }

                        loc->idx += wordLen;

                        Scope* newScope = new Scope();
                        newScope->scope = scope;

                        Operand* newOperand = new Operand(newScope);
                        
                        err = parseExpression(newOperand, str, loc, '{');
                        if (err < 0) return err;
                        
                        err = parseScope(newScope, str, loc);
                        if (err < 0) return err;

                        branch->scopes.push_back(newScope);
                        branch->expressions.push_back(newOperand);

                    }


                }


                break;

            }

            case KW_FOR : {

                break;

            }

            case KW_WHILE : {

                int err = 0;
                int linesSkipped = 0;

                Scope* newScope = new Scope();
                newScope->scope = scope;

                Operand* newOperand = new Operand(newScope);

                err = parseExpression(newOperand, str, loc, '{');
                if (err < 0) return err;

                err = parseScope(newScope, str, loc);
                if (err < 0) return err;

                WhileLoop* loop = new WhileLoop();
                loop->scope = scope;
                loop->bodyScope = newScope;
                loop->expression = newOperand;

                scope->children.push_back(loop);

                break;

            }

            case KW_GOTO : {


                break;

            }

            case KW_ENUM : {

                // enum <name> : <type> { .. }
                
                int err = 0;
                int linesSkipped = 0;

                linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }

                char* const name = str + loc->idx;
                const int nameLen = findVarEnd(name);

                loc->idx += nameLen;

                linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }

                DataTypeEnum dtype;
                const char tmpCh = str[loc->idx];
                if (tmpCh == ':') {
                    
                    loc->idx++;
                    
                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    char* const dtypeName = str + loc->idx;
                    const int dtypeNameLen = findVarEnd(dtypeName);

                    loc->idx += dtypeNameLen;

                    dtype = (DataTypeEnum) findDataType(dtypeName, dtypeNameLen);
                    if (dtype == DT_UNDEFINED) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNKNOWN_DATA_TYPE));
                        return Err::UNKNOWN_DATA_TYPE;
                    } else if (dtype < DT_INT || dtype > DT_INT_64) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_DATA_TYPE));
                        return Err::INVALID_DATA_TYPE;
                    }

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    if (str[loc->idx] != '{') {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;
                    }

                    loc->idx++;

                } else if (tmpCh == '{') {

                    dtype = DT_INT;
                    loc->idx++;

                } else {
                    
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                    return Err::UNEXPECTED_SYMBOL;
                
                }

                Enumerator* enumerator = new Enumerator();
                enumerator->name = name;
                enumerator->nameLen = nameLen;
                enumerator->dtype = dtype;
                enumerator->loc = getLocationStamp(loc);

                uint64_t lastValue = -1;//((uint64_t) 0) - ((uint64_t) 1);
                while (1) {
                    
                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    if (str[loc->idx] == SCOPE_END) {
                        loc->idx++;
                        break;
                    }

                    char* const word = str + loc->idx;
                    const int wordLen = findVarEnd(word);

                    loc->idx += wordLen;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }

                    VariableDefinition* newVarDef = new VariableDefinition();
                    Variable* newVar = new Variable(scope, dtype, loc);

                    newVarDef->loc = getLocationStamp(loc);
                    newVarDef->flags = 0;

                    newVar->def = newVarDef;
                    newVar->name = word;
                    newVar->nameLen = wordLen;

                    enumerator->vars.push_back(newVar);

                    const char ch = str[loc->idx];
                    if (ch == '=') {

                        loc->idx++;

                        err = parseExpression(newVar, str, loc, ',');
                        if (err < 0) return err;

                        const int expDtype = newVar->expression->evaluate((Operand**) &newVar, NULL);
                        if (newVar->unrollExpression) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::COMPILE_TIME_KNOWN_EXPRESSION_REQUARIED));
                            return Err::COMPILE_TIME_KNOWN_EXPRESSION_REQUARIED;
                        } else if (expDtype < DT_INT || expDtype > DT_INT_64) {
                            // need to cast
                            cast(newVar, (DataTypeEnum) dtype);
                        }

                        lastValue = (uint64_t) *((uint64_t*) &(newVar->value));

                    } else if (ch == ',') {

                        lastValue++;
                        newVar->value = (void*) *((void**) &lastValue);
                        
                        loc->idx++;

                    } else if (ch == SCOPE_END) {

                        lastValue++;
                        newVar->value = (void*) *((void**)&lastValue);
                        
                        loc->idx++;
                        break;
                    
                    } else {
                        
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;    
                    
                    }

                }

                scope->children.push_back(enumerator);

                break;
            
            }

            case KW_TYPE_DEF : {

                int err = 0;
                int linesSkipped = 0;

                linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }
                loc->line += linesSkipped;

                char* const name = str + loc->idx;
                const int nameLen = findVarEnd(name);

                loc->idx += nameLen;

                linesSkipped = skipWhiteSpaces(str, &loc->idx);
                if (linesSkipped < 0) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                    return Err::UNEXPECTED_END_OF_FILE;
                }
                loc->line += linesSkipped;

                if (str[loc->idx] != SCOPE_BEGIN) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                    return Err::UNEXPECTED_SYMBOL;
                }
                loc->line += linesSkipped;

                TypeDefinition* newTypeDefinition = new TypeDefinition();
                newTypeDefinition->scope = scope;
                newTypeDefinition->name = name;
                newTypeDefinition->nameLen = nameLen;

                customDataTypes.push_back(newTypeDefinition);
                scope->customDataTypes.push_back(newTypeDefinition);
                scope->children.push_back(newTypeDefinition);

                loc->idx++;
                while (1) {
                    
                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    if (str[loc->idx] == SCOPE_END) {
                        // TODO : out of the loop?
                        loc->idx++;
                        break;
                    }

                    Location* const startLoc = getLocationStamp(loc);

                    char* const dtypeName = str + loc->idx;
                    const int dtypeNameLen = findVarEnd(dtypeName);

                    loc->idx += dtypeNameLen;

                    // TODO : if enum changes has to be rewriten (>=/<= maybe with these things)
                    DataTypeEnum dtype = (DataTypeEnum) findDataType(dtypeName, dtypeNameLen);
                    if (dtype < 0 || dtype == DT_MULTIPLE_TYPES) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_DATA_TYPE));
                        return Err::INVALID_DATA_TYPE;
                    }
                    if (dtype == DT_UNDEFINED) dtype = DT_CUSTOM;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    char* const varName = str + loc->idx;
                    const int varNameLen = findVarEnd(varName);

                    const int idx = findVariable(newTypeDefinition->vars, varName, varNameLen);
                    if (idx >= 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::VARIABLE_ALREADY_DEFINED), loc, varNameLen, varNameLen, varName);
                        return Err::VARIABLE_ALREADY_DEFINED;
                    }

                    // Variable* var = newTypeDefinition->vars[idx];

                    loc->idx += varNameLen;

                    linesSkipped = skipWhiteSpaces(str, &loc->idx);
                    if (linesSkipped < 0) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                        return Err::UNEXPECTED_END_OF_FILE;
                    }
                    loc->line += linesSkipped;

                    Variable* newVar = new Variable(scope, dtype, loc);
                    newVar->name = varName;
                    newVar->nameLen = varNameLen;
                    newVar->flags = 0;

                    if (dtype == DT_CUSTOM) {
                        VariableDefinition* newVarDef = new VariableDefinition();
                        newVarDef->var = newVar;
                        newVarDef->dtypeName = dtypeName;
                        newVarDef->dtypeNameLen = dtypeNameLen;
                        newVarDef->flags = 0;
                        newVarDef->scope = scope;
                        newVarDef->loc = startLoc;
                        customDataTypesReferences.push_back(newVarDef);
                    }

                    newTypeDefinition->vars.push_back(newVar);

                    const char ch = str[loc->idx];
                    if (ch == '=') {

                        loc->idx++;

                        err = parseExpression(newVar, str, loc, ';');
                        if (err < 0) return err;

                        // TODO : maybe do it after parsing with other evaluatings 
                        /*
                        const int expDtype = newVar->expression->evaluate(newVar);
                        if (newVar->unrollExpression) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::COMPILE_TIME_KNOWN_EXPRESSION_REQUARIED));
                            return Err::COMPILE_TIME_KNOWN_EXPRESSION_REQUARIED;
                        }
                        */

                    } else if (ch == ';') {

                        loc->idx++;

                    } else if (ch == SCOPE_END) {

                        loc->idx++;
                        break;
                    
                    } else {
                        
                        Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL));
                        return Err::UNEXPECTED_SYMBOL;    
                    
                    }

                }
                

                break;
            }

        }

        return Err::OK;

    }

    // G as gneral
    enum {
        G_NONE,
        G_VARIABLE,
        G_STRING_LITERAL,
        G_NUMBER_LITERAL,
        G_OPERATOR,
        G_SUB_EXPRESSION
    };

    // expression is something like this
    // x = 10 * 1 + y + z * sin(u) + tmp[0];
    // states
    // 0 -> unary operator or value/var is expected
    // 1 -> binary operator or value/var is expected
    // expresion has to have at least one operator, otherwise it has to be NULL and value is represented directly in variable
    int parseExpression(Operand* operand, char* const str, Location* const loc, const uint16_t endChar) {

        int lastOperatorRank = 0;
        UnaryExpression* lastUnaryExpression = NULL;
        BinaryExpression* lastBinaryExpression = NULL;
        int lastType = G_NONE;
        int lastOperator = -1;
        int lastOperandType = DT_UNDEFINED;

        const char endCharA = (char) endChar;
        const char endCharB = (endChar > 255) ? endChar >> 8 : endCharA;

        while(1) {

            const char ch = str[loc->idx];

            if (ch == endCharA || ch == endCharB) {
                // end of expression

                // TODO: validate

                if (lastType == G_OPERATOR) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_EXPRESSION), loc);
                    return Err::UNEXPECTED_END_OF_EXPRESSION;
                }

                loc->idx++;

                return Err::OK;

            } else if (ch == '(') {
                // subexpression start

                Operand* newOperand = new Operand();
                newOperand->dataTypeEnum = DT_UNDEFINED; // TODO : exchange for UNDEFINED or whatever!!!!!
                newOperand->scope = operand->scope;
                newOperand->unrollExpression = 1;
                newOperand->value = NULL;

                loc->idx++;
                parseExpression(newOperand, str, loc, ')');

                if (lastType == G_NONE) {
                
                    // TODO: typecheck
                    WrapperExpression* ex = new WrapperExpression();
                    ex->operand = newOperand;
                    operand->expression = ex;

                } else {

                    if (lastUnaryExpression) lastUnaryExpression->operand = newOperand;
                    else ((BinaryExpression*) operand->expression)->operandB = newOperand;

                }

                loc->idx--;
                lastType = G_SUB_EXPRESSION;

            } else if (ch == SCOPE_BEGIN) {
                // custom type

                // TypeInitialization* typeInit;

                Operand* newOperand = new Operand();
                // newOperand->expression = typeInit;
                newOperand->dataTypeEnum = DT_CUSTOM;
                newOperand->unrollExpression = 0;

                loc->idx++;

                const int err = parseTypeInitialization(operand->scope, str, loc, (TypeInitialization**) &(newOperand->expression));
                if (err < 0) return err;

                if (lastType == G_NONE) {

                    // TODO: typecheck
                    WrapperExpression* ex = new WrapperExpression();
                    ex->operand = newOperand;
                    operand->expression = ex;

                } else {

                    if (lastUnaryExpression) lastUnaryExpression->operand = newOperand;
                    else ((BinaryExpression*)operand->expression)->operandB = newOperand;

                }

                loc->idx--;

            } else if (ch > 32) {
                // word found

                // LOOK AT : check also for '_'?
                if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
                    // variable or function call

                    char* const word = str + loc->idx;
                    const int wordLen = findVarEnd(word + 1) + 1;

                    // decide var or fcn
                    int isVar = 1;
                    int lines = 0;
                    int idx = 0;
                    for (; ; idx++) {
                        const char ch = *(word + wordLen + idx);
                        if (ch == '\n') {
                            lines++;
                        } else if (ch > ' ') {
                            if (ch == FUNCTION_START) isVar = 0;
                            break;
                        }
                    }

                    if (isVar) {
                        // have to be already defined
                        // have to copy variable

                        Variable* var;
                        const int err = parseVariable(operand->scope, str, loc, &var);
                        if (err < 0) return err;

                        Operand* newOperand;

                        // TODO : mess
                        if (var->def->flags & IS_CMPTIME) {
                            // we can treat it just like a Operand with a singl const value, dont we?
                            newOperand = new Operand();
                            copy(newOperand, var);
                        } else {
                            /*
                            newOperand = new Variable(loc);
                            ((Variable*) newOperand)->def = var->def;
                            ((Variable*) newOperand)->unrollExpression = 0;
                            copy((Variable*) newOperand, var);
                            */
                            newOperand = (Variable*) var;
                            newOperand->unrollExpression = 0;

                        }
                        
                        if (lastType == G_NONE) {
                        
                            if (operand->dataTypeEnum == DT_CUSTOM || newOperand->dataTypeEnum == DT_CUSTOM) {
                                // LOOK AT : is it enough?
                                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION), loc, wordLen, wordLen, word);
                                return Err::INVALID_TYPE_CONVERSION;
                            }
                            WrapperExpression* ex = new WrapperExpression();
                            ex->operand = newOperand;
                            operand->expression = ex;

                        } else {

                            if (lastUnaryExpression) {
                                lastUnaryExpression->operand = newOperand;
                            } else {
                                ((BinaryExpression*) operand->expression)->operandB = newOperand;
                            }

                        }

                        loc->idx--;

                    } else {
                        // doesnt have to be already defined!!!
                        // TODO : rewrite it to make it posiible!!!!!

                        // just to work variant!!!!
                        // Function* const fcn = findFunction(operand->scope, word, wordLen);
                         
                        FunctionCall* fcnCall = new FunctionCall;
                        fcnCall->fcn = NULL;
                        fcnCall->name = word;
                        fcnCall->nameLen = wordLen;

                        // parse input
                        loc->idx += wordLen + 1;
                        while (1) {

                            const int linesSkipped = skipWhiteSpaces(str, &loc->idx);
                            if (linesSkipped < 0) {
                                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                                return Err::UNEXPECTED_END_OF_FILE;
                            }

                            loc->line += linesSkipped;

                            // LOOK AT : process this out of the loop for the first time
                            if (str[loc->idx] == FUNCTION_END) break;

                            // LOOK AT : is Variable needed? maybe use just Operand 
                            Variable* newOperand = new Variable(operand->scope, DT_INT_64, loc);
                            /*
                            newOperand->name = NULL;
                            newOperand->nameLen = 0;
                            newOperand->expression = NULL;
                            newOperand->scope = operand->scope;
                            newOperand->unrollExpression = 1;
                            // TODO : !!!!!!!!!!!!!!!!!!!!!!!!!!!
                            newOperand->dataTypeEnum = DT_INT_64;
                            newOperand->value = malloc(sizeof(uint64_t));
                            */

                            int err = parseExpression(newOperand, str, loc, ',' + (FUNCTION_END << 8));
                            if (err < 0) return err;

                            fcnCall->inArgs.push_back(newOperand);

                            loc->idx--;
                            const char ch = str[loc->idx]; // LOOK AT : could be zero?
                            if (ch == ',') {
                                loc->idx++;
                            } else if (ch == FUNCTION_END) {
                                break;                        
                            } else {
                                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_SYMBOL), loc);
                                return Err::UNEXPECTED_SYMBOL;
                            }
                        
                        }

                        Operand* newOperand = new Operand;
                        newOperand->dataTypeEnum = DT_UNDEFINED; // TODO : exchange for UNDEFINED or whatever!!!!!
                        newOperand->expression = fcnCall;
                        newOperand->scope = operand->scope;
                        newOperand->unrollExpression = 0;
                        newOperand->value = NULL;
                    
                        if (lastType == G_NONE) {
                        
                            // TODO: typecheck
                            WrapperExpression* ex = new WrapperExpression();
                            ex->operand = newOperand;
                            operand->expression = ex;

                        } else {

                            if (lastUnaryExpression) lastUnaryExpression->operand = newOperand;
                            else ((BinaryExpression*) operand->expression)->operandB = newOperand;

                        }

                        fcnCalls.push_back(newOperand);

                    }

                    lastType = G_VARIABLE;
                
                } else if (ch >= '0' && ch <= '9' || ch == '.') {
                    // number literal

                    if (lastType != G_OPERATOR && lastType != G_NONE) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_NUMBER_LITERAL), loc);
                        return Err::INVALID_NUMBER_LITERAL;
                    }

                    uint64_t value;
                    DataTypeEnum dataType = parseNumberLiteral(str + loc->idx, &(loc->idx), &value);
                    if (dataType == DT_VOID) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_NUMBER_LITERAL), loc);
                        return Err::INVALID_NUMBER_LITERAL;
                    }

                    Variable* newVar = new Variable(operand->scope, dataType, loc);
                    newVar->value = (void*) value;

                    if (lastType == G_NONE) {

                        WrapperExpression* ex = new WrapperExpression();
                        ex->operand = newVar;
                        operand->expression = ex;

                        /*                       
                        if (operand->dataTypeEnum == DT_CUSTOM || castBasicDataType(operand->dataTypeEnum, dataType, (uint64_t*) &operand->value, value)) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION), loc, str);
                            return Err::INVALID_TYPE_CONVERSION;
                        }
                        */

                    } else {
                        
                        newVar->unrollExpression = 0; // LOOK AT : do we have to set it?
                        
                        if (lastUnaryExpression) lastUnaryExpression->operand = newVar;
                        else ((BinaryExpression*) operand->expression)->operandB = newVar;

                    }

                    lastType = G_NUMBER_LITERAL;

                    loc->idx -= 1;

                } else if (ch == '"') {
                    // string

                    lastType = G_STRING_LITERAL;
                
                } else {
                    // operator

                    // operator can be merged with other word...
                    uint32_t word = ch;
                    int wordLen = 1;
                    for (int i = loc->idx + 1; i < loc->idx + 4; i++) {
                        const char ch = str[i];
                        // TODO : change that, use #define maybe
                        if (ch <= ' ' || ch == ';' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '.' || ch == '(' || ch == ')') {
                            break;
                        }
                        wordLen++;
                        word = (word << 8) + ch;
                    }

                    Operator* op = NULL;
                    OperatorEnum opType;
                    for (int i = 0; i < OPERATORS_COUNT; i++) {
                        if ((operators + i)->compare(operators + i, word)) {
                            opType = (OperatorEnum) i;
                            op = operators + i;
                            break;
                        }
                    }

                    if (!op) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_OPERATOR), loc, wordLen, wordLen, &word);
                        return Err::INVALID_OPERATOR;
                    }

                    if (opType == OP_ADDRESS && lastOperator == OP_ADDRESS) {
                        Logger::log(Logger::ERROR, ERR_STR(Err::CANNOT_GET_ADDRESS_OF_ADDRESS), loc, wordLen);
                        return Err::CANNOT_GET_ADDRESS_OF_ADDRESS;
                    }

                    lastOperator = opType;

                    if (lastType == G_NONE) {

                        if (!(op->flag & IS_UNARY)) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::OPERATOR_CANNOT_ACT_AS_UNARY), loc, wordLen, wordLen, &op->word);
                            return Err::OPERATOR_CANNOT_ACT_AS_UNARY;
                        }
                        
                        UnaryExpression* uEx = new UnaryExpression;
                        uEx->operand = NULL;
                        uEx->oper = op;

                        operand->expression = uEx;
                        lastUnaryExpression = uEx;
                        
                    } else if (lastType == G_OPERATOR) {

                        if (!(op->flag & IS_UNARY)) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::OPERATOR_CANNOT_ACT_AS_UNARY), loc, wordLen, wordLen, &op->word);
                            return Err::OPERATOR_CANNOT_ACT_AS_UNARY;
                        }

                        UnaryExpression* uEx = new UnaryExpression;
                        uEx->operand = NULL;
                        uEx->oper = op;

                        Variable* newVar = new Variable(loc);
                        // newVar->unrollExpression = 1;
                        newVar->scope = operand->scope;
                        //newVar->name = NULL;
                        //newVar->nameLen = 0;
                        newVar->expression = uEx;

                        if (lastUnaryExpression == NULL) {
                            // LOOK_AT: is it allways be binary at this point?
                            ((BinaryExpression*) operand->expression)->operandB = newVar;
                        } else {
                            lastUnaryExpression->operand = newVar;
                        }

                        lastUnaryExpression = uEx;

                    } else {

                        if (!(op->flag & IS_BINARY)) {
                            Logger::log(Logger::ERROR, ERR_STR(Err::OPERATOR_CANNOT_ACT_AS_BINARY), loc, wordLen, wordLen, &op->word);
                            return Err::OPERATOR_CANNOT_ACT_AS_BINARY;
                        }

                        lastUnaryExpression = NULL;

                        BinaryExpression* bEx = new BinaryExpression;

                        Variable* newVar = new Variable(loc);
                        //newVar->unrollExpression = 1;
                        newVar->scope = operand->scope;
                        //newVar->name = NULL;
                        //newVar->nameLen = 0;
                        //newVar->expression = NULL;

                        // LOOK_AT: maybe change design to allways somehow have root as expression, so we dont have to check or something
                        const int operatorRank = op->rank;
                        if (lastOperatorRank > operatorRank) {

                            newVar->expression = bEx;

                            Variable* tmpVar = (Variable*) (((BinaryExpression*) operand->expression)->operandB);
                            ((BinaryExpression*) operand->expression)->operandB = newVar;
                            bEx->operandA = tmpVar;

                            operand = newVar;

                        } else if (operand->expression) {

                            newVar->expression = operand->expression;
                            bEx->operandA = newVar;
                        
                        } else {
                            
                            newVar->dataTypeEnum = operand->dataTypeEnum;
                            // newVar->dataType = operand->dataType;
                            newVar->value = operand->value;
                            newVar->expression = NULL;
                            bEx->operandA = newVar;
                        
                        }

                        operand->expression = bEx;
                        bEx->operandB = NULL;
                        bEx->oper = op;

                        lastOperatorRank = operatorRank;
                        lastBinaryExpression = bEx;

                    }

                    lastType = G_OPERATOR;
                
                }

            } else if (ch == '\n') {

                loc->line++;
            
            } else if (ch == '\0') {

                Logger::log(Logger::ERROR, ERR_STR(Err::UNEXPECTED_END_OF_FILE));
                return Err::UNEXPECTED_END_OF_FILE;
            
            }

            loc->idx++; // LOOK AT : get rid of it???

        }

    }

    int parseOperator(Operator** op) {

        /*
            // maybe switch for array
            switch (op->word) {

                case '+' :
                    *op = new operators[];
                    break;



            }
        */

       return 1;


    }

    int parseString(char* const str, const int len) {

        return 0;

    }

    // maybe better name?
    DataTypeEnum str2dtype(char* const str, const int len) {

        // 0 -> int
        // 1 -> float
        int typeCategory = 0;
        for (int i = 0; i < len; i++) {
            if (str[i] == '.') {
                typeCategory = 1;
                break;
            }
        }

        // TODO
        return DT_INT;

    }

    void* dtype2val(DataTypeEnum dataType, char* const str, const int len) {

        switch (dataType) {

            case DT_INT : {
                
            }

            case DT_INT_32 : {

                int endIdx;
                long val = Utils::str2int(str, len, &endIdx);

                if (endIdx != len) {
                    // TODO
                    return NULL;
                }

                int32_t* pval = (int32_t*) malloc(sizeof(int32_t));
                *pval = val;

                return pval;
                
            }

            case DT_INT_64 : {

                int endIdx;
                long val = Utils::str2int(str, len, &endIdx);

                if (endIdx != len) {
                    // TODO
                    return NULL;
                }

                int64_t* pval = (int64_t*) malloc(sizeof(int64_t));
                *pval = val;

                return pval;
                
            }

            default : {
                return NULL;
            }

        }

    }

    void copy(Variable* dest, Variable* src) {

        dest->name = src->name;
        dest->nameLen = src->nameLen;
        dest->dataTypeEnum = src->dataTypeEnum;
        dest->expression = src->expression;
        dest->scope = src->scope;
        dest->unrollExpression = src->unrollExpression;
        dest->value = src->value;

    }

    
    void copy(Operand* dest, Variable* src) {

        dest->dataTypeEnum = src->dataTypeEnum;
        dest->expression = src->expression;
        dest->scope = src->scope;
        dest->unrollExpression = src->unrollExpression;
        dest->value = src->value;

    }

    // LOOK AT : templates, or meh?
    // TODO : refactor!!!
    Variable* findVariable(Scope* scope, char* const name, const int nameLen) {
        
        while (scope) {

            std::vector<Variable*> vars = scope->vars;

            for (int i = 0; i < (int) vars.size(); i++) {

                if (nameLen != vars[i]->nameLen) continue;
                
                char* const varName = vars[i]->name;
                const int varNameLen = vars[i]->nameLen;
                
                int j = 0;
                for (; j < varNameLen; j++) {
                    if (varName[j] != name[j]) break;
                }

                if (j != varNameLen) continue;
                else return vars[i];
            
            }

            scope = scope->scope;
        
        }

        return NULL;

    }

    TypeDefinition* findCustomVariable(Scope* scope, char* const name, const int nameLen) {
        
        while (scope) {

            std::vector<TypeDefinition*> vars = scope->customDataTypes;

            for (int i = 0; i < (int) vars.size(); i++) {

                if (nameLen != vars[i]->nameLen) continue;
                
                char* const varName = vars[i]->name;
                const int varNameLen = vars[i]->nameLen;
                
                int j = 0;
                for (; j < varNameLen; j++) {
                    if (varName[j] != name[j]) break;
                }

                if (j != varNameLen) continue;
                else return vars[i];
            
            }

            scope = scope->scope;
        
        }

        return NULL;

    }

    // LOOK AT : not so fancy, maybe refactor
    INamed* findVarOrFcn(Scope* scope, char* const name, const int nameLen) {
        
        while (scope) {

            std::vector<Variable*> vars = scope->vars; 
            
            for (int i = 0; i < (int) vars.size(); i++) {

                if (nameLen != vars[i]->nameLen) continue;
                
                char* const varName = vars[i]->name;
                const int varNameLen = vars[i]->nameLen;
                
                int j = 0;
                for (; j < varNameLen; j++) {
                    if (varName[j] != name[j]) break;
                }

                if (j != varNameLen) continue;
                else return vars[i];
            
            }

            std::vector<Function*> fcns = scope->fcns;

            for (int i = 0; i < (int) fcns.size(); i++) {

                if (nameLen != fcns[i]->nameLen) continue;
            
                char* const fcnName = fcns[i]->name;
                const int fcnNameLen = fcns[i]->nameLen;

                int j = 0;
                for (; j < nameLen; j++) {
                    if (fcnName[j] != name[j]) break;
                }

                if (j != nameLen) continue;
                else return fcns[i];
            
            }

            scope = scope->scope;
        
        }

        return NULL;

    }

    // LOOK AT : templates, or meh?

    int findVariable(std::vector<Variable*> vars, char* const name, const int nameLen) {
        
        for (int i = 0; i < (int) vars.size(); i++) {

            if (nameLen != vars[i]->nameLen) continue;
            
            char* const varName = vars[i]->name;
            const int varNameLen = vars[i]->nameLen;

            int j = 0;
            for (; j < nameLen; j++) {
                if (varName[j] != name[j]) break;
            }

            if (j != nameLen) continue;
            else return i;
            
        
        }

        return -1;

    }

    // LOOK AT : template
    Function* findFunction(Scope* scope, char* const name, const int nameLen) {

        while (scope) {

            std::vector<Function*> fcns = scope->fcns;

            for (int i = 0; i < (int) fcns.size(); i++) {

                if (nameLen != fcns[i]->nameLen) continue;
                
                char* const fcnName = fcns[i]->name;
                const int fcnNameLen = fcns[i]->nameLen;
                
                int j = 0;
                for (; j < nameLen; j++) {
                    if (fcnName[j] != name[j]) break;
                }

                if (j != nameLen) continue;
                else return fcns[i];
            
            }

            scope = scope->scope;
        
        }

        return NULL;

    }

    uint32_t findDataType(char* const name, const int nameLen) {

        for (int i = 0; i < DATA_TYPES_COUNT; i++) {
            
            const DataType* dtype = dataTypes + i;
            if (dtype->nameLen != nameLen) continue;

            const char* word = dtype->name;

            int j = 0;
            for (; j < nameLen; j++) {
                if (word[j] != name[j]) break;
            }

            if (j != nameLen) continue;
            else return i;

        }

        return DT_UNDEFINED;

    }

    TypeDefinition* findCustomDataType(Scope* scope, char* const name, const int nameLen) {
        
        while (scope) {

            std::vector<TypeDefinition*> dtypes = scope->customDataTypes;

            for (int i = 0; i < (int) dtypes.size(); i++) {

                if (nameLen != dtypes[i]->nameLen) continue;
                
                char* const dtypeName = dtypes[i]->name;
                const int dtypeNameLen = dtypes[i]->nameLen;
                
                int j = 0;
                for (; j < nameLen; j++) {
                    if (dtypeName[j] != name[j]) break;
                }

                if (j != nameLen) continue;
                else return dtypes[i];
            
            }

            scope = scope->scope;
        
        }

        return NULL;

    }

    // WARNING : typechek before!!! does not check anything!!!
    void copyValue(Operand* const dest, Operand* const src) {

        dest->value = malloc(8); // TODO : !!!!!!!!!!!!!!!!!!!!!!!
        memcpy(dest->value, src->value, (dataTypes + src->dataTypeEnum)->size);

    }








    // utils, maybe separate namespace

    int strcmp(char* strA, char* strB);
    
    // TODO: make ';' const or define
    // TODO: make ' ' const or define
    int findWordEnd(char* const str) {

        for (int i = 0; ; i++) {
            const char ch = str[i];
            if (ch <= ' ' || ch == ';') {
                return i;
            }
        }
    
    }

    int findWordEndOrChar(char* const str, const char ch) {

        for (int i = 0; ; i++) {
            const char sch = str[i];
            if (sch == ch || sch <= ' ' || sch == ';') {
                return i;
            }
        }
    
    }

    int findVarEnd(char* const str) {

        for (int i = 0; ; i++) {
            const char ch = str[i];
            // ((ch < '0' || ch > 'z') || (ch > '9' && ch < 'A') || (ch != '_' && (ch > 'Z' && ch < 'a')))
            if (!((ch >= '0' && ch <= '9') || ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'))) {
                return i;
            }
        }
    
    }

    enum LiteralType {
        LT_INT_HEX,
        LT_INT_BIN,
        LT_INT,
        LT_FLOAT,
        LT_CHAR,
        LT_STRING
    };


    // returns data type (DT_VOID on error)
    DataTypeEnum parseNumberLiteral(char* const str, int* idx, uint64_t* out) {

        const int16_t tmp = ((int16_t*) str)[0];
        if (tmp == (('x' << 8) + '0')) {
            // int-hex

            uint64_t num = 0;
            for (int i = 2; ; i++) {

                const char ch = str[i];
                
                if (ch >= '0' && ch <= '9') num = num * 16 + (ch - '0');
                else if (ch >= 'A' && ch <= 'F') num = num * 16 + (10 + ch - 'A');
                else if (ch >= 'a' && ch <= 'f') num = num * 16 + (10 + ch - 'a');
                else { *idx += i; *out = num; return DT_INT_64; }
            
            }

        } else if (tmp == (('b' << 8) + '0')) {
            // int-bin

            uint64_t num = 0;
            for (int i = 2; ; i++) {

                const char ch = str[i];
                if (ch == '0' || ch == '1') num = num * 2 + (ch - '0');
                else { *idx += i; *out = num; return DT_INT_64; }
            
            }

        } else {

            uint64_t integer = 0;
            for (int i = 0; ; i++) {

                const char ch = str[i];

                if (ch >= '0' && ch <= '9') integer = integer * 10 + (ch - '0');
                else {

                    if (ch == '.') {
                        // float
                        
                        uint64_t decimal = 0;
                        uint64_t base = 1;
                        for (int j = i + 1; ; j++) {

                            const char ch = str[j];
                            
                            if (ch >= '0' && ch <= '9') decimal = decimal * 10 + (ch - '0');
                            else {

                                if (ch == 'f') {

                                    *idx += j + 1;
                                    *((float_t*) out) = (float_t) (integer + (decimal / (double) base));
                                    return DT_FLOAT_32;
                                
                                } else {

                                    *idx += j;
                                    *((double_t*) out) = (double_t) (integer + (decimal / (double) base));
                                    return DT_FLOAT_64;
                                
                                }

                            }

                            base *= 10;

                        }

                    } else {

                        if (ch == 'f') {

                            *idx += i + 1;
                            *((float_t*) out) = (float_t) integer; 
                            return DT_FLOAT_32; 
                        
                        
                        } else { 

                            *idx += i;
                            *out = integer;
                            return DT_INT_64;
                        
                        }

                    }
                
                }

            }

        }

        return DT_VOID;

    }

    int findNumberEnd(char* const str) {

        for (int i = 0; ; i++) {
            const char ch = str[i];
            if (!(ch >= '0' && ch <= '9')) {
                return i;
            }
        }

    }

    // TODO : refactor!!

    template <typename T>
    uint64_t castToDataType(DataTypeEnum dtype, T value) {

        #define CAST_AND_RETURN(dtype, value) { dtype tmp = (dtype) value; return *((uint64_t*) &tmp); }
        
        switch (dtype) {
            
            case DT_INT:        CAST_AND_RETURN(int32_t, value)
            case DT_INT_32 :    CAST_AND_RETURN(int32_t, value)
            case DT_INT_64 :    CAST_AND_RETURN(int64_t, value)
            case DT_FLOAT_32 :  CAST_AND_RETURN(float_t, value)
            case DT_FLOAT_64 :  CAST_AND_RETURN(double_t, value)
            default: return (uint64_t) (value);
        
        }
    
    }

    // return 0 on success
    int cast(Operand* opTo, Operand* opFrom) {

        if (opTo->dataTypeEnum == DT_CUSTOM || opFrom->dataTypeEnum == DT_CUSTOM) return 1;

        DataType* const dtypeTo = dataTypes + opTo->dataTypeEnum;
        DataType* const dtypeFrom = dataTypes + opFrom->dataTypeEnum;

        switch (opFrom->dataTypeEnum) {
            
            case DT_INT :
                opTo->value = (void*) castToDataType(opTo->dataTypeEnum, *((int32_t*) &opFrom->value));            
                break;

            case DT_INT_32 :
                opTo->value = (void*) castToDataType(opTo->dataTypeEnum, *((int32_t*) &opFrom->value));
                break;

            case DT_INT_64 :
                opTo->value = (void*) castToDataType(opTo->dataTypeEnum, *((int64_t*) &opFrom->value));            
                break;

            case DT_FLOAT_32 :
                opTo->value = (void*) castToDataType(opTo->dataTypeEnum, *((float_t*) &opFrom->value));            
                break;
        
            case DT_FLOAT_64 :
                opTo->value = (void*) castToDataType(opTo->dataTypeEnum, *((double_t*) &opFrom->value));
                break;

            default:
                return 2;
        
        }

        
        opTo->dataTypeEnum = opFrom->dataTypeEnum;
        return 0;

    }

    int castBasicDataType(DataTypeEnum dtypeTo, DataTypeEnum dtypeFrom, uint64_t* valueTo, uint64_t valueFrom) {

        switch (dtypeFrom) {
            
            case DT_INT :
                *valueTo = (uint64_t) castToDataType(dtypeTo, *((int32_t*) &valueFrom));            
                break;

            case DT_INT_32 :
                *valueTo = (uint64_t) castToDataType(dtypeTo, *((int32_t*) &valueFrom));
                break;

            case DT_INT_64 :
                *valueTo = (uint64_t) castToDataType(dtypeTo, *((int64_t*) &valueFrom));            
                break;

            case DT_FLOAT_32 :
                *valueTo = (uint64_t) castToDataType(dtypeTo, *((float_t*) &valueFrom));          
                break;
        
            case DT_FLOAT_64 :
                *valueTo = (uint64_t) castToDataType(dtypeTo, *((double_t*) &valueFrom));
                break;

            default:
                return 2;
        
        }

        return 0;

    }

    // casts op to dtype
    int cast(Operand* const op, const DataTypeEnum dtype) {

        switch (op->dataTypeEnum) {
            
            case DT_INT :
                op->value = (uint64_t*) castToDataType(dtype, *((int32_t*) &op->value));            
                break;

            case DT_INT_32 :
                op->value = (uint64_t*) castToDataType(dtype, *((int32_t*) &op->value));
                break;

            case DT_INT_64 :
                op->value = (uint64_t*) castToDataType(dtype, *((int64_t*) &op->value));            
                break;

            case DT_FLOAT_32 :
                op->value = (uint64_t*) castToDataType(dtype, *((float_t*) &op->value));          
                break;
        
            case DT_FLOAT_64 :
                op->value = (uint64_t*) castToDataType(dtype, *((double_t*) &op->value));
                break;

            default:
                return 2;
        
        }

        op->dataTypeEnum = dtype;

        return 0;

    }

    // has to return DataTypeEnum of transformed operand on sucess
    // to be 'compatible' with evaluate function!
    int applyOperator(Operator* op, Operand* operand) {

        DataType* const dtype = dataTypes + operand->dataTypeEnum;
        switch (op->word) {
            
            case '+' :
                
                if (!dtype->unaryPlus) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_UNARY_OPERATOR), operand->loc, 1, dtype->nameLen, dtype->name, 1, "+");
                    return Err::INVALID_TYPE_UNARY_OPERATOR;
                }
                
                dtype->unaryPlus(operand);
                
                break;

            case '-' :
                
                if (!dtype->unaryMinus) {
                    Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_UNARY_OPERATOR), operand->loc, 1, dtype->nameLen, dtype->name, 1, "-");
                    return Err::INVALID_TYPE_UNARY_OPERATOR;
                }

                dtype->unaryMinus(operand);
                
                break;
            
            case '&' :

                dtype->address(operand);
                break;

            default:
                return 2;
        
        }

        return operand->dataTypeEnum;

    }

    // has to return DataTypeEnum of transformed operand on sucess
    // to be 'compatible' with evaluate function!
    // a op b
    int applyOperator(Operator* op, Operand* a, Operand* b) {

        DataType* const dtype = dataTypes + a->dataTypeEnum;
        switch (op->word) {
            
            case '+' :
                dtype->addition(a, b);
                break;

            case '-' :
                dtype->subtraction(a, b);
                break;

            case '*' :
                dtype->multiplication(a, b);
                break;
            
            case '/' :
                dtype->division(a, b);
                break;

            default:
                return 2;
        
        }

        return a->dataTypeEnum;

    }

    Location* getLocationStamp(Location* loc) {

        Location* stamp = (Location*) malloc(sizeof(Location));
        if (!stamp) return NULL;

        stamp->file = loc->file;
        stamp->idx = loc->idx;
        stamp->line = loc->line;

        return stamp;

    }







    // dtype operation function

    int applyUnaryOperatorPlusI32(Operand* operand) {
        return Err::OK;
    }

    int applyUnaryOperatorPlusI64(Operand* operand) {
        return Err::OK;
    }

    int applyUnaryOperatorPlusF32(Operand* operand) {
        return Err::OK;
    }

    int applyUnaryOperatorPlusF64(Operand* operand) {
        return Err::OK;
    }



    int applyUnaryOperatorMinusI32(Operand* operand) {
        const int32_t tmp = - *((int32_t*) &(operand->value));
        operand->value = (void*) *((void**) &tmp);
        return Err::OK;        
    }

    int applyUnaryOperatorMinusI64(Operand* operand) {
        const int64_t tmp = - *((int64_t*) &(operand->value));
        operand->value = (void*) *((void**) &tmp);
        return Err::OK;
    }

    int applyUnaryOperatorMinusF32(Operand* operand) {
        const float_t tmp = - *((float_t*) &(operand->value));
        operand->value = (void*) *((void**) &tmp);
        return Err::OK;
    }

    int applyUnaryOperatorMinusF64(Operand* operand) {
        const double_t tmp = - *((double_t*) &(operand->value));
        operand->value = (void*) *((void**) &tmp);
        return Err::OK;
    }



    int applyBinaryOperatorAdditionI32(Operand* a, Operand* b) {
        const int32_t ans = *((int32_t*) &(a->value)) + *((int32_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorAdditionI64(Operand* a, Operand* b) {
        const int64_t ans = *((int64_t*) &(a->value)) + *((int64_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorAdditionF32(Operand* a, Operand* b) {
        const float_t ans = *((float_t*) &(a->value)) + *((float_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorAdditionF64(Operand* a, Operand* b) {
        const double_t ans = *((double_t*) &(a->value)) + *((double_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }



    int applyBinaryOperatorSubtractionI32(Operand* a, Operand* b) {
        const int32_t ans = *((int32_t*) &(a->value)) - *((int32_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorSubtractionI64(Operand* a, Operand* b) {
        const int64_t ans = *((int64_t*) &(a->value)) - *((int64_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorSubtractionF32(Operand* a, Operand* b) {
        const float_t ans = *((float_t*) &(a->value)) - *((float_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorSubtractionF64(Operand* a, Operand* b) {
        const double_t ans = *((double_t*) &(a->value)) - *((double_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }


    int applyBinaryOperatorMultiplicationI32(Operand* a, Operand* b) {
        const int32_t ans = *((int32_t*) &(a->value)) * *((int32_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorMultiplicationI64(Operand* a, Operand* b) {
        const int64_t ans = *((int64_t*) &(a->value)) * *((int64_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorMultiplicationF32(Operand* a, Operand* b) {
        const float_t ans = *((float_t*) &(a->value)) * *((float_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorMultiplicationF64(Operand* a, Operand* b) {
        const double_t ans = *((double_t*) &(a->value)) * *((double_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }



    int applyBinaryOperatorDivisionI32(Operand* a, Operand* b) {
        const int32_t ans = *((int32_t*) &(a->value)) / *((int32_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorDivisionI64(Operand* a, Operand* b) {
        const int64_t ans = *((int64_t*) &(a->value)) / *((int64_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorDivisionF32(Operand* a, Operand* b) {
        const float_t ans = *((float_t*) &(a->value)) / *((float_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorDivisionF64(Operand* a, Operand* b) {
        const double_t ans = *((double_t*) &(a->value)) / *((double_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }



    int applyBinaryOperatorModuloI32(Operand* a, Operand* b) {
        const int32_t ans = *((int32_t*) &(a->value)) % *((int32_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }

    int applyBinaryOperatorModuloI64(Operand* a, Operand* b) {
        const int64_t ans = *((int64_t*) &(a->value)) % *((int64_t*) &(b->value));
        a->value = (void*) *((void**) &ans);
        return Err::OK;
    }



    int applyUnaryOperatorAddress(Operand* operand) {
        return Err::OK;
    }







    int cmpOneChar(Operator* op, uint32_t word) {
        return op->word == word & 0xFF;
    }

    int cmpTwoChars(Operator* op, char* const str) {
        
        const uint16_t wordA = op->word;
        const uint16_t wordB = *((uint16_t*) str);

        if (wordA < 256) return wordA == wordB & 0xFF;
        return wordA == wordB;

    }

    
}