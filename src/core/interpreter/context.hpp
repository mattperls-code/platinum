#ifndef PLATINUM_CORE_INTERPRETER_CONTEXT_HPP_
#define PLATINUM_CORE_INTERPRETER_CONTEXT_HPP_

#include <string>
#include <unordered_map>
#include <chrono>
#include <memory>

#include "../data/value.hpp"

using namespace std;

namespace Platinum
{
    namespace Data
    {
        class Prototype;
        typedef shared_ptr<Prototype> Value;
    };

    namespace Interpreter
    {
        class Context : public enable_shared_from_this<Context>
        {
            private:
                chrono::steady_clock::time_point timestamp;
                string id;

                int line;
                int pos;

                bool hasParent;
                shared_ptr<Context> parent;

                unordered_map<string, Data::Value> variables;
                
                string path();
            public:
                Context();
                Context(string id, int line, int pos);
                Context(string id, int line, int pos, shared_ptr<Context> parent);

                double getExecutionTime();

                shared_ptr<Context> createChild(string id, int line, int pos);

                bool hasVariable(string name);
                Data::Value getVariable(string name);
                void overwriteVariable(string name, Data::Value value);
                void merge();

                enum ErrorTypes { FILE, TOKEN, BLOCK, BASE_TYPE, NO_OP_DEF, CONTEXTUAL };
                void throwError(int type, string reason);

                void log(string message);
        };
    };
};

#endif