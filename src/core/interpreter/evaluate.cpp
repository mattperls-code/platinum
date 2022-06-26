#include <iostream>
#include <string>
#include <vector>
#include <variant>

#include "../data/value.hpp"
#include "evaluate.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        string trim(string str)
        {
            while(str.size() > 0 && str[0] == ' ')
            {
                str.erase(str.begin());
            };
            while(str.size() > 0 && str.back() == ' ')
            {
                str.pop_back();
            };
            return str;
        };

        AST::TrackedCode trimFront(AST::TrackedCode str)
        {
            while(str.set.size() > 0 && str.set[0].c == ' ')
            {
                str.set.erase(str.set.begin());
            };
            return str;
        };

        AST::TrackedCode trimBack(AST::TrackedCode str)
        {
            while(str.set.size() > 0 && str.set.back().c == ' ')
            {
                str.set.pop_back();
            };
            return str;
        };

        bool isNumericChar(char c)
        {
            return (
                c == '0' ||
                c == '1' ||
                c == '2' ||
                c == '3' ||
                c == '4' ||
                c == '5' ||
                c == '6' ||
                c == '7' ||
                c == '8' ||
                c == '9'
            );
        };

        Data::Value evaluateExpression(shared_ptr<Context> context, AST::TrackedCode expression)
        {
            return evaluateBlocks(context, AST::Parser::createEvaluationBlocks(context, expression));
        };

        Data::Value evaluateBlocks(shared_ptr<Context> context, AST::ParsedCode expressionBlocks)
        {
            vector<AST::TrackedCode> currentUnaryOperations;
            vector<Data::Value> valueNodes;
            vector<AST::TrackedCode> operationNodes;

            if(expressionBlocks.size() == 0){
                return Data::makeValue();
            };

            for(int i = 0;i<expressionBlocks.size();i++)
            {
                if(expressionBlocks[i].id == "unary"){
                    currentUnaryOperations.push_back(expressionBlocks[i].sequence[0].sequence);
                } else if (expressionBlocks[i].id == "term"){
                    shared_ptr<Context> termContext = context->createChild("TERM", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    if(i + 2 < expressionBlocks.size() && expressionBlocks[i + 1].id == "dot" && expressionBlocks[i + 2].id == "term" && isNumericChar(trim(expressionBlocks[i].sequence[0].sequence.toString())[0]) && isNumericChar(trim(expressionBlocks[i + 2].sequence[0].sequence.toString())[0])){
                        valueNodes.push_back(evaluateNumber(termContext, expressionBlocks[i].sequence[0].sequence.set, expressionBlocks[i + 2].sequence[0].sequence.set));
                        i += 2;
                    } else {
                        valueNodes.push_back(evaluateTerm(termContext, expressionBlocks[i].sequence[0].sequence.set));
                        termContext->merge();
                    };
                } else if(expressionBlocks[i].id == "group"){
                    shared_ptr<Context> groupContext = context->createChild("GROUP", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    valueNodes.push_back(evaluateGroup(groupContext, expressionBlocks[i].sequence[0].sequence.set));
                } else if(expressionBlocks[i].id == "funcDef") {
                    shared_ptr<Context> funcDefContext = context->createChild("FUNC_DEF", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    valueNodes.push_back(evaluateFuncDef(funcDefContext, expressionBlocks[i].sequence[1].sequence.set, expressionBlocks[i].sequence[3].sequence.set));
                } else if(expressionBlocks[i].id == "binary"){
                    operationNodes.push_back(expressionBlocks[i].sequence[0].sequence.set);
                    valueNodes.back() = evaluateUnaryOperations(context, valueNodes.back(), currentUnaryOperations);
                    currentUnaryOperations.clear();
                } else if(expressionBlocks[i].id == "access"){
                    shared_ptr<Context> accessContext = context->createChild("ACCESS", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    Data::Value accessorValue = evaluateGroup(accessContext, expressionBlocks[i].sequence[0].sequence.set);
                    valueNodes.back() = evaluateAccess(accessContext, valueNodes.back(), accessorValue);
                } else if(expressionBlocks[i].id == "dot"){
                    if(i + 1 < expressionBlocks.size()){
                        i++;
                        shared_ptr<Context> accessContext = context->createChild("ACCESS", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                        valueNodes.back() = evaluateAccess(accessContext, valueNodes.back(), Data::makeValue(trim(expressionBlocks[i].sequence[0].sequence.toString())));
                    } else {
                        shared_ptr<Context> dotContext = context->createChild("DOT", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                        dotContext->throwError(Context::ErrorTypes::BLOCK, "No term following dot");
                    };
                } else if(expressionBlocks[i].id == "call"){
                    shared_ptr<Context> callContext = context->createChild("CALL", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    valueNodes.back() = evaluateCall(callContext, valueNodes.back(), expressionBlocks[i].sequence[0].sequence);
                } else if(expressionBlocks[i].id == "colon" || expressionBlocks[i].id == "end"){
                    shared_ptr<Context> expressionBreakingBlockContext = context->createChild(expressionBlocks[i].id == "colon" ? "COLON" : "END", expressionBlocks[i].sequence[0].sequence.set[0].line, expressionBlocks[i].sequence[0].sequence.set[0].pos);
                    expressionBreakingBlockContext->throwError(Context::ErrorTypes::BLOCK, "Unexpected expression breaking block");
                }
            };
            valueNodes.back() = evaluateUnaryOperations(context, valueNodes.back(), currentUnaryOperations);

            for(int i = 0;i<operationNodes.size();i++)
            {
                string opString = operationNodes[i].toString();
                shared_ptr<Context> binaryContext = context->createChild("BINARY", operationNodes[i].set[0].line, operationNodes[i].set[0].pos);

                if(opString == "*" || opString == "/" || opString == "%"){
                    if(opString == "*"){
                        valueNodes[i] = valueNodes[i]->binaryMultiply(binaryContext, valueNodes[i + 1]);
                    } else if(opString == "/"){
                        valueNodes[i] = valueNodes[i]->binaryDivide(binaryContext, valueNodes[i + 1]);
                    } else {
                        valueNodes[i] = valueNodes[i]->binaryModulo(binaryContext, valueNodes[i + 1]);
                    };

                    valueNodes.erase(valueNodes.begin() + i + 1);
                    operationNodes.erase(operationNodes.begin() + i);
                    i--;
                };
            };

            for(int i = 0;i<operationNodes.size();i++)
            {
                string opString = operationNodes[i].toString();
                shared_ptr<Context> binaryContext = context->createChild("BINARY", operationNodes[i].set[0].line, operationNodes[i].set[0].pos);

                if(opString == "+" || opString == "-"){
                    if(opString == "+"){
                        valueNodes[i] = valueNodes[i]->binaryAdd(binaryContext, valueNodes[i + 1]);
                    } else {
                        valueNodes[i] = valueNodes[i]->binarySubtract(binaryContext, valueNodes[i + 1]);
                    };

                    valueNodes.erase(valueNodes.begin() + i + 1);
                    operationNodes.erase(operationNodes.begin() + i);
                    i--;
                };
            };

            for(int i = 0;i<operationNodes.size();i++)
            {
                string opString = operationNodes[i].toString();
                shared_ptr<Context> binaryContext = context->createChild("BINARY", operationNodes[i].set[0].line, operationNodes[i].set[0].pos);

                if(opString == "<" || opString == ">" || opString == "<=" || opString == ">="){
                    if(opString == "<"){
                        valueNodes[i] = valueNodes[i]->binaryLess(binaryContext, valueNodes[i + 1]);
                    } else if(opString == ">"){
                        valueNodes[i] = valueNodes[i]->binaryGreater(binaryContext, valueNodes[i + 1]);
                    } else if(opString == "<="){
                        valueNodes[i] = valueNodes[i]->binaryLessOrEquals(binaryContext, valueNodes[i + 1]);
                    } else {
                        valueNodes[i] = valueNodes[i]->binaryGreaterOrEquals(binaryContext, valueNodes[i + 1]);
                    };

                    valueNodes.erase(valueNodes.begin() + i + 1);
                    operationNodes.erase(operationNodes.begin() + i);
                    i--;
                };
            };

            for(int i = 0;i<operationNodes.size();i++)
            {
                string opString = operationNodes[i].toString();
                shared_ptr<Context> binaryContext = context->createChild("BINARY", operationNodes[i].set[0].line, operationNodes[i].set[0].pos);

                if(opString == "==" || opString == "!="){
                    if(opString == "=="){
                        valueNodes[i] = valueNodes[i]->binaryEquals(binaryContext, valueNodes[i + 1]);
                    } else {
                        valueNodes[i] = valueNodes[i]->binaryNotEquals(binaryContext, valueNodes[i + 1]);
                    };

                    valueNodes.erase(valueNodes.begin() + i + 1);
                    operationNodes.erase(operationNodes.begin() + i);
                    i--;
                };
            };

            for(int i = 0;i<operationNodes.size();i++)
            {
                string opString = operationNodes[i].toString();
                shared_ptr<Context> binaryContext = context->createChild("BINARY", operationNodes[i].set[0].line, operationNodes[i].set[0].pos);

                if(opString == "&&" || opString == "||"){
                    if(opString == "&&"){
                        valueNodes[i] = valueNodes[i]->binaryAnd(binaryContext, valueNodes[i + 1]);
                    } else {
                        valueNodes[i] = valueNodes[i]->binaryOr(binaryContext, valueNodes[i + 1]);
                    };

                    valueNodes.erase(valueNodes.begin() + i + 1);
                    operationNodes.erase(operationNodes.begin() + i);
                    i--;
                };
            };

            return valueNodes[0];
        };

        Data::Value evaluateTerm(shared_ptr<Context> context, AST::TrackedCode term)
        {
            string termString = trim(term.toString());
            if(termString == "void"){
                return Data::makeValue();
            } else if(termString == "true" || termString == "false"){
                return Data::makeValue(termString == "true");
            } else if(termString[0] == '\"'){
                return Data::makeValue(term.slice(1, term.set.size() - 1).toString());
            } else if(termString[0] == '['){
                shared_ptr<Context> arrayContext = context->createChild("ARRAY", term.set[0].line, term.set[0].pos);
                return evaluateArray(arrayContext, term);
            } else if(termString[0] == '{'){
                shared_ptr<Context> objectContext = context->createChild("OBJECT", term.set[0].line, term.set[0].pos);
                return evaluateObject(objectContext, term);
            } else if(isNumericChar(termString[0])) {
                return evaluateNumber(context, term);
            } else {
                Data::Value variableValue = context->getVariable(termString);
                context->merge();
                return variableValue;
            };
        };

        Data::Value evaluateGroup(shared_ptr<Context> context, AST::TrackedCode group)
        {
            return evaluateExpression(context, group.slice(1, group.set.size() - 1));
        };

        Data::Value evaluateNumber(shared_ptr<Context> context, AST::TrackedCode numericalString)
        {
            AST::TrackedCode trimmedNumericalString = trimFront(trimBack(numericalString));
            for(int i = 0;i<trimmedNumericalString.set.size();i++)
            {
                if(!isNumericChar(trimmedNumericalString.set[i].c)){
                    shared_ptr<Context> charContext = context->createChild("NUMERICAL_CHAR", trimmedNumericalString.set[i].line, trimmedNumericalString.set[i].pos);
                    charContext->throwError(Context::ErrorTypes::CONTEXTUAL, "Invalid numerical string");
                };
            };

            return Data::makeValue(stof(trimmedNumericalString.toString()));
        };

        Data::Value evaluateNumber(shared_ptr<Context> context, AST::TrackedCode integralNumericalString, AST::TrackedCode fractionalNumericalString)
        {
            AST::TrackedCode trimmedIntegralPart = trimFront(integralNumericalString);
            for(int i = 0;i<trimmedIntegralPart.set.size();i++)
            {
                if(!isNumericChar(trimmedIntegralPart.set[i].c)){
                    shared_ptr<Context> charContext = context->createChild("NUMERICAL_CHAR", trimmedIntegralPart.set[i].line, trimmedIntegralPart.set[i].pos);
                    charContext->throwError(Context::ErrorTypes::CONTEXTUAL, "Invalid numerical string");
                };
            };

            AST::TrackedCode trimmedFractionalPart = trimBack(fractionalNumericalString);
            for(int i = 0;i<trimmedFractionalPart.set.size();i++)
            {
                if(!isNumericChar(trimmedFractionalPart.set[i].c)){
                    shared_ptr<Context> charContext = context->createChild("NUMERICAL_CHAR", trimmedFractionalPart.set[i].line, trimmedFractionalPart.set[i].pos);
                    charContext->throwError(Context::ErrorTypes::CONTEXTUAL, "Invalid numerical string");
                };
            };

            return Data::makeValue(stof(trimmedIntegralPart.toString() + "." + trimmedFractionalPart.toString()));
        };

        Data::Value evaluateArray(shared_ptr<Context> context, AST::TrackedCode arrayLiteral)
        {
            AST::TrackedCode arrayLiteralInternals = arrayLiteral.slice(1, arrayLiteral.set.size() - 1);

            if(trimFront(trimBack(arrayLiteralInternals)).set.size() == 0){
                return Data::makeValue(vector<Data::Value>());
            };

            AST::ParsedCode arrayLiteralBlocks = AST::Parser::createEvaluationBlocks(context, arrayLiteralInternals);

            vector<Data::Value> arrayContents;
            AST::ParsedCode currentArrayItemBlocks;

            for(int i = 0;i<arrayLiteralBlocks.size();i++)
            {
                if(arrayLiteralBlocks[i].id == "end"){
                    arrayContents.push_back(evaluateBlocks(context, currentArrayItemBlocks));
                    currentArrayItemBlocks.clear();
                } else {
                    currentArrayItemBlocks.push_back(arrayLiteralBlocks[i]);
                };
            };
            arrayContents.push_back(evaluateBlocks(context, currentArrayItemBlocks));

            return Data::makeValue(arrayContents);
        };

        Data::Value evaluateObject(shared_ptr<Context> context, AST::TrackedCode objectLiteral)
        {
            AST::TrackedCode objectLiteralInternals = objectLiteral.slice(1, objectLiteral.set.size() - 1);

            if(trimFront(trimBack(objectLiteralInternals)).set.size() == 0){
                return Data::makeValue(unordered_map<string, Data::Value>());
            };

            AST::ParsedCode objectLiteralBlocks = AST::Parser::createEvaluationBlocks(context, objectLiteralInternals);

            unordered_map<string, Data::Value> objectContents;
            bool finishedKey = false;
            AST::ParsedCode currentKeyBlocks;
            AST::ParsedCode currentValueBlocks;

            for(int i = 0;i<objectLiteralBlocks.size();i++)
            {
                if(objectLiteralBlocks[i].id == "end"){
                    string key = evaluateBlocks(context, currentKeyBlocks)->implicitToString(context)->getPrimitiveString(context);
                    Data::Value value = evaluateBlocks(context, currentValueBlocks);

                    objectContents[key] = value;

                    finishedKey = false;
                    currentKeyBlocks.clear();
                    currentValueBlocks.clear();
                } else if(objectLiteralBlocks[i].id == "colon"){
                    finishedKey = true;
                } else if(finishedKey){
                    currentValueBlocks.push_back(objectLiteralBlocks[i]);
                } else {
                    currentKeyBlocks.push_back(objectLiteralBlocks[i]);
                };
            };

            if(finishedKey){
                string key = evaluateBlocks(context, currentKeyBlocks)->implicitToString(context)->getPrimitiveString(context);
                Data::Value value = evaluateBlocks(context, currentValueBlocks);

                objectContents[key] = value;
            };

            return Data::makeValue(objectContents);
        };

        Data::Value evaluateFuncDef(shared_ptr<Context> context, AST::TrackedCode argsDef, AST::TrackedCode callback)
        {
            vector<string> argNames;
            string argsString = argsDef.slice(1, argsDef.set.size() - 1).toString();

            string currentArg;
            for(int i = 0;i<argsString.size();i++)
            {
                if(argsString[i] == ','){
                    argNames.push_back(currentArg);
                    currentArg.clear();
                } else if(argsString[i] != ' '){
                    currentArg += argsString[i];
                };
            };
            if(currentArg != ""){
                argNames.push_back(currentArg);
            }

            return Data::makeValue(Data::Function(Data::makeValue(), Data::DynamicFunction(context, argNames, AST::Parser::createExecutionBlocks(context, callback.slice(1, callback.set.size() - 1)))));
        };

        Data::Value evaluateUnaryOperation(shared_ptr<Context> context, Data::Value operand, AST::TrackedCode operation)
        {
            string opString = trim(operation.toString());

            if(opString == "!"){
                return operand->unaryNot(context);
            } else if(opString == "-"){
                return operand->unaryMinus(context);
            } else if(opString == "*"){
                return operand->unaryCopy(context);
            };
        };

        Data::Value evaluateUnaryOperations(shared_ptr<Context> context, Data::Value operand, vector<AST::TrackedCode> operations)
        {
            if(operations.size() == 0){
                return operand;
            };

            Data::Value value = operand;
            for(int i = operations.size() - 1;i>=0;i--)
            {
                shared_ptr<Context> unaryContext = context->createChild("UNARY", operations[i].set[0].line, operations[i].set[0].pos);
                value = evaluateUnaryOperation(unaryContext, value, operations[i]);
            };
            
            return value;
        };

        Data::Value evaluateAccess(shared_ptr<Context> context, Data::Value operand, Data::Value accessor)
        {
            return operand->access(context, accessor);
        };

        Data::Value evaluateCall(shared_ptr<Context> context, Data::Value function, AST::TrackedCode argsLiteral)
        {
            AST::TrackedCode argsLiteralInternals = argsLiteral.slice(1, argsLiteral.set.size() - 1);

            if(trimFront(argsLiteralInternals).set.size() == 0){
                return function->call(context, vector<Data::Value>());
            };

            AST::ParsedCode argsLiteralBlocks = AST::Parser::createEvaluationBlocks(context, argsLiteralInternals);

            vector<Data::Value> args;
            AST::ParsedCode currentArgBlocks;

            for(int i = 0;i<argsLiteralBlocks.size();i++)
            {
                if(argsLiteralBlocks[i].id == "end"){
                    args.push_back(evaluateBlocks(context, currentArgBlocks));
                    currentArgBlocks.clear();
                } else {
                    currentArgBlocks.push_back(argsLiteralBlocks[i]);
                };
            };
            args.push_back(evaluateBlocks(context, currentArgBlocks));

            Data::Value returnValue = function->call(context, args);

            return returnValue;
        };
    };
};