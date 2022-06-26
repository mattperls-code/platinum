#include <iostream>
#include <fstream>

#include "standardLibrary.hpp"
#include "context.hpp"
#include "execute.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        unordered_map<string, Data::Value> standardLibrary()
        {
            unordered_map<string, Data::Value> exposed;

            exposed["importLibrary"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg. (libraryName: string)");
                };

                string moduleName = args[0]->getPrimitiveString(context);

                if(moduleName == "types"){
                    return Modules::types();
                } else if(moduleName == "io"){
                    return Modules::io();
                } else if(moduleName == "math"){
                    return Modules::math();
                } else if(moduleName == "fs"){
                    return Modules::fs();
                };

                context->throwError(Context::ErrorTypes::CONTEXTUAL, "Invalid module name");
            })));

            exposed["importModule"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg. (path: string)");
                };

                string path = args[0]->getPrimitiveString(context);
                ifstream runTarget(path);
                if(runTarget.fail()){
                    context->throwError(Context::ErrorTypes::FILE, "File \"" + path + "\" was unable to be read");
                };
                string untrackedCode;
                string line;
                while(getline(runTarget, line))
                {
                    untrackedCode += line + '\n';
                };
                runTarget.close();

                executeOnContext(context, AST::TrackedCode(untrackedCode));

                context->merge();

                return Data::makeValue();
            })));

            return exposed;
        };
    };
};