#include <iostream>
#include <fstream>
#include <filesystem>

#include "../core/interpreter/standardLibrary.hpp"

namespace Platinum
{
    namespace Modules
    {
        Data::Value fs()
        {
            unordered_map<string, Data::Value> components;

            components["readDir"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                string path = args[0]->getPrimitiveString(context);
                vector<Data::Value> dirChildren;

                for(const auto& child : filesystem::directory_iterator(path))
                {
                    dirChildren.push_back(Data::makeValue(child.path()));
                };

                return Data::makeValue(dirChildren);
            })));

            components["readFile"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };

                string path = args[0]->getPrimitiveString(context);

                ifstream fileTarget(path);
                if(fileTarget.fail()){
                    context->throwError(Interpreter::Context::ErrorTypes::FILE, "File \"" + path + "\" was unable to be read");
                };
                string file;
                string line;
                while(getline(fileTarget, line))
                {
                    file += line + '\n';
                };
                fileTarget.close();

                return Data::makeValue(file);
            })));

            components["writeFile"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 2 arg");
                };
                string path = args[0]->getPrimitiveString(context);
                string content = args[1]->getPrimitiveString(context);

                ofstream file(path);
                file << content;
                file.close();

                return Data::makeValue();
            })));

            components["appendFile"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 2 arg");
                };
                string path = args[0]->getPrimitiveString(context);
                string content = args[1]->getPrimitiveString(context);

                ofstream file(path, ios::app);
                file << content;
                file.close();

                return Data::makeValue();
            })));

            components["delete"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                string path = args[0]->getPrimitiveString(context);

                return Data::makeValue((float) filesystem::remove_all(path));
            })));

            components["rename"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 2 arg");
                };
                string path = args[0]->getPrimitiveString(context);
                string newName = args[1]->getPrimitiveString(context);

                filesystem::rename(path, newName);
                
                return Data::makeValue();
            })));

            return Data::makeValue(components);
        };
    };
};