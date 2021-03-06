#ifndef PLATINUM_CORE_INTERPRETER_STANDARD_LIBRARY_HPP_
#define PLATINUM_CORE_INTERPRETER_STANDARD_LIBRARY_HPP_

#include <string>
#include <unordered_map>

#include "../data/value.hpp"

using namespace std;

namespace Platinum
{
    namespace Modules
    {
        Data::Value types();
        Data::Value io();
        Data::Value math();
        Data::Value fs();
    };

    namespace Interpreter
    {
        unordered_map<string, Data::Value> standardLibrary();
    };
};

#endif