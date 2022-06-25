#include "tokenRule.hpp"

namespace Platinum
{
    namespace AST
    {
        TokenRule::TokenRule(){};

        TokenRule::TokenRule(string id, bool removeEnding)
        {
            this->id = id;
            this->removeEnding = removeEnding;

            this->usesCustomMatchValidator = false;
        };

        TokenRule::TokenRule(string id, function<Token(TrackedCode currentSequence)> customMatchValidator)
        {
            this->id = id;

            this->usesCustomMatchValidator = true;
            this->customMatchValidator = customMatchValidator;
        };

        TokenRule& TokenRule::addTokenLiteral(string tokenLiteral)
        {
            this->tokenLiterals.push_back(tokenLiteral);
            return *this;
        };

        TokenRule& TokenRule::addLegalEnding(string legalEnding)
        {
            this->legalEndings.push_back(legalEnding);
            return *this;
        };

        TokenRule& TokenRule::addIllegalEnding(string illegalEnding)
        {
            this->illegalEndings.push_back(illegalEnding);
            return *this;
        };

        Token TokenRule::match(TrackedCode currentSequence)
        {
            if(this->usesCustomMatchValidator){
                return this->customMatchValidator(currentSequence);
            };

            string sequenceString = currentSequence.toString();
            for(int i = 0;i<this->illegalEndings.size();i++)
            {
                if(sequenceString.size() >= this->illegalEndings[i].size() && sequenceString.substr(sequenceString.size() - this->illegalEndings[i].size()) == this->illegalEndings[i]){
                    return Token();
                };
            };

            bool endingFound = this->legalEndings.size() == 0;

            int endingSize = 0;
            {
                int i = 0;
                while(i < this->legalEndings.size() && !endingFound)
                {
                    if(sequenceString.size() >= this->legalEndings[i].size() && sequenceString.substr(sequenceString.size() - this->legalEndings[i].size()) == this->legalEndings[i]){
                        endingFound = true;
                        endingSize = this->legalEndings[i].size();
                    };
                    i++;
                };
            };
            if(endingFound){
                if(this->removeEnding){
                    currentSequence.set.resize(currentSequence.set.size() - endingSize);
                };
                while(currentSequence.set.size() > 0 && currentSequence.set.front().c == ' ')
                {
                    currentSequence.set.erase(currentSequence.set.begin());
                };
                while(currentSequence.set.size() > 0 && currentSequence.set.back().c == ' ')
                {
                    currentSequence.set.pop_back();
                };
                if(currentSequence.set.size() != 0){
                    if(this->tokenLiterals.size() > 0){
                        for(int i = 0;i<this->tokenLiterals.size();i++)
                        {
                            if(this->tokenLiterals[i] == currentSequence.toString()){
                                return Token(this->id, currentSequence, this->removeEnding ? endingSize : 0);
                            };
                        };
                    } else {
                        string sequenceStringWithoutStringLiterals;
                        bool inString = false;
                        for(int i = 0;i<currentSequence.set.size();i++)
                        {
                            if(currentSequence.set[i].c == '\"'){
                                inString = !inString;
                            } else if(!inString){
                                sequenceStringWithoutStringLiterals += currentSequence.set[i].c;
                            };
                        };
                        if(!inString){
                            int pDepth = 0;
                            int bDepth = 0;
                            int cDepth = 0;
                            for(int i = 0;i<sequenceStringWithoutStringLiterals.size();i++)
                            {
                                switch(sequenceStringWithoutStringLiterals[i])
                                {
                                    case '(':
                                        pDepth++;
                                        break;
                                    case ')':
                                        pDepth--;
                                        break;
                                    case '[':
                                        bDepth++;
                                        break;
                                    case ']':
                                        bDepth--;
                                        break;
                                    case '{':
                                        cDepth++;
                                        break;
                                    case '}':
                                        cDepth--;
                                        break;
                                };
                            };
                            if(pDepth == 0 && bDepth == 0 && cDepth == 0){
                                return Token(this->id, currentSequence, this->removeEnding ? endingSize : 0);
                            };
                        };
                    };
                };
            };
            return Token();
        };

        TokenRule ExecutionTokens::_if = TokenRule("if", true)
            .addTokenLiteral("if")
            .addLegalEnding("(");

        TokenRule ExecutionTokens::_elseIf = TokenRule("elseIf", true)
            .addTokenLiteral("else if")
            .addLegalEnding("(");

        TokenRule ExecutionTokens::_else = TokenRule("else", true)
            .addTokenLiteral("else")
            .addLegalEnding("{");

        TokenRule ExecutionTokens::_for = TokenRule("for", true)
            .addTokenLiteral("for")
            .addLegalEnding("(");

        TokenRule ExecutionTokens::_while = TokenRule("while", true)
            .addTokenLiteral("while")
            .addLegalEnding("(");

        TokenRule ExecutionTokens::_args = TokenRule("args", false)
            .addLegalEnding(")");

        TokenRule ExecutionTokens::_scope = TokenRule("scope", false)
            .addLegalEnding("}");

        TokenRule ExecutionTokens::_return = TokenRule("return", true)
            .addLegalEnding(" ")
            .addLegalEnding("(")
            .addLegalEnding("[")
            .addLegalEnding("{")
            .addTokenLiteral("return");

        TokenRule ExecutionTokens::_mutate = TokenRule("mutate", false)
            .addTokenLiteral("=")
            .addTokenLiteral("+=")
            .addTokenLiteral("-=")
            .addTokenLiteral("*=")
            .addTokenLiteral("/=")
            .addTokenLiteral("%=")
            .addTokenLiteral("&=")
            .addTokenLiteral("|=")
            .addTokenLiteral("++")
            .addTokenLiteral("--");

