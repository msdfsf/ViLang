#pragma once

#include <vector>

#include "globals.h"


struct Translator;

enum KeyWordType : int;
struct KeyWord;

struct SyntaxNode;
struct Scope;
struct VariableDefinition;
struct VariableAssignment;
struct TypeDefinition;
struct TypeInitialization;
struct Enumerator;
struct Variable;
struct Function;
struct Branch;
struct WhileLoop;
struct ForLoop;
struct WrapperExpression;
struct ExpressionWrapper;
struct Expression;
struct ConstExpression;
struct OperatorExpression;
struct UnaryExpression;
struct BinaryExpression;
struct TernaryExpression;
struct Operator;
struct Operand;
struct FunctionCall;
struct UnaryOperator;
struct BinaryOperator;
struct TernaryOperator;

enum DataTypeEnum : int;
struct DataType;
struct TypeDefinition;
struct Pointer;



// for now here because of cross reference stuff 
struct Translator {

    void (*init) (char* const dirName);
    void (*printScope) (int level, Scope* const node);
    void (*printVariableDefinition) (int level, VariableDefinition* const node);
    void (*printVaraiableAssignment) (int level, VariableAssignment* const node);
    void (*printTypeDefinition) (int level, TypeDefinition* const node);
    void (*printTypeInitialization) (int level, TypeInitialization* const node);
    void (*printEnumerator) (int level, Enumerator* const node);
    void (*printVariable) (int level, Variable* const node);
    void (*printFunction) (int level, Function* const node);
    void (*printBranch) (int level, Branch* const node);
    void (*printWhileLoop) (int level, WhileLoop* const node);
    void (*printForLoop) (int level, ForLoop* const node);
    void (*printExpression) (int level, Expression* const node);
    void (*printWrapperExpression) (int level, WrapperExpression* const node);
    void (*printExpressionWrapper) (int level, ExpressionWrapper* const node);
    void (*printConstExpression) (int level, ConstExpression* const node);
    void (*printOperatorExpression) (int level, OperatorExpression* const node);
    void (*printUnaryExpression) (int level, UnaryExpression* const node);
    void (*printBinaryExpression) (int level, BinaryExpression* const node);
    void (*printTernaryExpression) (int level, TernaryExpression* const node);
    void (*printFunctionCall) (int level, FunctionCall* const node);
    void (*printOperand) (int level, Operand* const node);
    void (*printOperator) (int spaces, Operator* const node);
    void (*printUnaryOperator) (int level, UnaryOperator* const node);
    void (*printBinaryOperator) (int level, BinaryOperator* const node);
    void (*printTernaryOperator) (int level, TernaryOperator* const node);

};



// ================================= //
//  Section:
//    KEY WORDS
// ================================= //

enum KeyWordType : int {
    KW_VOID,
    KW_INT,
    KW_INT_32,
    KW_INT_64,
    KW_FLOAT_32,
    KW_FLOAT_64,
    KW_STRING,
    KW_CONST,
    KW_FUNCTION,
    KW_IF,
    KW_FOR,
    KW_WHILE,
    KW_GOTO,
    KW_ENUM,
    KW_TYPE_DEF
};

struct KeyWord {
    int type;
    const char* str;
};

// ================================= //
//  Section:
//    OPERATORS
// ================================= //

enum OperatorEnum {
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_MODULO,
    OP_ADDRESS
};

struct OperatorMap {

    int (*unaryPlus) (Operand* op);
    int (*unaryMinus) (Operand* op);
    int (*addition) (Operand* a, Operand* b);
    int (*subtraction) (Operand* a, Operand* b);
    int (*multiplication) (Operand* a, Operand* b);
    int (*division) (Operand* a, Operand* b);
    int (*modulo) (Operand* a, Operand* b);
    int (*address) (Operand* op);

