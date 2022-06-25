#ifndef PLATINUM_CORE_AST_TOKEN_RULE_HPP_
#define PLATINUM_CORE_AST_TOKEN_RULE_HPP_

#include <string>
#include <vector>
#include <functional>

#include "tracking.hpp"
#include "token.hpp"

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class TokenRule
        {
            public:
                string id;
                vector<string> tokenLiterals;
                vector<string> legalEndings;
                vector<string> illegalEndings;
                bool removeEnding;

                bool usesCustomMatchValidator;
                function<Token(TrackedCode currentSequence)> customMatchValidator;
                
                TokenRule();
                TokenRule(string id, bool removeEnding);
                TokenRule(string id, function<Token(TrackedCode currentSequence)> customMatchValidator);

                TokenRule& addTokenLiteral(string tokenLiteral);
                TokenRule& addLegalEnding(string ending);
                TokenRule& addIllegalEnding(string ending);

                Token match(TrackedCode currentSequence);
        };

        class ExecutionTokens
        {
            private:
                static TokenRule _if;
                static TokenRule _elseIf;
                static TokenRule _else;
                static TokenRule _for;
                static TokenRule _while;
                static TokenRule _args;
                static TokenRule _scope;
                static TokenRule _return;
                static TokenRule _mutate;
                static TokenRule _lValue1;
                static TokenRule _lValue2;
                static TokenRule _rValue;
                static TokenRule _end;
            public:
                static vector<TokenRule> rules;
        };

        class EvaluationTokens
        {
            private:
                static TokenRule _term;
                static TokenRule _group;
                static TokenRule _unary;
                static TokenRule _binary1;
                static TokenRule _binary2;
                static TokenRule _access;
                static TokenRule _dot;
                static TokenRule _call;
                static TokenRule _func;
                static TokenRule _argsDef;
                static TokenRule _arrow;
                static TokenRule _callback;
                static TokenRule _colon;
                static TokenRule _end;
            public:
                static vector<TokenRule> rules;
        };
    };
};

#endif