#ifndef PLATINUM_CORE_AST_BLOCK_RULE_HPP_
#define PLATINUM_CORE_AST_BLOCK_RULE_HPP_

#include <string>
#include <vector>

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class BlockRule
        {
            public:
                string id;
                bool isChaining;
                vector<string> precursorIds;
                vector<string> tokenIds;
                
                BlockRule();
                BlockRule(string id, bool isChaining);

                BlockRule& addPrecursorId(string precursorId);
                BlockRule& addTokenId(string tokenId);
        };

        class ExecutionBlocks
        {
            private:
                static BlockRule _if;
                static BlockRule _ifChainElseIf;
                static BlockRule _ifChainElse;
                static BlockRule _elseIf;
                static BlockRule _elseIfChainElseIf;
                static BlockRule _elseIfChainElse;
                static BlockRule _else;
                static BlockRule _for;
                static BlockRule _while;
                static BlockRule _return;
                static BlockRule _mutate1;
                static BlockRule _mutate2;
                static BlockRule _mutate3;
                static BlockRule _evaluate;
            public:
                static vector<BlockRule> rules;
        };

        class EvaluationBlocks
        {
            private:
                static BlockRule _term;
                static BlockRule _group;
                static BlockRule _unary;
                static BlockRule _binary1;
                static BlockRule _binary2;
                static BlockRule _access;
                static BlockRule _dot;
                static BlockRule _call;
                static BlockRule _funcDef;
                static BlockRule _colon;
                static BlockRule _end;
            public:
                static vector<BlockRule> rules;
        };
    };
};

#endif