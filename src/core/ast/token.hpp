#ifndef PLATINUM_CORE_AST_TOKEN_HPP_
#define PLATINUM_CORE_AST_TOKEN_HPP_

#include <string>
#include <vector>

#include "tracking.hpp"

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class Token
        {
            public:
                bool match;
                string id;
                TrackedCode sequence;
                int decrement;

                Token();
                Token(string id, TrackedCode sequence, int decrement);
        };
    };
};

#endif