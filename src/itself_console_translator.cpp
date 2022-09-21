#pragma once

#include "itself_console_translator.h"



#define CSP_MAX_TAB_LEVEL 4
#define CSP_GEN_TAB(str, n) for (int i = 0; i < (n); i++) str[i] = '\t'; str[(n)] = '\0';



// maybe const?
Translator* const thisTranslator = &translatorItselfConsole;



void init (char* const dirName) {

}

void printDataType(DataTypeEnum dtype) {

    switch (dtype) {

        case DT_INT :
            printf(KWS_INT);
            break;

        case DT_INT_32 :
            printf(KWS_INT_32);
            break;

        case DT_INT_64 :
            printf(KWS_INT_64);
            break;

        case DT_FLOAT_32 :
            printf(KWS_FLOAT_32);
            break;

        case DT_FLOAT_64 :
            printf(KWS_FLOAT_64);
            break;

        case DT_STRING :
            printf(KWS_STRING);
            break;

        case DT_POINTER :
            printf("<ptr>");
            break;

        default : {
            printf("<unknown type>");
        }
    
    }

}

void printOperandValue(Operand* op) {

    switch (op->dataTypeEnum) {

        case DT_INT : {

        }

        case DT_INT_32 : {
            printf("%i", *((int32_t*) &op->value));
            break;
        }

        case DT_INT_64 : {
            printf("%i", *((int64_t*) &op->value));
            break;
        }

        case DT_FLOAT_32 : {
            printf("%.2f", *((float_t*) &op->value));
            break;
        }

        case DT_FLOAT_64 : {
            printf("%.2f", *((double_t*) &op->value));
            break;
        }

        case DT_STRING : {
            printf("\"%s\"", (char*) op->value);
            break;
        }

        default : {
            printf("<unknown type>");
        }
    
    }

}

// TODO : maybe abstract??? will it benefit??
void printDataType(DataType* const dtype, const DataTypeEnum dtypeEnum) {

    if (dtypeEnum == DT_CUSTOM) {

        printf("%.*s", dtype->name, dtype->nameLen);    
    
    } else if (dtypeEnum == DT_POINTER) {

        Pointer* const ptr = (Pointer*) dtype;
        DataType* ptrDtype = dataTypes + DT_POINTER;

        printDataType(ptr->pointsTo, ptr->pointsToEnum);
        printf("%s", ptrDtype->name);
    
    } else {

        printf("%s", dtype->name);
    
    }

}



void printScope(int level, Scope* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);
    
    printf("%s{\n", tab);
    for (int i = 0; i < (int) node->children.size(); i++) {
        node->children[i]->print(thisTranslator, level + 1);
    }
    printf("%s}\n", tab);

}

void printVariableDefinition(int level, VariableDefinition* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%s", tab);
    if (node->flags & IS_CONST) {
        printf("const ");
        printDataType(node->var->dtype, node->var->dataTypeEnum);
    } else {
        printDataType(node->var->dtype, node->var->dataTypeEnum);
    }

    printf(" %.*s = ", node->var->nameLen, node->var->name);
    
    // again, somehow have to get rid of this check
    if (node->var->expression) {
        
        Expression* ex = node->var->expression;
        ex->print(thisTranslator, level);

    } else {
        //var->print(level);
        printOperandValue(node->var);
    }

    printf(";\n");

}

void printVariableAssignment(int level, VariableAssignment* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%s", tab);
    node->var->print(thisTranslator, level);
    printf(" = ");
    //printf("%s%.*s = ", tab, var->nameLen, var->name);
    // again, somehow have to get rid of this check
    if (node->var->expression) {
        
        Expression* ex = node->var->expression;
        ex->print(thisTranslator);

    } else {
        printOperandValue(node->var);
    }

    printf(";\n");

}

