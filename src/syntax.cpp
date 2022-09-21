#pragma once

#include "globals.h"

#include "syntax.h"
#include "error.h"
#include "logger.h"




Scope* SyntaxNode::root = NULL;



// helping functions (move to utils?)
// 
Location* getLocationStamp(Location* loc) {

    Location* stamp = (Location*) malloc(sizeof(Location));
    if (!stamp) return NULL;

    stamp->file = loc->file;
    stamp->idx = loc->idx;
    stamp->line = loc->line;

    return stamp;

}



// Constructors
//

Operand::Operand(Scope* scope) {

    this->scope = scope;
    this->expression = NULL;
    this->unrollExpression = 1;
    this->value = NULL;
    this->dataTypeEnum = DT_UNDEFINED;
    this->loc = NULL;
    this->value = NULL;

}

VariableDefinition::VariableDefinition(Location* loc) {
    
    this->loc = getLocationStamp(loc);
    if (!this->loc) exit(1); // LOOK AT : maybe manage better

}

VariableAssignment::VariableAssignment(Location* loc) {
    
    this->loc = getLocationStamp(loc);
    if (!this->loc) exit(1); // LOOK AT : maybe manage better

}

Variable::Variable() {

    unrollExpression = 1;
    scope = root;
    name = NULL;
    nameLen = 0;
    expression = NULL;
    dataTypeEnum = DT_UNDEFINED;
    value = NULL;

}

Variable::Variable(Location* loc) : Variable() {
    
    this->loc = getLocationStamp(loc);
    if (!this->loc) exit(1); // LOOK AT : maybe manage better
    
}

Variable::Variable(Scope* const sc, DataTypeEnum dtype) {
    
    unrollExpression = 1;
    scope = sc;
    name = NULL;
    nameLen = 0;
    expression = NULL;
    dataTypeEnum = dtype;
    value = NULL;
    parentStruct = NULL;

}

Variable::Variable(Scope* const sc, DataTypeEnum dtype, Location* loc) : Variable(sc, dtype) {
    
    this->loc = getLocationStamp(loc);
    if (!this->loc) exit(1); // LOOK AT : maybe manage better
    
}

Function::Function(Scope* sc, char* name, int nameLen, std::vector<Variable*> inArgs, std::vector<DataTypeEnum> outArgs, int internalIdx) {
    this->scope = sc;
    this->name = name;
    this->nameLen = nameLen;
    this->inArgs = inArgs;
    this->outArgs = outArgs;
    this->internalIdx = internalIdx;
    this->bodyScope = NULL;
}



// print stuff
//

void Scope::print(Translator* const translator, int level) {
    translator->printScope(level, this);
};

void Enumerator::print(Translator* const translator, int level) {
    translator->printEnumerator(level, this);
};

void TypeDefinition::print(Translator* const translator, int level) {
    translator->printTypeDefinition(level, this);
};

void VariableDefinition::print(Translator* const translator, int level) {
    translator->printVariableDefinition(level, this);
}

void VariableAssignment::print(Translator* const translator, int level) {
    translator->printVaraiableAssignment(level, this);
}

void Variable::print(Translator* const translator, int level) {
    translator->printVariable(level, this);
};

void Function::print(Translator* const translator, int level) {
    translator->printFunction(level, this);
};

void Branch::print(Translator* const translator, int level) {
    translator->printBranch(level, this);
};

void WhileLoop::print(Translator* const translator, int level) {
    translator->printWhileLoop(level, this);
};

void ExpressionWrapper::print(Translator* const translator, int level) {
    translator->printExpressionWrapper(level, this);
}

void Operand::print(Translator* const translator, const int level) {
    translator->printOperand(level, this);
}

void Operator::print(Translator* const translator, const int spaces) {
    translator->printOperator(spaces, this);
}

void FunctionCall::print(Translator* const translator, int level) {
    translator->printFunctionCall(level, this);
}

void TypeInitialization::print(Translator* const translator, int level) {
    translator->printTypeInitialization(level, this);
}

void WrapperExpression::print(Translator* const translator, int level) {
    translator->printWrapperExpression(level, this);
}

void UnaryExpression::print(Translator* const translator, int level) {
    translator->printUnaryExpression(level, this);
}

void BinaryExpression::print(Translator* const translator, int level) {
    translator->printBinaryExpression(level, this);
}



// expression evaluating stuff
//

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

int evaluateExpression(Expression* exp) {

    Operand* ans;
    Operand* varPart;
    Operand* constPart;

    exp->evaluate(&ans, &constPart);

    return 0;

}

