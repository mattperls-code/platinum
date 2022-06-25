#include "blockRule.hpp"

namespace Platinum
{
    namespace AST
    {
        BlockRule::BlockRule(){};

        BlockRule::BlockRule(string id, bool isChaining)
        {
            this->id = id;
            this->isChaining = isChaining;
        };

        BlockRule& BlockRule::addPrecursorId(string precursorId)
        {
            this->precursorIds.push_back(precursorId);
            return *this;
        };

        BlockRule& BlockRule::addTokenId(string tokenId)
        {
            this->tokenIds.push_back(tokenId);
            return *this;
        };

        BlockRule ExecutionBlocks::_if = BlockRule("if", false)
            .addTokenId("if")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("end");
        
        BlockRule ExecutionBlocks::_ifChainElseIf = BlockRule("ifChainElseIf", true)
            .addTokenId("if")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("elseIf");
        
        BlockRule ExecutionBlocks::_ifChainElse = BlockRule("ifChainElse", true)
            .addTokenId("if")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("else");
        
        BlockRule ExecutionBlocks::_elseIf = BlockRule("elseIf", false)
            .addPrecursorId("ifChainElseIf")
            .addPrecursorId("elseIfChainElseIf")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("end");
        
        BlockRule ExecutionBlocks::_elseIfChainElseIf = BlockRule("elseIfChainElseIf", true)
            .addPrecursorId("ifChainElseIf")
            .addPrecursorId("elseIfChainElseIf")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("elseIf");
        
        BlockRule ExecutionBlocks::_elseIfChainElse = BlockRule("elseIfChainElse", true)
            .addPrecursorId("ifChainElseIf")
            .addPrecursorId("elseIfChainElseIf")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("else");

        BlockRule ExecutionBlocks::_else = BlockRule("else", false)
            .addPrecursorId("ifChainElse")
            .addPrecursorId("elseIfChainElse")
            .addTokenId("scope")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_for = BlockRule("for", false)
            .addTokenId("for")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_while = BlockRule("while", false)
            .addTokenId("while")
            .addTokenId("args")
            .addTokenId("scope")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_return = BlockRule("return", false)
            .addTokenId("return")
            .addTokenId("rValue")
            .addTokenId("end");
        
        BlockRule ExecutionBlocks::_mutate1 = BlockRule("mutate", false)
            .addTokenId("lValue1")
            .addTokenId("mutate")
            .addTokenId("rValue")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_mutate2 = BlockRule("mutate", false)
            .addTokenId("lValue2")
            .addTokenId("mutate")
            .addTokenId("rValue")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_mutate3 = BlockRule("mutate", false)
            .addTokenId("lValue1")
            .addTokenId("mutate")
            .addTokenId("end");

        BlockRule ExecutionBlocks::_evaluate = BlockRule("evaluate", false)
            .addTokenId("rValue")
            .addTokenId("end");
        
        vector<BlockRule> ExecutionBlocks::rules = {
            ExecutionBlocks::_if,
            ExecutionBlocks::_ifChainElseIf,
            ExecutionBlocks::_ifChainElse,
            ExecutionBlocks::_elseIf,
            ExecutionBlocks::_elseIfChainElseIf,
            ExecutionBlocks::_elseIfChainElse,
            ExecutionBlocks::_else,
            ExecutionBlocks::_for,
            ExecutionBlocks::_while,
            ExecutionBlocks::_return,
            ExecutionBlocks::_mutate1,
            ExecutionBlocks::_mutate2,
            ExecutionBlocks::_mutate3,
            ExecutionBlocks::_evaluate
        };

        BlockRule EvaluationBlocks::_term = BlockRule("term", true)
            .addPrecursorId("end")
            .addPrecursorId("colon")
            .addPrecursorId("unary")
            .addPrecursorId("binary")
            .addPrecursorId("dot")
            .addTokenId("term");

        BlockRule EvaluationBlocks::_group = BlockRule("group", true)
            .addPrecursorId("end")
            .addPrecursorId("colon")
            .addPrecursorId("unary")
            .addPrecursorId("binary")
            .addTokenId("group");

        BlockRule EvaluationBlocks::_unary = BlockRule("unary", true)
            .addPrecursorId("end")
            .addPrecursorId("colon")
            .addPrecursorId("unary")
            .addPrecursorId("binary")
            .addTokenId("unary");

        BlockRule EvaluationBlocks::_binary1 = BlockRule("binary", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("binary1");

        BlockRule EvaluationBlocks::_binary2 = BlockRule("binary", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("binary2");
        
        BlockRule EvaluationBlocks::_access = BlockRule("access", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("access");
        
        BlockRule EvaluationBlocks::_dot = BlockRule("dot", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("dot");
        
        BlockRule EvaluationBlocks::_call = BlockRule("call", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("call");
        
        BlockRule EvaluationBlocks::_funcDef = BlockRule("funcDef", true)
            .addPrecursorId("end")
            .addPrecursorId("colon")
            .addPrecursorId("unary")
            .addPrecursorId("binary")
            .addTokenId("func")
            .addTokenId("argsDef")
            .addTokenId("arrow")
            .addTokenId("callback");
        
        BlockRule EvaluationBlocks::_colon = BlockRule("colon", true)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("colon");
        
        BlockRule EvaluationBlocks::_end = BlockRule("end", false)
            .addPrecursorId("term")
            .addPrecursorId("group")
            .addPrecursorId("access")
            .addPrecursorId("call")
            .addPrecursorId("funcDef")
            .addTokenId("end");

        vector<BlockRule> EvaluationBlocks::rules = {
            EvaluationBlocks::_term,
            EvaluationBlocks::_group,
            EvaluationBlocks::_unary,
            EvaluationBlocks::_binary1,
            EvaluationBlocks::_binary2,
            EvaluationBlocks::_access,
            EvaluationBlocks::_dot,
            EvaluationBlocks::_call,
            EvaluationBlocks::_funcDef,
            EvaluationBlocks::_colon,
            EvaluationBlocks::_end,
        };
    };
};