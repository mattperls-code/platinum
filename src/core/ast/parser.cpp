#include "parser.hpp"
#include "../interpreter/context.hpp"

namespace Platinum
{
    namespace AST
    {
        void Parser::resetValidBlocks(vector<bool>& validBlocks, vector<BlockRule>& blockRules, string lastBlockId, bool lastBlockIsChaining)
        {
            for(int i = 0;i<validBlocks.size();i++)
            {
                if(blockRules[i].precursorIds.size() == 0){
                    validBlocks[i] = !lastBlockIsChaining;
                } else {
                    validBlocks[i] = false;
                    int j = 0;
                    while(!validBlocks[i] && j < blockRules[i].precursorIds.size())
                    {
                        if(lastBlockId == blockRules[i].precursorIds[j]){
                            validBlocks[i] = true;
                        };
                        j++;
                    };
                };
            };
        };

        void Parser::updateValidBlocks(vector<bool>& validBlocks, vector<BlockRule>& blockRules, string lastTokenId, int lastTokenIndex)
        {
            for(int i = 0;i<validBlocks.size();i++)
            {
                if(validBlocks[i] && lastTokenId != blockRules[i].tokenIds[lastTokenIndex]){
                    validBlocks[i] = false;
                };
            };
        };

        int Parser::withId(vector<TokenRule>& tokenRules, string id)
        {
            for(int i = 0;i<tokenRules.size();i++)
            {
                if(id == tokenRules[i].id){
                    return i;
                };
            };
            return -1;
        };

        bool Parser::isWhiteSpace(TrackedCode code)
        {
            for(int i = 0;i<code.set.size();i++)
            {
                if(code.set[i].c != ' '){
                    return false;
                };
            };
            return true;
        };

        ParsedCode Parser::createBlocks(shared_ptr<Interpreter::Context> context, TrackedCode code, vector<TokenRule>& tokenRules, vector<BlockRule>& blockRules, string startingBlockId)
        {
            ParsedCode blocks;

            TrackedCode currentSequence;
            vector<Token> tokens;

            vector<bool> validBlocks(blockRules.size());
            resetValidBlocks(validBlocks, blockRules, startingBlockId, false);

            for(int i = 0;i<code.set.size();i++)
            {
                currentSequence.set.push_back(code.set[i]);
                bool foundMatchingToken = false;
                int j = 0;
                while(!foundMatchingToken && j < blockRules.size())
                {
                    if(validBlocks[j]){
                        string requiredId = blockRules[j].tokenIds[tokens.size()];
                        Token tokenMatchTest = tokenRules[withId(tokenRules, requiredId)].match(currentSequence);

                        if(tokenMatchTest.match){
                            updateValidBlocks(validBlocks, blockRules, requiredId, tokens.size());

                            tokens.push_back(tokenMatchTest);
                            currentSequence.set.clear();
                            i -= tokenMatchTest.decrement;
                            foundMatchingToken = true;

                            if(tokens.size() == blockRules[j].tokenIds.size()){
                                blocks.push_back(Block(blockRules[j].id, blockRules[j].isChaining, tokens));
                                tokens.clear();
                                resetValidBlocks(validBlocks, blockRules, blocks.back().id, blocks.back().isChaining);
                            };
                        };
                    };
                    j++;
                };
            };
            if((currentSequence.set.size() != 0 && !isWhiteSpace(currentSequence)) || tokens.size() != 0){
                // TODO: rule out token literals if the current sequence is not a substring of one of their keys
                vector<string> possibleTokenIds;
                for(int i = 0;i<validBlocks.size();i++)
                {
                    if(validBlocks[i]){
                        bool foundDuplicate = false;
                        int j = 0;
                        while(!foundDuplicate && j < possibleTokenIds.size())
                        {
                            if(blockRules[i].tokenIds[tokens.size()] == possibleTokenIds[j]){
                                foundDuplicate = true;
                            };
                            j++;
                        };
                        if(!foundDuplicate){
                            possibleTokenIds.push_back(blockRules[i].tokenIds[tokens.size()]);
                        };
                    };
                };
                string possibleTokenIdsConcat;
                switch(possibleTokenIds.size())
                {
                    case 0:
                        possibleTokenIdsConcat = "(no possible tokens)";
                        break;
                    case 1:
                        possibleTokenIdsConcat = possibleTokenIds[0];
                        break;
                    case 2:
                        possibleTokenIdsConcat = possibleTokenIds[0] + " or " + possibleTokenIds[1];
                        break;
                    default:
                        for(int i = 0;i<possibleTokenIds.size() - 2;i++)
                        {
                            possibleTokenIdsConcat += possibleTokenIds[i] + ", ";
                        };
                        possibleTokenIdsConcat += possibleTokenIds[possibleTokenIds.size() - 2] + " or " + possibleTokenIds.back();
                        break;
                };
                if(currentSequence.set.size() != 0){
                    shared_ptr<Interpreter::Context> unterminatedTokenContext = context->createChild("UNTERMINATED_TOKEN", currentSequence.set.back().line, currentSequence.set.back().pos + 1);
                    string currentSequenceString = currentSequence.toString();
                    if(currentSequenceString.back() == ',' && isWhiteSpace(currentSequenceString.substr(0, currentSequenceString.size() - 1))){
                        currentSequenceString = "";
                    };
                    unterminatedTokenContext->throwError(Interpreter::Context::ErrorTypes::TOKEN, "Possible ids of incomplete token: " + possibleTokenIdsConcat + "\nCurrently unterminated token sequence: \"" + currentSequenceString + "\"");
                } else {
                    shared_ptr<Interpreter::Context> unterminatedBlockContext = context->createChild("UNTERMINATED_BLOCK", tokens.back().sequence.set.back().line, tokens.back().sequence.set.back().pos + 1);
                    unterminatedBlockContext->throwError(Interpreter::Context::ErrorTypes::BLOCK, "Possible ids of next token: " + possibleTokenIdsConcat);
                };
            };
            return blocks;
        };

        ParsedCode Parser::createExecutionBlocks(shared_ptr<Interpreter::Context> context, TrackedCode code)
        {
            return createBlocks(context, code, ExecutionTokens::rules, ExecutionBlocks::rules, "");
        };

        ParsedCode Parser::createEvaluationBlocks(shared_ptr<Interpreter::Context> context, TrackedCode code)
        {
            if(isWhiteSpace(code)){
                return ParsedCode();
            };

            TrackedCode manuallyTerminatedCode = code;
            manuallyTerminatedCode.set.push_back(TrackedChar(',', 0, 0));

            ParsedCode blocks = createBlocks(context, manuallyTerminatedCode, EvaluationTokens::rules, EvaluationBlocks::rules, "end");
            blocks.pop_back();

            return blocks;
        };
    };
};