// basicly the root for an expression
int WrapperExpression::evaluate(Operand** ans, Operand** constPart) {

    if (!operand->unrollExpression) {
        return operand->dataTypeEnum;
    }

    const int dtype = (operand->expression) ?  operand->expression->evaluate(&operand, constPart) : operand->dataTypeEnum;

    if (operand->dataTypeEnum == DT_UNDEFINED) {
        return dtype;
    }

    (*ans)->value = operand->value;
    (*ans)->unrollExpression = 0;
    (*ans)->expression = NULL;
    (*ans)->dataTypeEnum = (DataTypeEnum) dtype;
    // TODO : free!!!!

    return dtype;

}
    
int FunctionCall::evaluate(Operand** ans, Operand** constPart) {

    return outArg->dataTypeEnum;

}

int TypeInitialization::evaluate(Operand** ans, Operand** constPart) {

    return DT_CUSTOM;

}

int UnaryExpression::evaluate(Operand** ans, Operand** constPart) {

    if (!operand->unrollExpression) {
        return operand->dataTypeEnum;
    }

    if (operand->expression) {
        const int dtype = operand->expression->evaluate(&operand, constPart);
        if (operand->dataTypeEnum == DT_UNDEFINED) {
            return dtype;
        }
    }

    const int dtype = applyOperator(oper, operand);

    (*ans)->value = operand->value;
    (*ans)->unrollExpression = 0;
    (*ans)->expression = NULL;
    (*ans)->dataTypeEnum = (DataTypeEnum) dtype;
    // TODO : free!!!!

    return dtype;

}

