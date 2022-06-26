#include "../core/interpreter/standardLibrary.hpp"

namespace Platinum
{
    namespace Modules
    {
        Data::Value types()
        {
            unordered_map<string, Data::Value> components;

            components["Void"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 0){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected no args");
                };
                return Data::makeValue();
            })));

            components["Boolean"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveBoolean(context));
            })));

            components["Number"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveNumber(context));
            })));

            components["String"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveString(context));
            })));

            components["Array"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveArray(context));
            })));

            components["Object"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveObject(context));
            })));

            components["Function"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->getPrimitiveFunction(context));
            })));

            components["typeof"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return args[0]->implicitType(context);
            })));

            components["isVoid"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isVoid());
            })));

            components["isBoolean"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isBoolean());
            })));

            components["isNumber"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isNumber());
            })));

            components["isString"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isString());
            })));

            components["isArray"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isArray());
            })));

            components["isObject"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isObject());
            })));

            components["isFunction"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(args[0]->isFunction());
            })));

            components["parseNumber"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                // TODO: should not allow for partially numeric strings
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                try {
                    return Data::makeValue(stof(args[0]->getPrimitiveString(context)));
                } catch(invalid_argument){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Invalid numeric string");
                };
            })));

            components["toString"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return args[0]->implicitToString(context);
            })));
            
            components["toArray"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                string str = args[0]->implicitToString(context)->getPrimitiveString(context);
                vector<Data::Value> array;
                for(int i = 0;i<str.size();i++)
                {
                    array.push_back(Data::makeValue((string){ str[i] }));
                };
                return Data::makeValue(array);
            })));

            components["createGenerator"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                // TODO

                return Data::makeValue();
            })));

            return Data::makeValue(components);
        };
    };
};