#pragma once

#include "c_translator.h"



void asm_init(char* const dirName) {

}

void asm_printScope(int level, Scope* const node) {

}

void asm_printVariableDefinition(int level, VariableDefinition* const node) {

}

void asm_printVariableAssignment(int level, VariableAssignment* const node) {

}

void asm_printTypeDefinition(int level, TypeDefinition* const node) {

}

void asm_printTypeInitialization(int level, TypeInitialization* const node) {

}

void asm_printEnumerator(int level, Enumerator* const node) {

}

void asm_printVariable(int level, Variable* const node) {

}


void asm_printFunction(int level, Function* const node) {

}


void asm_printBranch(int level, Branch* const node) {

}


void asm_printWhileLoop(int level, WhileLoop* const node) {

}


void asm_printForLoop(int level, ForLoop* const node) {

}

void asm_printExpression(int level, Expression* const node) {

}

void asm_printWrapperExpression(int level, WrapperExpression* const node) {

}

void asm_printExpressionWrapper(int level, ExpressionWrapper* const node) {

}

void asm_printConstExpression(int level, ConstExpression* const node) {

}

void asm_printOperatorExpression(int level, OperatorExpression* const node) {

}

void asm_printUnaryExpression(int level, UnaryExpression* const node) {

}

void asm_printBinaryExpression(int level, BinaryExpression* const node) {

}

void asm_printTernaryExpression(int level, TernaryExpression* const node) {

}

void asm_printFunctionCall(int level, FunctionCall* const node) {

}

void asm_printOperand(int level, Operand* const node) {

}

void asm_printOperator(int spaces, Operator* const node) {

}

void asm_printUnaryOperator(int level, UnaryOperator* const node) {

}

void asm_printBinaryOperator(int level, BinaryOperator* const node) {

}

void asm_printTernaryOperator(int level, TernaryOperator* const node) {

}



Translator translatorASM {
    &asm_init,
    &asm_printScope,
    &asm_printVariableDefinition,
    &asm_printVariableAssignment,
    &asm_printTypeDefinition,
    &asm_printTypeInitialization,
    &asm_printEnumerator,
    &asm_printVariable,
    &asm_printFunction,
    &asm_printBranch,
    &asm_printWhileLoop,
    &asm_printForLoop,
    &asm_printExpression,
    &asm_printWrapperExpression,
    &asm_printExpressionWrapper,
    &asm_printConstExpression,
    &asm_printOperatorExpression,
    &asm_printUnaryExpression,
    &asm_printBinaryExpression,
    &asm_printTernaryExpression,
    &asm_printFunctionCall,
    &asm_printOperand,
    &asm_printOperator,
    &asm_printUnaryOperator,
    &asm_printBinaryOperator,
    &asm_printTernaryOperator
};