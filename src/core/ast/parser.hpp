#ifndef PLATINUM_CORE_AST_PARSER_HPP_
#define PLATINUM_CORE_AST_PARSER_HPP_

#include <string>
#include <vector>
#include <memory>

#include "tokenRule.hpp"
#include "block.hpp"
#include "blockRule.hpp"

using namespace std;

namespace Platinum
{
    namespace Interpreter
    {
        class Context;
    };

    namespace AST
    {
        typedef vector<Block> ParsedCode;

        class Parser
        {   
            private:
                static void resetValidBlocks(vector<bool>& validBlocks, vector<BlockRule>& blockRules, string lastBlockId, bool lastBlockIsChaining);
                static void updateValidBlocks(vector<bool>& validBlocks, vector<BlockRule>& blockRules, string lastTokenId, int lastTokenIndex);

                static int withId(vector<TokenRule>& tokenRules, string id);

                static bool isWhiteSpace(TrackedCode code);

                static ParsedCode createBlocks(shared_ptr<Interpreter::Context>, TrackedCode code, vector<TokenRule>& tokenRules, vector<BlockRule>& blockRules, string startingBlockId);

            public:
                static ParsedCode createExecutionBlocks(shared_ptr<Interpreter::Context> context, TrackedCode code);
                static ParsedCode createEvaluationBlocks(shared_ptr<Interpreter::Context> context, TrackedCode code);
        };
    };
};

#endif