int BinaryExpression::evaluate(Operand** ans, Operand** constPart) {

    /*

    const int dtypeA = (operandA->expression) ? operandA->expression->evaluate(&operandA, constPart) : operandA->dataTypeEnum;
    const int dtypeB = (operandB->expression) ? operandB->expression->evaluate(&operandB, constPart) : operandB->dataTypeEnum;

    const int dtypeARank = (dataTypes + dtypeA)->rank;
    const int dtypeBRank = (dataTypes + dtypeB)->rank;

    if (oper == operators + OP_ADDITION || oper == operators + OP_SUBTRACTION) {

        const int isAVar = operandA->dataTypeEnum == DT_UNDEFINED;
        const int isBVar = operandB->dataTypeEnum == DT_UNDEFINED;
        
        if (isAVar && isBVar) {
            
            return DT_UNDEFINED;
        
        } else if (isAVar) {
            
            *ans = operandA;
            
            if (!(*constPart)) {
                *constPart = operandB;
                return DT_UNDEFINED;
            }

            if (dtypeARank > dtypeBRank && !cast(operandB, (DataTypeEnum) dtypeA) || !cast(*constPart, (DataTypeEnum) dtypeB)) {
                return applyOperator(oper, *constPart, operandB);
            } else {
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION));
                return Err::INVALID_TYPE_CONVERSION;
            }

        } else if (isBVar) {

            *ans = operandB;

            if (!(*constPart)) {
                *constPart = operandA;
                return DT_UNDEFINED;
            }

            if (dtypeARank > dtypeBRank && !cast(operandA, (DataTypeEnum) dtypeB) || !cast(*constPart, (DataTypeEnum) dtypeA)) {
                return applyOperator(oper, *constPart, operandA);
            } else {
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION));
                return Err::INVALID_TYPE_CONVERSION;
            }

        } else {

            if (dtypeARank > dtypeBRank && !cast(operandB, (DataTypeEnum) dtypeB) || !cast(operandA, (DataTypeEnum) dtypeA)) {
                return applyOperator(oper, operandA, operandB);
            } else {
                Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION));
                return Err::INVALID_TYPE_CONVERSION;
            }

        }
        
    }
    */

    const int dtypeA = (operandA->expression) ? operandA->expression->evaluate(&operandA, constPart) : operandA->dataTypeEnum;
    const int dtypeB = (operandB->expression) ? operandB->expression->evaluate(&operandB, constPart) : operandB->dataTypeEnum;

    const int dtypeARank = (dataTypes + dtypeA)->rank;
    const int dtypeBRank = (dataTypes + dtypeB)->rank;

    if (operandA->dataTypeEnum == DT_UNDEFINED || operandB->dataTypeEnum == DT_UNDEFINED) {
        // LOOK AT : cast here? to know if it fails or not? ot make cast 'bulletproof'?
        return (dtypeARank > dtypeBRank) ? dtypeARank : dtypeBRank;
    }

    if (dtypeARank > dtypeBRank && !cast(operandB, (DataTypeEnum) dtypeA) || !cast(operandA, (DataTypeEnum) dtypeB)) {

        const int dtype = applyOperator(oper, operandA, operandB);
        
        (*ans)->value = operandA->value;
        (*ans)->unrollExpression = 0;
        (*ans)->expression = NULL;
        (*ans)->dataTypeEnum = (DataTypeEnum) dtype;

        return dtype;
    
    }

    Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION));
    return Err::INVALID_TYPE_CONVERSION;

    /*

    const int dtypeA = (operandA->expression) ? operandA->expression->evaluate(operandA, varPart, constPart) : operandA->dataTypeEnum;
    if (dtypeA == DT_UNDEFINED && *varPart) {
        operandA = *varPart;
        operandB = *constPart;
    }
    
    const int dtypeB = (operandB->expression) ? operandB->expression->evaluate(operandB, varPart, constPart) : operandB->dataTypeEnum;
    if (dtypeB == DT_UNDEFINED && *varPart) {
        operandA = *constPart;
        operandB = *varPart;
    }

    if (dtypeA == DT_CUSTOM || dtypeB == DT_CUSTOM) {
        // TODO : change when custom data type support will be added, for now error
        //Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION), loc, str);
        return Err::INVALID_TYPE_CONVERSION;
    }

    // TODO : do a better job here!!!
    if (oper == operators + OP_ADDITION || oper == operators + OP_SUBTRACTION) {

        Operand* constOperand;
        Operand* varOperand;
        DataTypeEnum dtype = DT_UNDEFINED;

        if (operandA->dataTypeEnum == DT_UNDEFINED && operandB->unrollExpression != DT_UNDEFINED) {

            constOperand = operandB;
            varOperand = operandA;

        } else if (operandA->unrollExpression != DT_UNDEFINED && operandB->unrollExpression == DT_UNDEFINED) {
            
            constOperand = operandA;
            varOperand = operandB;
            
        } else {

            goto constVarSortEnd;

        }

        if (*constPart) {

            if (((dataTypes + constOperand->dataTypeEnum)->rank > (dataTypes + (*constPart)->dataTypeEnum)->rank && !cast(*constPart, constOperand->dataTypeEnum)) || !cast(constOperand, (*constPart)->dataTypeEnum)) {
                
                dtype = (DataTypeEnum) applyOperator(oper, *constPart, constOperand);

                //ans->value = (*constPart)->value;
                //ans->unrollExpression = 0;
                //ans->expression = NULL;
                //ans->dataTypeEnum = dtype;
            
            }

        } else {

            *constPart = constOperand;
        
        }

        if (*varPart) {

            operandA = *varPart;
            operandB = varOperand;

        } else {

            *varPart = varOperand;
        }

        return dtype;
    
    } else {

        Operand* operandA = (varPart) ? *varPart : this->operandA;
        Operand* operandB = (constPart) ? *constPart : this->operandB;

        const int dtypeARank = (dataTypes + dtypeA)->rank;
        const int dtypeBRank = (dataTypes + dtypeB)->rank;

        if (operandA->dataTypeEnum == DT_UNDEFINED || operandB->dataTypeEnum == DT_UNDEFINED) {
            // LOOK AT : cast here? to know if it fails or not? ot make cast 'bulletproof'?
            return (dtypeARank > dtypeBRank) ? dtypeARank : dtypeBRank;
        }

        if (dtypeARank > dtypeBRank && !cast(operandB, (DataTypeEnum) dtypeA)) {

            const int dtype = applyOperator(oper, operandA, operandB);

            ans->value = operandA->value;
            ans->unrollExpression = 0;
            ans->expression = NULL;
            ans->dataTypeEnum = (DataTypeEnum) dtype;

            return dtype;
        
        } else if (!cast(operandA, (DataTypeEnum) dtypeB)) {
            
            const int dtype = applyOperator(oper, operandA, operandB);
            
            ans->value = operandA->value;
            ans->unrollExpression = 0;
            ans->expression = NULL;
            ans->dataTypeEnum = (DataTypeEnum) dtype;

            return dtype;
            
            return dtypeB;
        
        }

    }

    constVarSortEnd:

    const int dtypeARank = (dataTypes + dtypeA)->rank;
    const int dtypeBRank = (dataTypes + dtypeB)->rank;

    if (operandA->dataTypeEnum == DT_UNDEFINED || operandB->dataTypeEnum == DT_UNDEFINED) {
        // LOOK AT : cast here? to know if it fails or not? ot make cast 'bulletproof'?
        return (dtypeARank > dtypeBRank) ? dtypeARank : dtypeBRank;
    }

    if (dtypeARank > dtypeBRank && !cast(operandB, (DataTypeEnum) dtypeA)) {

        const int dtype = applyOperator(oper, operandA, operandB);

        ans->value = operandA->value;
        ans->unrollExpression = 0;
        ans->expression = NULL;
        ans->dataTypeEnum = (DataTypeEnum) dtype;

        return dtype;
    
    } else if (!cast(operandA, (DataTypeEnum) dtypeB)) {
        
        const int dtype = applyOperator(oper, operandA, operandB);
        
        ans->value = operandA->value;
        ans->unrollExpression = 0;
        ans->expression = NULL;
        ans->dataTypeEnum = (DataTypeEnum) dtype;

        return dtype;
        
        return dtypeB;
    
    }

    Logger::log(Logger::ERROR, ERR_STR(Err::INVALID_TYPE_CONVERSION));
    return Err::INVALID_TYPE_CONVERSION;
    */

}



