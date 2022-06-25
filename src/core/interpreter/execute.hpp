#ifndef PLATINUM_CORE_INTERPRETER_EXECUTE_HPP_
#define PLATINUM_CORE_INTERPRETER_EXECUTE_HPP_

#include <string>
#include <vector>

#include "../ast/parser.hpp"
#include "../data/value.hpp"
#include "context.hpp"
#include "statement.hpp"

using namespace std;

namespace Platinum
{
    namespace Interpreter
    {
        void execute(vector<string> args);

        StatementResolution executeOnContext(shared_ptr<Context> context, AST::ParsedCode blocks);
        StatementResolution executeOnContext(shared_ptr<Context> context, AST::TrackedCode code);
    };
};

#endif