    constexpr OperatorMap() : 
        unaryPlus(NULL), 
        unaryMinus(NULL),
        addition(NULL), 
        subtraction(NULL),
        multiplication(NULL), 
        division(NULL),
        modulo(NULL),
        address(NULL)
    {
    };
    
};



// ================================= //
//  Section:
//    SYNTAX NODES
// ================================= //

struct SyntaxNode {

    static Scope* root;

    Scope* scope;
    Location* loc;

    virtual void print(Translator* const translator, int level) = 0;
    
};

struct Scope : SyntaxNode {

    std::vector<SyntaxNode*> children;

    // LOOK AT : unite?
    std::vector<Variable*> vars; // LOOK AT : maybe exchange for an hash, depends, how many vars are here typicaly per scope
    std::vector<Function*> fcns;
    std::vector<TypeDefinition*> customDataTypes;
    
    virtual void print(Translator* const translator, int level);

};

struct Enumerator : SyntaxNode, INamed {
    
    DataTypeEnum dtype;
    std::vector<Variable*> vars;

    virtual void print(Translator* const translator, int level);

};

struct Operand : SyntaxNode {

    DataTypeEnum dataTypeEnum;
    DataType* dtype; // !!! if DT_POINTER points to Pointer struct !!!
    void* value;

    int unrollExpression; // LOOk AT : maybe get rid of Operand itself and use Variable instead as before? or have two types of operand constant and dynamic?
    Expression* expression;

    Operand() {};
    Operand(Scope* scope);
    virtual void print(Translator* const translator, int level);
    
};

// evaluate function note:
//  varPart and constPart are used to simplify epression, where varPart is used to keep track of
//  only variable expression and constPart will 'sum' constant reamainder through recursive steps.
//  each time the 'end' of such simplyfing is reahced, means there is condition that doesn't for
//  any pre-defined reason allow it to simplyfy futher, the pointer swap in current poisition will
//  be made so current expression will be used to represent new one, simplyfied with varPart 
//  and constPart
struct Expression {
    virtual int evaluate(Operand** ans, Operand** constPart) = 0;
    virtual void print(Translator* const translator, int level = 0) = 0;
};

// LOOK AT : is there a better way
/*
struct VariableDefiniton : SyntaxNode {
    
    Variable* var;
    int flags;

    VariableDefiniton() {};
    VariableDefiniton(Location* loc);
    virtual void print(int level);

};
*/

struct TypeInitialization : Expression {

    std::vector<Variable*> attributes;

    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);

};

struct VariableDefinition : SyntaxNode {
    
    Variable* var;
    int flags;

    // only for custom data types as they will be linked at the end
    char* dtypeName;
    int dtypeNameLen;

    VariableDefinition() {};
    VariableDefinition(Location* loc);
    virtual void print(Translator* const translator, int level);

};

// LOOK AT : look up)
struct VariableAssignment : SyntaxNode {
    
    Variable* var;

    VariableAssignment() {};
    VariableAssignment(Location* loc);
    virtual void print(Translator* const translator, int level);

};

// TODO : why do variable even have a name????
// move it to the definition!!!!, the same for
// assignment
struct Variable : INamed, Operand {

    VariableDefinition* def;
    Variable* parentStruct;

    uint64_t flags; // TODO : get rid of, doesnt helped to solve the problem

    Variable();
    Variable(Location* loc);
    Variable(Scope* const sc, DataTypeEnum dtype);
    Variable(Scope* const sc, DataTypeEnum dtype, Location* loc);
    virtual void print(Translator* const translator, int level);

};

struct Function : SyntaxNode, INamed {
    
    std::vector<Variable*> inArgs;
    std::vector<DataTypeEnum> outArgs;
    
    Scope* bodyScope;
    
    int internalIdx; // if is > 0, then its internal function, and value represents unique id, otherwise should be ignored 
    
    Function() {};
    Function(Scope* sc, char* name, int nameLen, std::vector<Variable*> inArgs, std::vector<DataTypeEnum> outArgs, int internalIdx);
    virtual void print(Translator* const translator, int level);

};