// on change update size!!!
DataType dataTypes[9] = {
        
    // VOID
    {
        (char*) KWS_VOID,
        sizeof(KWS_VOID) - 1,
        0,
        0,
        // TODO : do it in a proper way!!!!
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },

    // INT
    {
        (char*) KWS_INT,
        sizeof(KWS_INT) - 1,
        4,
        1,
        &applyUnaryOperatorPlusI32,
        &applyUnaryOperatorMinusI32,
        &applyBinaryOperatorAdditionI32,
        &applyBinaryOperatorSubtractionI32,
        &applyBinaryOperatorMultiplicationI32,
        &applyBinaryOperatorDivisionI32,
        &applyBinaryOperatorModuloI32,
        &applyUnaryOperatorAddress
    },

    // INT_32
    {
        (char*) KWS_INT_32,
        sizeof(KWS_INT_32) - 1,
        4,
        1,
        &applyUnaryOperatorPlusI32,
        &applyUnaryOperatorMinusI32,
        &applyBinaryOperatorAdditionI32,
        &applyBinaryOperatorSubtractionI32,
        &applyBinaryOperatorMultiplicationI32,
        &applyBinaryOperatorDivisionI32,
        &applyBinaryOperatorModuloI32,
        &applyUnaryOperatorAddress
    },

    // INT_64
    {
        (char*) KWS_INT_64,
        sizeof(KWS_INT_64) - 1,
        8,
        2,
        &applyUnaryOperatorPlusI64,
        &applyUnaryOperatorMinusI64,
        &applyBinaryOperatorAdditionI64,
        &applyBinaryOperatorSubtractionI64,
        &applyBinaryOperatorMultiplicationI64,
        &applyBinaryOperatorDivisionI64,
        &applyBinaryOperatorModuloI64,
        &applyUnaryOperatorAddress
    },

    // FLOAT_32
    {
        (char*) KWS_FLOAT_32,
        sizeof(KWS_FLOAT_32) - 1,
        4,
        3,
        &applyUnaryOperatorPlusF32,
        &applyUnaryOperatorMinusF32,
        &applyBinaryOperatorAdditionF32,
        &applyBinaryOperatorSubtractionF32,
        &applyBinaryOperatorMultiplicationF32,
        &applyBinaryOperatorDivisionF32,
        NULL,
        &applyUnaryOperatorAddress
    },

    // FLOAT_64
    {
        (char*) KWS_FLOAT_64,
        sizeof(KWS_FLOAT_64) - 1,
        8,
        4,
        &applyUnaryOperatorPlusF64,
        &applyUnaryOperatorMinusF64,
        &applyBinaryOperatorAdditionF64,
        &applyBinaryOperatorSubtractionF64,
        &applyBinaryOperatorMultiplicationF64,
        &applyBinaryOperatorDivisionF64,
        NULL,
        &applyUnaryOperatorAddress
    },

    // STRING
    {
        (char*) KWS_STRING,
        sizeof(KWS_STRING) - 1,
        8 * 2,
        5,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &applyUnaryOperatorAddress
    },

    // POINTER
    {
        (char*) KWS_POINTER,
        sizeof(KWS_POINTER) - 1,
        8 * 8,
        5,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &applyUnaryOperatorAddress
    },

    // MULTIPLE_TYPES
    {
        "...",
        3,
        0,
        0,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &applyUnaryOperatorAddress
    }

};