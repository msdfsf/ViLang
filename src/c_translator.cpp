#pragma once

#include <stdio.h>

#include "c_translator.h"

#define MAX_FILE_SIZE 256

const char  MAIN_FILE_STR[] = "main.c";
const char  FUNC_FILE_STR[] = "functions.c";


FILE* mFile = NULL;
FILE* fFile = NULL;



// TODO : move to utils or globals
//#define POSIX 0
#define WINDOWS 1

// 0 on success
#ifdef POSIX == 1
    int newdir(char* const path) {
        mkdir(path, 0777);
    }
#elif WINDOWS == 1
    #include <direct.h>
    int newDir(char* const path) {
        if (_mkdir(path)) {
            return (errno != EEXIST);
        }
    }
#endif



void c_printOperandValue(FILE* file, Operand* op) {

    switch (op->dataTypeEnum) {

        case DT_INT : {

        }

        case DT_INT_32 : {
            fprintf(file, "%i", *((int32_t*) &op->value));
            break;
        }

        case DT_INT_64 : {
            fprintf(file, "%i", *((int64_t*) &op->value));
            break;
        }

        case DT_FLOAT_32 : {
            fprintf(file, "%.2f", *((float_t*) &op->value));
            break;
        }

        case DT_FLOAT_64 : {
            fprintf(file, "%.2f", *((double_t*) &op->value));
            break;
        }

        case DT_STRING : {
            fprintf(file, "\"%s\"", (char*) op->value);
            break;
        }

        default : {
            fprintf(file, "<unknown type>");
        }
    
    }

}

void c_printDataType(DataType* const dtype, const DataTypeEnum dtypeEnum) {

    if (dtypeEnum == DT_CUSTOM) {

        fprintf(mFile, "%.*s", dtype->nameLen, dtype->name);    
    
    } else if (dtypeEnum == DT_POINTER) {

        Pointer* const ptr = (Pointer*) dtype;
        DataType* ptrDtype = dataTypes + DT_POINTER;

        c_printDataType(ptr->pointsTo, ptr->pointsToEnum);
        fprintf(mFile, "%s", ptrDtype->name);
    
    } else {

        fprintf(mFile, "%s", dtype->name);
    
    }

}

// TODO : handle errors
void c_init(char* const dirName) {

    if (newDir(dirName)) {
        return;
    }

    if (chdir(dirName)) {
        return;
    }

    mFile = fopen(MAIN_FILE_STR, "w");
    if (!mFile) {
        return;
    }

    fFile = fopen(FUNC_FILE_STR, "w");
    if (!fFile) {
        return;
    }

    fprintf(mFile, "#include \"functions.c\"\n");
    fprintf(mFile, "int main(int argc, char* argv)");

}

void c_printScope(int level, Scope* const node) {

    fputc('{', mFile);

    for(int i = 0; i < (int) node->children.size(); i++) {
        node->children[i]->print(&translatorC, level + 1);
    }

    fputc('}', mFile);

}

void c_printVariableDefinition(int level, VariableDefinition* const node) {

    if (node->flags & IS_CONST) fprintf(mFile, "const");

    c_printDataType(node->var->dtype, node->var->dataTypeEnum);

    fprintf(mFile, " %.*s=", node->var->nameLen, node->var->name);
    
    if (node->var->expression) {
        node->var->expression->print(&translatorC, level);
    } else {
        c_printOperandValue(mFile, node->var);
    }

    fputc(';', mFile);

}

void c_printVariableAssignment(int level, VariableAssignment* const node) {
    
    fprintf(mFile, "%.*s=", node->var->nameLen, node->var->name);
    
    if (node->var->expression) {
        node->var->expression->print(&translatorC, level);
    } else {
        c_printOperandValue(mFile, node->var);
    }

    fputc(';', mFile);

}

void c_printTypeDefinition(int level, TypeDefinition* const node) {

    fprintf(mFile, "typedef struct %.*s{", node->nameLen, node->name);
    
    const int lastIdx = (int) node->vars.size() - 1;
    
    for (int i = 0; i < lastIdx; i++) {
        
        Variable* const var = node->vars[i];
        
        // TODO : cleanup
        if (var->dataTypeEnum != DT_CUSTOM) {
            DataType* const dtype = dataTypes + var->dataTypeEnum; 
            fprintf(mFile, "%.*s %.*s;",  dtype->nameLen, dtype->name, var->nameLen, var->name);
        } else {
            TypeDefinition* const dtype = (TypeDefinition*) var->dtype; 
            fprintf(mFile, "%.*s %.*s;", dtype->nameLen, dtype->name, var->nameLen, var->name);    
        }

    }

    if (lastIdx - 1 >= 0) {
        
        Variable* const var = node->vars[lastIdx];
        
        if (var->dataTypeEnum != DT_CUSTOM) {
            DataType* const dtype = dataTypes + var->dataTypeEnum; 
            fprintf(mFile, "%.*s %.*s", dtype->nameLen, dtype->name, var->nameLen, var->name);
        } else {
            TypeDefinition* const dtype = (TypeDefinition*) var->dtype; 
            fprintf(mFile, "%.*s %.*s", dtype->nameLen, dtype->name, var->nameLen, var->name);
        }
    }
    
    fprintf(mFile, "}%.*s;", node->nameLen, node->name);

}

