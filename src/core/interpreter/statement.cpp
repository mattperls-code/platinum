#include <sstream>

#include "statement.hpp"
#include "evaluate.hpp"
#include "execute.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        StatementResolution::StatementResolution()
        {
            this->didReturn = false;
            this->returnValue = Data::makeValue();
        };

        StatementResolution::StatementResolution(Data::Value returnValue)
        {
            this->didReturn = true;
            this->returnValue = returnValue;
        };

        namespace Statements
        {
            StatementResolution forStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("FOR", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);

                AST::TrackedCode rawArgsLiteral = statementBlock.sequence[1].sequence;
                AST::TrackedCode argsLiteral = rawArgsLiteral.slice(1, rawArgsLiteral.set.size() - 1);
                argsLiteral.set.push_back(AST::TrackedChar(';', 0, 0));

                AST::ParsedCode args = AST::Parser::createExecutionBlocks(context, argsLiteral);
                if(args.size() != 3){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 3 args in for loop");
                };

                AST::Block initiator = args[0];
                if(initiator.id != "mutate" && initiator.id != "evaluate"){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected a mutate or evaluate block in first arg of for loop");
                };
                AST::Block condition = args[1];
                if(condition.id != "evaluate"){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected an evaluate block in second arg of for loop");
                };
                AST::Block postExecute = args[2];
                if(postExecute.id != "mutate" && postExecute.id != "evaluate"){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected a mutate or evaluate block in third arg of for loop");
                };

                AST::TrackedCode rawExecutionLiteral = statementBlock.sequence[2].sequence;
                AST::TrackedCode executionLiteral = rawExecutionLiteral.slice(1, rawExecutionLiteral.set.size() - 1);
                AST::ParsedCode executionBlocks = AST::Parser::createExecutionBlocks(context, executionLiteral);

                if(initiator.id == "mutate"){
                    mutateStatement(context, initiator);
                } else {
                    evaluateStatement(context, initiator);
                };

                int iterations = 0;
                // TODO: test the validity of iterations limit and potentially remove safeguard
                while(iterations < 10000 && evaluateExpression(context, condition.sequence[0].sequence)->getPrimitiveBoolean(context))
                {
                    StatementResolution resolution = executeOnContext(context, executionBlocks);
                    if(resolution.didReturn){
                        return resolution;
                    };
                    if(postExecute.id == "mutate"){
                        mutateStatement(context, postExecute);
                    } else {
                        evaluateStatement(context, postExecute);
                    };
                    iterations++;
                };

                return StatementResolution();
            };

            StatementResolution whileStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("WHILE", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);

                AST::TrackedCode rawArgsLiteral = statementBlock.sequence[1].sequence;
                AST::TrackedCode argsLiteral = rawArgsLiteral.slice(1, rawArgsLiteral.set.size() - 1);
                argsLiteral.set.push_back(AST::TrackedChar(';', 0, 0));

                AST::ParsedCode args = AST::Parser::createExecutionBlocks(context, argsLiteral);
                if(args.size() != 1){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg in while loop");
                };

                AST::Block condition = args[0];
                if(condition.id != "evaluate"){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected an evaluate block as arg of while loop");
                };

                AST::TrackedCode rawExecutionLiteral = statementBlock.sequence[2].sequence;
                AST::TrackedCode executionLiteral = rawExecutionLiteral.slice(1, rawExecutionLiteral.set.size() - 1);
                AST::ParsedCode executionBlocks = AST::Parser::createExecutionBlocks(context, executionLiteral);

                int iterations = 0;
                // TODO: test the validity of iterations limit and potentially remove safeguard
                while(iterations < 10000 && evaluateExpression(context, condition.sequence[0].sequence)->getPrimitiveBoolean(context))
                {
                    StatementResolution resolution = executeOnContext(context, executionBlocks);
                    if(resolution.didReturn){
                        return resolution;
                    };
                    iterations++;
                };

                return StatementResolution();
            };

            StatementResolution returnStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("RETURN", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);
                
                Data::Value returnValue = evaluateExpression(context, statementBlock.sequence[1].sequence);

                // TODO: should this merge?
                context->merge();

                return StatementResolution(returnValue);
            };

            StatementResolution mutateStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("MUTATE", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);

                Data::Value lvalue = evaluateExpression(context, statementBlock.sequence[0].sequence);

                string opString = statementBlock.sequence[1].sequence.toString();

                if((opString == "++" || opString == "--") == (statementBlock.sequence.size() == 4)){
                    if(opString == "++" || opString == "--"){
                        context->throwError(Interpreter::Context::CONTEXTUAL, "Cannot pass a binary operand to a postfix mutator");
                    } else {
                        context->throwError(Interpreter::Context::CONTEXTUAL, "Must pass a binary operand to a standard mutator");
                    };
                };
                
                if(opString == "="){
                    Data::Value rvalue = evaluateExpression(context, statementBlock.sequence[2].sequence);

                    *lvalue = *rvalue;
                } else if(opString != "++" && opString != "--"){
                    Data::Value rvalue = evaluateExpression(context, statementBlock.sequence[2].sequence);

                    if(opString == "+="){
                        lvalue->mutativeBinaryAdd(context, rvalue);
                    } else if(opString == "-="){
                        lvalue->mutativeBinarySubtract(context, rvalue);
                    } else if(opString == "*="){
                        lvalue->mutativeBinaryMultiply(context, rvalue);
                    } else if(opString == "/="){
                        lvalue->mutativeBinaryDivide(context, rvalue);
                    } else if(opString == "%="){
                        lvalue->mutativeBinaryModulo(context, rvalue);
                    } else if(opString == "&="){
                        lvalue->mutativeBinaryAnd(context, rvalue);
                    } else {
                        lvalue->mutativeBinaryOr(context, rvalue);
                    };
                } else if(opString == "++"){
                    lvalue->mutativeIncrement(context);
                } else {
                    Data::Value lvalue = evaluateExpression(context, statementBlock.sequence[0].sequence);

                    lvalue->mutativeDecrement(context);
                };

                // TODO: why is binding not working?
                lvalue->construct(lvalue);

                context->merge();

                return StatementResolution();
            };

            StatementResolution evaluateStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("EVALUATE", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);
                
                evaluateExpression(context, statementBlock.sequence[0].sequence);

                context->merge();
                
                return StatementResolution();
            };

            StatementResolution ifStatement(shared_ptr<Context> parentContext, AST::Block statementBlock)
            {
                shared_ptr<Context> context = parentContext->createChild("IF", statementBlock.sequence[0].sequence.set[0].line, statementBlock.sequence[0].sequence.set[0].pos);
            
                AST::TrackedCode rawArgsLiteral = statementBlock.sequence[1].sequence;
                AST::TrackedCode argsLiteral = rawArgsLiteral.slice(1, rawArgsLiteral.set.size() - 1);
                argsLiteral.set.push_back(AST::TrackedChar(';', 0, 0));

                AST::ParsedCode args = AST::Parser::createExecutionBlocks(context, argsLiteral);
                if(args.size() != 1){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg in conditional control flow statement");
                };

                AST::Block condition = args[0];
                if(condition.id != "evaluate"){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected an evaluate block as arg of conditional control flow statement");
                };

                if(evaluateExpression(context, condition.sequence[0].sequence)->getPrimitiveBoolean(context)){
                    AST::TrackedCode rawExecutionLiteral = statementBlock.sequence[2].sequence;
                    AST::TrackedCode executionLiteral = rawExecutionLiteral.slice(1, rawExecutionLiteral.set.size() - 1);

                    return executeOnContext(context, executionLiteral);
                } else {
                    return StatementResolution();
                };
            };

            StatementResolution conditionalChainStatement(shared_ptr<Context> parentContext, AST::ParsedCode statementBlocks)
            {
                shared_ptr<Context> context = parentContext->createChild("CONDITIONAL", statementBlocks[0].sequence[0].sequence.set[0].line, statementBlocks[0].sequence[0].sequence.set[0].pos);

                for(int i = 0;i<statementBlocks.size();i++)
                {
                    if(statementBlocks[i].id == "else"){
                        AST::TrackedCode rawExecutionLiteral = statementBlocks[i].sequence[0].sequence;
                        AST::TrackedCode executionLiteral = rawExecutionLiteral.slice(1, rawExecutionLiteral.set.size() - 1);

                        return executeOnContext(context, executionLiteral);
                    } else {
                        int argsIndex = (statementBlocks[i].id == "ifChainElseIf" || statementBlocks[i].id == "ifChainElse") ? 1 : 0;
                        int executionIndex = argsIndex + 1;

                        AST::TrackedCode rawArgsLiteral = statementBlocks[i].sequence[argsIndex].sequence;
                        AST::TrackedCode argsLiteral = rawArgsLiteral.slice(1, rawArgsLiteral.set.size() - 1);
                        argsLiteral.set.push_back(AST::TrackedChar(';', 0, 0));

                        AST::ParsedCode args = AST::Parser::createExecutionBlocks(context, argsLiteral);
                        if(args.size() != 1){
                            context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg in conditional control flow statement");
                        };

                        AST::Block condition = args[0];
                        if(condition.id != "evaluate"){
                            context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected an evaluate block as arg of conditional control flow statement");
                        };

                        if(evaluateExpression(context, condition.sequence[0].sequence)->getPrimitiveBoolean(context)){
                            AST::TrackedCode rawExecutionLiteral = statementBlocks[i].sequence[executionIndex].sequence;
                            AST::TrackedCode executionLiteral = rawExecutionLiteral.slice(1, rawExecutionLiteral.set.size() - 1);

                            return executeOnContext(context, executionLiteral);
                        };
                    };
                };

                return StatementResolution();
            };
        };
    };
};