void printTypeDefinition(int level, TypeDefinition* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%sdef %.*s \n%s{\n", tab, node->nameLen, node->name, tab);
    
    const int lastIdx = (int) node->vars.size() - 1;
    
    for (int i = 0; i < lastIdx; i++) {
        
        Variable* const var = node->vars[i];
        
        // TODO : cleanup
        if (var->dataTypeEnum != DT_CUSTOM) {
            DataType* const dtype = dataTypes + var->dataTypeEnum; 
            printf("%s\t%.*s %.*s = %i,\n", tab, dtype->nameLen, dtype->name, var->nameLen, var->name, var->value);
        } else {
            TypeDefinition* const dtype = (TypeDefinition*) var->dtype; 
            printf("%s\t%.*s %.*s = %i,\n", tab, dtype->nameLen, dtype->name, var->nameLen, var->name, var->value);    
        }

    }

    if (lastIdx - 1 >= 0) {
        
        Variable* const var = node->vars[lastIdx];
        
        if (var->dataTypeEnum != DT_CUSTOM) {
            DataType* const dtype = dataTypes + var->dataTypeEnum; 
            printf("%s\t%.*s %.*s = %i\n", tab, dtype->nameLen, dtype->name, var->nameLen, var->name, var->value);
        } else {
            TypeDefinition* const dtype = (TypeDefinition*) var->dtype; 
            printf("%s\t%.*s %.*s = %i\n", tab, dtype->nameLen, dtype->name, var->nameLen, var->name, var->value);
        }
    }
    
    printf("%s}\n", tab);

}

void printTypeInitialization(int level, TypeInitialization* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("{\n");

    for (int i = 0; i < (int) node->attributes.size() - 1; i++) {
        
        Variable* const var = node->attributes[i];
        
        printf("%s\t", tab);

        if (var->nameLen > 0) {
            printf("%.*s = ", var->nameLen, var->name);
            if (var->expression) {
                var->expression->print(thisTranslator, level + 1);
            } else {
                //printf("%s", tab);
                printOperandValue(var);
            }
        } else {
            if (var->expression) {
                var->expression->print(thisTranslator, level + 1);
            } else {
                //printf("%s", tab);
                printOperandValue(var);
            }
        }

        printf(",\n");

    }

    if ((int) node->attributes.size() > 0) {
        
        Variable* const var = node->attributes[(int) node->attributes.size() - 1];
        
        printf("%s\t", tab);

        if (var->nameLen > 0) {
            printf("%.*s = ", var->nameLen, var->name);
            if (var->expression) {
                var->expression->print(thisTranslator, level + 1);
            } else {
                //printf("%s", tab);
                printOperandValue(var);
            }
        } else {
            if (var->expression) {
                var->expression->print(thisTranslator, level + 1);
            } else {
                //printf("%s", tab);
                printOperandValue(var);
            }
        }

    }

    printf("\n%s}", tab);

}

void printEnumerator(int level, Enumerator* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    DataType* const dt = dataTypes + node->dtype;

    printf("%senum %.*s %.*s \n%s{\n", tab, node->nameLen, node->name, dt->nameLen, dt->name, tab);
    
    const int lastIdx = (int) node->vars.size() - 1;
    
    for (int i = 0; i < lastIdx; i++) {
        Variable* const var = node->vars[i];
        printf("%s\t%.*s = %d,\n", tab, var->nameLen, var->name, var->value);
    }

    if (lastIdx - 1 >= 0) {
        Variable* const var = node->vars[lastIdx];
        printf("%s\t%.*s = %i\n", tab, var->nameLen, var->name, var->value);
    }
    
    printf("%s}\n", tab);

}

void printVariable(int level, Variable* const node) {
    
    if (node->parentStruct) {
        node->parentStruct->print(thisTranslator, level);
        putchar('.');
    }
    
    // maybe separate type?
    if (node->nameLen > 0) {
        printf("%.*s", node->nameLen, node->name);
    } else {
        if (node->expression) node->expression->print(thisTranslator);
        else printOperandValue(node);
    }

}


void printFunction(int level, Function* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%s%.*s(", tab, node->nameLen, node->name);

    // in args
    for (int i = 0; i < ((int) node->inArgs.size()) - 1; i++) {
        Variable* const var = node->inArgs[i];
        printf("%s %.*s, ", (dataTypes + var->dataTypeEnum)->name, var->nameLen, var->name);
    }
    if ((int) node->inArgs.size() - 1 >= 0) {
        Variable* const var = node->inArgs[node->inArgs.size() - 1];
        printf("%s %.*s) => (", (dataTypes + var->dataTypeEnum)->name, var->nameLen, var->name);
    } else {
        printf(") => (");
    
    }

    // out args
    for (int i = 0; i < ((int) node->outArgs.size()) - 1; i++) {
        const DataType dtype = dataTypes[node->outArgs[i]];
        printf("%s, ", dtype.name);
    }
    if ((int) node->outArgs.size() - 1 >= 0) {
        const DataType dtype = dataTypes[node->outArgs[node->outArgs.size() - 1]];
        printf("%s)\n", dtype.name);
    } else {
        printf(")\n");
    }

    // TODO : empty body crash
    node->bodyScope->print(thisTranslator, level);

}


void printBranch(int level, Branch* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    // basic if branche
    printf("%s%s ", tab, KWS_IF);
    node->expressions[0]->print(thisTranslator, level);
    putchar('\n');
    node->scopes[0]->print(thisTranslator, level);
    
    // if elses
    int i = 1;
    for (; i < node->expressions.size(); i++) {
        printf("%s%s %s ", tab, KWS_ELSE, KWS_IF);
        node->expressions[i]->print(thisTranslator, level);
        putchar('\n');
        node->scopes[i]->print(thisTranslator, level);
    }

    // final else if present
    if (i < node->scopes.size()) {
        printf("%s%s\n", tab, KWS_ELSE);
        node->scopes[(int) node->scopes.size() - 1]->print(thisTranslator, level);
    }

}


void printWhileLoop(int level, WhileLoop* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%s%s ", tab, KWS_WHILE);
    node->expression->print(thisTranslator, level);
    putchar('\n');
    node->bodyScope->print(thisTranslator, level);

}


void printForLoop(int level, ForLoop* const node) {

}

void printExpression(int level, Expression* const node) {

}

void printExpressionWrapper(int level, ExpressionWrapper* const node) {

    level = (level > CSP_MAX_TAB_LEVEL) ? CSP_MAX_TAB_LEVEL : level;
    char tab[CSP_MAX_TAB_LEVEL + 1];
    CSP_GEN_TAB(tab, level);

    printf("%s", tab);
    node->operand->print(thisTranslator, level);
    printf(";\n");

}

void printWrapperExpression(int level, WrapperExpression* const node) {

    node->operand->print(thisTranslator, level);

}

void printConstExpression(int level, ConstExpression* const node) {

}

void printOperatorExpression(int level, OperatorExpression* const node) {

}

void printUnaryExpression(int level, UnaryExpression* const node) {

    printf("(");
    
    node->oper->print(thisTranslator);

    if (node->operand->unrollExpression && node->operand->expression) node->operand->expression->print(thisTranslator);
    else node->operand->print(thisTranslator, level);
        // printOperandValue(operand);

    printf(")");

}

void printBinaryExpression(int level, BinaryExpression* const node) {

    printf("(");
    
    if (node->operandA->unrollExpression && node->operandA->expression) node->operandA->expression->print(thisTranslator);
    else node->operandA->print(thisTranslator, level); //printOperandValue(operandA);

    node->oper->print(thisTranslator, 1);
    
    if (node->operandB->unrollExpression && node->operandB->expression) node->operandB->expression->print(thisTranslator);
    else node->operandB->print(thisTranslator, level); //printOperandValue(operandB);

    printf(")");

}

void printTernaryExpression(int level, TernaryExpression* const node) {

}

void printFunctionCall(int level, FunctionCall* const node) {

    printf("%.*s(", node->nameLen, node->name);
    for (int i = 0; i < ((int) node->inArgs.size()) - 1; i++) {
        node->inArgs[i]->print(thisTranslator, level);
        putchar(',');
        putchar(' ');
        //Variable* const var = inArgs[i];
        //printf("%s %.*s, ", (dataTypes + var->dataTypeEnum)->word, var->nameLen, var->name);
    }
    if ((int) node->inArgs.size() - 1 >= 0) {
        node->inArgs[node->inArgs.size() - 1]->print(thisTranslator, 0);
        //Variable* const var = inArgs[inArgs.size() - 1];
        //printf("%s %.*s", (dataTypes + var->dataTypeEnum)->word, var->nameLen, var->name);
    }
    putchar(')');

}

void printOperand(int level, Operand* const node) {

    // maybe separate type?
    if (node->expression) {
        node->expression->print(thisTranslator, level);
    } else {
        printOperandValue(node);
    }

}

void printOperator(int spaces, Operator* const node) {

    const char A = node->word;
    const char B = node->word >> 8;
    const char C = node->word >> 16;
    const char D = node->word >> 24;

    if (spaces) printf(" %c%c%c%c ", A, B, C, D);
    else printf("%c%c%c%c", A, B, C, D);

}

void printUnaryOperator(int level, UnaryOperator* const node) {

}

void printBinaryOperator(int level, BinaryOperator* const node) {

}

void printTernaryOperator(int level, TernaryOperator* const node) {

}



Translator translatorItselfConsole {
    &init,
    &printScope,
    &printVariableDefinition,
    &printVariableAssignment,
    &printTypeDefinition,
    &printTypeInitialization,
    &printEnumerator,
    &printVariable,
    &printFunction,
    &printBranch,
    &printWhileLoop,
    &printForLoop,
    &printExpression,
    &printWrapperExpression,
    &printExpressionWrapper,
    &printConstExpression,
    &printOperatorExpression,
    &printUnaryExpression,
    &printBinaryExpression,
    &printTernaryExpression,
    &printFunctionCall,
    &printOperand,
    &printOperator,
    &printUnaryOperator,
    &printBinaryOperator,
    &printTernaryOperator
};