#ifndef PLATINUM_CORE_INTERPRETER_EVALUATE_HPP_
#define PLATINUM_CORE_INTERPRETER_EVALUATE_HPP_

#include "../ast/parser.hpp"
#include "context.hpp"

using namespace std;

namespace Platinum
{
    namespace Interpreter
    {
        string trim(string str);
        AST::TrackedCode trimFront(AST::TrackedCode str);
        AST::TrackedCode trimBack(AST::TrackedCode str);

        bool isNumericChar(char c);
        
        Data::Value evaluateExpression(shared_ptr<Context> context, AST::TrackedCode expression);
        Data::Value evaluateBlocks(shared_ptr<Context> context, AST::ParsedCode expressionBlocks);

        Data::Value evaluateTerm(shared_ptr<Context> context, AST::TrackedCode term);
        Data::Value evaluateGroup(shared_ptr<Context> context, AST::TrackedCode group);

        Data::Value evaluateNumber(shared_ptr<Context> context, AST::TrackedCode numericalString);
        Data::Value evaluateNumber(shared_ptr<Context> context, AST::TrackedCode integralNumericalString, AST::TrackedCode fractionalNumericalString);
        Data::Value evaluateArray(shared_ptr<Context> context, AST::TrackedCode arrayLiteral);
        Data::Value evaluateObject(shared_ptr<Context> context, AST::TrackedCode objectLiteral);
        Data::Value evaluateFuncDef(shared_ptr<Context> context, AST::TrackedCode argsDef, AST::TrackedCode callback);

        Data::Value evaluateUnaryOperation(shared_ptr<Context> context, Data::Value operand, AST::TrackedCode operation);
        Data::Value evaluateUnaryOperations(shared_ptr<Context> context, Data::Value operand, vector<AST::TrackedCode> operations);
        Data::Value evaluateAccess(shared_ptr<Context> context, Data::Value operand, Data::Value accessor);
        Data::Value evaluateCall(shared_ptr<Context> context, Data::Value function, AST::TrackedCode argsLiteral);
    };
};

#endif