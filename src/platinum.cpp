#include <string>
#include <vector>

#include "core/interpreter/execute.hpp"

using namespace std;

int main(int argc, char** argv)
{
    vector<string> args(argv, argv + argc);
    Platinum::Interpreter::execute(args);
    return 0;
};