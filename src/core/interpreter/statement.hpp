#ifndef PLATINUM_CORE_INTERPRETER_STATEMENT_HPP_
#define PLATINUM_CORE_INTERPRETER_STATEMENT_HPP_

#include "../ast/parser.hpp"
#include "../data/value.hpp"
#include "context.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        class StatementResolution
        {
            public:
                bool didReturn;
                Data::Value returnValue;

                StatementResolution();
                StatementResolution(Data::Value returnValue);
        };

        namespace Statements
        {
            StatementResolution forStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution whileStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution returnStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution mutateStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution evaluateStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution ifStatement(shared_ptr<Context> parentContext, AST::Block statementBlock);
            StatementResolution conditionalChainStatement(shared_ptr<Context> parentContext, AST::ParsedCode statementBlocks);
        };
    };
};

#endif