void c_printTypeInitialization(int level, TypeInitialization* const node) {

    fputc('{', mFile);

    for (int i = 0; i < (int) node->attributes.size() - 1; i++) {
        
        Variable* const var = node->attributes[i];

        if (var->nameLen > 0) {
            fprintf(mFile, "%.*s = ", var->nameLen, var->name);
            if (var->expression) {
                var->expression->print(&translatorC, level + 1);
            } else {
                c_printOperandValue(mFile, var);
            }
        } else {
            if (var->expression) {
                var->expression->print(&translatorC, level + 1);
            } else {
                c_printOperandValue(mFile, var);
            }
        }

        fputc(',', mFile);

    }

    if ((int) node->attributes.size() > 0) {
        
        Variable* const var = node->attributes[(int) node->attributes.size() - 1];

        if (var->nameLen > 0) {
            printf("%.*s = ", var->nameLen, var->name);
            if (var->expression) {
                var->expression->print(&translatorC, level + 1);
            } else {
                c_printOperandValue(mFile, var);
            }
        } else {
            if (var->expression) {
                var->expression->print(&translatorC, level + 1);
            } else {
                c_printOperandValue(mFile, var);
            }
        }

    }

    fputc('}', mFile);

}

void c_printEnumerator(int level, Enumerator* const node) {

}

void c_printVariable(int level, Variable* const node) {

}


void c_printFunction(int level, Function* const node) {

}


void c_printBranch(int level, Branch* const node) {

}


void c_printWhileLoop(int level, WhileLoop* const node) {

}


void c_printForLoop(int level, ForLoop* const node) {

}

void c_printExpression(int level, Expression* const node) {

}

void c_printWrapperExpression(int level, WrapperExpression* const node) {
    
    node->operand->print(&translatorC, level);

}

void c_printExpressionWrapper(int level, ExpressionWrapper* const node) {

    node->operand->print(&translatorC, level);
    fputc(';', mFile);

}

void c_printConstExpression(int level, ConstExpression* const node) {

}

void c_printOperatorExpression(int level, OperatorExpression* const node) {

}

void c_printUnaryExpression(int level, UnaryExpression* const node) {
    
    fputc('(', mFile);
    
    node->oper->print(&translatorC, level);

    if (node->operand->unrollExpression && node->operand->expression) node->operand->expression->print(thisTranslator);
    else node->operand->print(&translatorC, level);

    fputc(')', mFile);

}

void c_printBinaryExpression(int level, BinaryExpression* const node) {

    fputc('(', mFile);
    
    if (node->operandA->unrollExpression && node->operandA->expression) node->operandA->expression->print(thisTranslator);
    else node->operandA->print(&translatorC, level);

    node->oper->print(&translatorC, 1);
    
    if (node->operandB->unrollExpression && node->operandB->expression) node->operandB->expression->print(thisTranslator);
    else node->operandB->print(&translatorC, level);

    fputc(')', mFile);

}

void c_printTernaryExpression(int level, TernaryExpression* const node) {

}

void c_printFunctionCall(int level, FunctionCall* const node) {

}

void c_printOperand(int level, Operand* const node) {

}

void c_printOperator(int spaces, Operator* const node) {

}

void c_printUnaryOperator(int level, UnaryOperator* const node) {

}

void c_printBinaryOperator(int level, BinaryOperator* const node) {

}

void c_printTernaryOperator(int level, TernaryOperator* const node) {

}



Translator translatorC {
    &c_init,
    &c_printScope,
    &c_printVariableDefinition,
    &c_printVariableAssignment,
    &c_printTypeDefinition,
    &c_printTypeInitialization,
    &c_printEnumerator,
    &c_printVariable,
    &c_printFunction,
    &c_printBranch,
    &c_printWhileLoop,
    &c_printForLoop,
    &c_printExpression,
    &c_printWrapperExpression,
    &c_printExpressionWrapper,
    &c_printConstExpression,
    &c_printOperatorExpression,
    &c_printUnaryExpression,
    &c_printBinaryExpression,
    &c_printTernaryExpression,
    &c_printFunctionCall,
    &c_printOperand,
    &c_printOperator,
    &c_printUnaryOperator,
    &c_printBinaryOperator,
    &c_printTernaryOperator
};