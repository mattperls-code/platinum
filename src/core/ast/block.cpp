#include "block.hpp"

namespace Platinum
{
    namespace AST
    {
        Block::Block(string id, bool isChaining, vector<Token> sequence)
        {
            this->id = id;
            this->isChaining = isChaining;
            this->sequence = sequence;
        };
    };
};