struct FunctionCall : Expression, INamed {

    Function* fcn;
    std::vector<Variable*> inArgs;
    Variable* outArg;

    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);

};

struct Branch : SyntaxNode {

    std::vector<Scope*> scopes;
    std::vector<Operand*> expressions;
    
    virtual void print(Translator* const translator, int level);

};

struct WhileLoop : SyntaxNode {

    Scope* bodyScope;
    Operand* expression;
    
    virtual void print(Translator* const translator, int level);

};

struct ForLoop : SyntaxNode {

    Scope* bodyScope;
    Operand* expression;
    
    virtual void print(Translator* const translator, int level);

};

struct Operator {
    
    uint32_t word;
    int rank; // precedence of operator, zero->positive-whatever, high->low *(precedence seems too long for usage)
    uint64_t flag;
    int (*compare) (Operator* op, uint32_t word);

    void print(Translator* const translator, const int spaces = 0); // maybe separate function

};

// LOOK AT : think about better name
// what about ArithmeticExpression??
struct OperatorExpression : Expression {
    Operator* oper;
};

struct ExpressionWrapper : SyntaxNode {
    Operand* operand;
    virtual void print(Translator* const translator, int lines);
};

// LOOK AT : think about better name
struct WrapperExpression : Expression {
    Operand* operand;
    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);
};

struct UnaryExpression : OperatorExpression {
    Operand* operand;
    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);
};

struct BinaryExpression : UnaryExpression {
    Operand* operandA;
    Operand* operandB;
    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);
};

struct TernaryExpression : BinaryExpression {
    Variable* operandA;
    Variable* operandB;
    Variable* operandC;
    virtual int evaluate(Operand** ans, Operand** constPart);
    virtual void print(Translator* const translator, int level = 0);
};



// ================================= //
//  Section:
//    DATA TYPES
// ================================= //

enum DataTypeEnum : int {
    DT_VOID,
    DT_INT,
    DT_INT_32,
    DT_INT_64,
    DT_FLOAT_32,
    DT_FLOAT_64,
    DT_STRING,
    DT_POINTER,
    DT_MULTIPLE_TYPES,
    DT_CUSTOM,
    DT_UNDEFINED
};

struct DataType : OperatorMap, INamed {
    
    int size; // in bytes
    int rank;

    constexpr DataType() : 
            
        size(0), 
        rank(0),
        
        OperatorMap(),
        INamed(NULL, 0)

    {
    
    };

    constexpr DataType(
        
        char* const wd, 
        const int wdLen, 
        const int sz, 
        const int rk,
        
        int (*unaryPlus) (Operand*), 
        int (*unaryMinus) (Operand*),
        int (*addition) (Operand*, Operand*), 
        int (*subtraction) (Operand*, Operand*),
        int (*multiplication) (Operand*, Operand*),
        int (*division) (Operand*, Operand*),
        int (*modulo) (Operand*, Operand*),
        int (*address) (Operand*)
    
    ) : 
        
        size(sz), 
        rank(rk),
        
        OperatorMap(),
        INamed(wd, wdLen)
    
    {
        this->unaryPlus = unaryPlus;
        this->unaryMinus = unaryMinus;
        this->addition = addition;
        this->subtraction = subtraction;
        this->multiplication = multiplication;
        this->division = division;
        this->modulo = modulo;
        this->address = address;
    }

    ~DataType() {};

};

extern DataType dataTypes[9];

struct TypeDefinition : DataType, SyntaxNode {
    
    std::vector<Variable*> vars;

    Function* unaryPlus;
    Function* unaryMinus;
    Function* addition;
    Function* subtraction;
    Function* multiplication;
    Function* division;
    Function* modulo;
    Function* address;

    TypeDefinition() {};
    virtual void print(Translator* const translator, int level);

};

struct Pointer {
    DataType* pointsTo;
    DataTypeEnum pointsToEnum;
};