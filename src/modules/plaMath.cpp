#include <cmath>
#include <limits>

#include "../core/interpreter/standardLibrary.hpp"

namespace Platinum
{
    namespace Modules
    {
        Data::Value math()
        {
            unordered_map<string, Data::Value> components;

            components["E"] = Data::makeValue((float) 2.71828182845904523536);

            components["PI"] = Data::makeValue((float) 3.14159265358979323846);

            components["abs"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(fabs(args[0]->getPrimitiveNumber(context)));
            })));

            components["round"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(roundf(args[0]->getPrimitiveNumber(context)));
            })));

            components["floor"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(floorf(args[0]->getPrimitiveNumber(context)));
            })));

            components["ceil"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(ceilf(args[0]->getPrimitiveNumber(context)));
            })));

            components["sin"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(sinf(args[0]->getPrimitiveNumber(context)));
            })));

            components["asin"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(asinf(args[0]->getPrimitiveNumber(context)));
            })));

            components["sinh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(sinhf(args[0]->getPrimitiveNumber(context)));
            })));

            components["asinh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(asinhf(args[0]->getPrimitiveNumber(context)));
            })));

            components["cos"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(cosf(args[0]->getPrimitiveNumber(context)));
            })));

            components["acos"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(acosf(args[0]->getPrimitiveNumber(context)));
            })));

            components["cosh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(coshf(args[0]->getPrimitiveNumber(context)));
            })));

            components["acosh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(acoshf(args[0]->getPrimitiveNumber(context)));
            })));

            components["tan"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(tanf(args[0]->getPrimitiveNumber(context)));
            })));

            components["atan"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(atanf(args[0]->getPrimitiveNumber(context)));
            })));

            components["atan2"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(atan2f(args[0]->getPrimitiveNumber(context), args[1]->getPrimitiveNumber(context)));
            })));

            components["tanh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(tanhf(args[0]->getPrimitiveNumber(context)));
            })));

            components["atanh"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(atanhf(args[0]->getPrimitiveNumber(context)));
            })));

            components["sqrt"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(sqrtf(args[0]->getPrimitiveNumber(context)));
            })));

            components["power"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(powf(args[0]->getPrimitiveNumber(context), args[1]->getPrimitiveNumber(context)));
            })));

            components["log"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(logf(args[0]->getPrimitiveNumber(context)));
            })));

            components["log10"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(log10f(args[0]->getPrimitiveNumber(context)));
            })));

            components["log2"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 1){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 1 arg");
                };
                return Data::makeValue(log2f(args[0]->getPrimitiveNumber(context)));
            })));

            components["min"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() == 0){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expects a non zero amount of args");
                };
                float min = numeric_limits<float>::max();
                for(int i = 0;i<args.size();i++)
                {
                    float arg = args[i]->getPrimitiveNumber(context);
                    if(arg < min){
                        min = arg;
                    };
                };
                return Data::makeValue(min);
            })));

            components["max"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() == 0){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expects a non zero amount of args");
                };
                float max = numeric_limits<float>::min();
                for(int i = 0;i<args.size();i++)
                {
                    float arg = args[i]->getPrimitiveNumber(context);
                    if(arg > max){
                        max = arg;
                    };
                };
                return Data::makeValue(max);
            })));

            return Data::makeValue(components);
        };
    };
};