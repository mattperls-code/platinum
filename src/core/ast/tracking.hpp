#ifndef PLATINUM_CORE_AST_TRACKING_HPP_
#define PLATINUM_CORE_AST_TRACKING_HPP_

#include <string>
#include <vector>

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class TrackedChar
        {
            public:
                char c;
                int line;
                int pos;

                TrackedChar();
                TrackedChar(char c, int line, int pos);
        };

        class TrackedCode
        {
            public:
                vector<TrackedChar> set;

                TrackedCode();
                TrackedCode(vector<TrackedChar> trackedChars);
                TrackedCode(string str);

                string toString();
                TrackedCode slice(int start, int end);
        };
    };
};

#endif