#ifndef PLATINUM_CORE_AST_BLOCK_HPP_
#define PLATINUM_CORE_AST_BLOCK_HPP_

#include <string>
#include <vector>

#include "token.hpp"

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class Block
        {
            public:
                string id;
                bool isChaining;
                vector<Token> sequence;

                Block(string id, bool isChaining, vector<Token> sequence);
        };
    };
};

#endif