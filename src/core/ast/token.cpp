#include "token.hpp"

namespace Platinum
{
    namespace AST
    {
        Token::Token()
        {
            this->match = false;
        };

        Token::Token(string id, TrackedCode sequence, int decrement){
            this->match = true;
            this->id = id;
            this->sequence = sequence;
            this->decrement = decrement;
        };
    };
};