        TokenRule ExecutionTokens::_lValue1 = TokenRule("lValue1", true)
            .addLegalEnding("+=")
            .addLegalEnding("-=")
            .addLegalEnding("*=")
            .addLegalEnding("/=")
            .addLegalEnding("%=")
            .addLegalEnding("&=")
            .addLegalEnding("|=")
            .addLegalEnding("++")
            .addLegalEnding("--");
        
        TokenRule ExecutionTokens::_lValue2 = TokenRule("lValue2", (function<Token(TrackedCode currentSequence)>) [](AST::TrackedCode currentSequence) -> Token {
            if(currentSequence.set.size() >= 3){
                char thirdToLast = currentSequence.set[currentSequence.set.size() - 3].c;
                char secondToLast = currentSequence.set[currentSequence.set.size() - 2].c;
                char last = currentSequence.set.back().c;
                if(
                    (thirdToLast != '=' && thirdToLast != '!' && thirdToLast != '<' && thirdToLast != '>') &&
                    secondToLast == '=' &&
                    (last != '=' && last != '>')
                ){
                    currentSequence.set.resize(currentSequence.set.size() - 2);
                    Token matchResult = TokenRule("lValue2", false).match(currentSequence);
                    matchResult.decrement = 2;
                    return matchResult;
                };
            };
            return Token();
        });

        TokenRule ExecutionTokens::_rValue = TokenRule("rValue", true)
            .addLegalEnding(";");

        TokenRule ExecutionTokens::_end = TokenRule("end", false)
            .addTokenLiteral(";");

        vector<TokenRule> ExecutionTokens::rules = {
            ExecutionTokens::_if,
            ExecutionTokens::_elseIf,
            ExecutionTokens::_else,
            ExecutionTokens::_for,
            ExecutionTokens::_while,
            ExecutionTokens::_args,
            ExecutionTokens::_scope,
            ExecutionTokens::_return,
            ExecutionTokens::_mutate,
            ExecutionTokens::_lValue1,
            ExecutionTokens::_lValue2,
            ExecutionTokens::_rValue,
            ExecutionTokens::_end,
        };

        TokenRule EvaluationTokens::_term = TokenRule("term", true)
            .addLegalEnding("==")
            .addLegalEnding("!=")
            .addLegalEnding("<")
            .addLegalEnding(">")
            .addLegalEnding("+")
            .addLegalEnding("-")
            .addLegalEnding("*")
            .addLegalEnding("/")
            .addLegalEnding("%")
            .addLegalEnding("&&")
            .addLegalEnding("||")
            .addLegalEnding("[")
            .addLegalEnding(".")
            .addLegalEnding("(")
            .addLegalEnding("=>")
            .addLegalEnding(":")
            .addLegalEnding(",");

        TokenRule EvaluationTokens::_group = TokenRule("group", false)
            .addLegalEnding(")");

        TokenRule EvaluationTokens::_unary = TokenRule("unary", false)
            .addTokenLiteral("!")
            .addTokenLiteral("-")
            .addTokenLiteral("*");
        
        TokenRule EvaluationTokens::_binary1 = TokenRule("binary1", false)
            .addTokenLiteral("==")
            .addTokenLiteral("!=")
            .addTokenLiteral("<=")
            .addTokenLiteral(">=")
            .addTokenLiteral("+")
            .addTokenLiteral("-")
            .addTokenLiteral("*")
            .addTokenLiteral("/")
            .addTokenLiteral("%")
            .addTokenLiteral("&&")
            .addTokenLiteral("||");
        
        TokenRule EvaluationTokens::_binary2 = TokenRule("binary2", (function<Token(TrackedCode currentSequence)>) [](AST::TrackedCode currentSequence) -> Token {
            if(currentSequence.set.size() >= 2){
                if((currentSequence.set[currentSequence.set.size() - 2].c == '<' || currentSequence.set[currentSequence.set.size() - 2].c == '>') && currentSequence.set.back().c != '='){
                    currentSequence.set.pop_back();
                    return Token("binary2", currentSequence, 1);
                };
            };
            return Token();
        });

        TokenRule EvaluationTokens::_access = TokenRule("access", false)
            .addLegalEnding("]");
        
        TokenRule EvaluationTokens::_dot = TokenRule("dot", false)
            .addTokenLiteral(".");
        
        TokenRule EvaluationTokens::_call = TokenRule("call", false)
            .addLegalEnding(")");

        TokenRule EvaluationTokens::_func = TokenRule("func", false)
            .addTokenLiteral("$");

        TokenRule EvaluationTokens::_argsDef = TokenRule("argsDef", false)
            .addLegalEnding(")");
        
        TokenRule EvaluationTokens::_arrow = TokenRule("arrow", false)
            .addLegalEnding("=>");
        
        TokenRule EvaluationTokens::_callback = TokenRule("callback", false)
            .addLegalEnding("}");

        TokenRule EvaluationTokens::_colon = TokenRule("colon", false)
            .addTokenLiteral(":");

        TokenRule EvaluationTokens::_end = TokenRule("end", false)
            .addTokenLiteral(",");
        
        vector<TokenRule> EvaluationTokens::rules = {
            EvaluationTokens::_term,
            EvaluationTokens::_group,
            EvaluationTokens::_unary,
            EvaluationTokens::_binary1,
            EvaluationTokens::_binary2,
            EvaluationTokens::_access,
            EvaluationTokens::_dot,
            EvaluationTokens::_call,
            EvaluationTokens::_func,
            EvaluationTokens::_argsDef,
            EvaluationTokens::_arrow,
            EvaluationTokens::_callback,
            EvaluationTokens::_colon,
            EvaluationTokens::_end,
        };
    };
};