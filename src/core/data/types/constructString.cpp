#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructString(Value ref)
        {
            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveString(context));
            }));

            this->binaryEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveString(context) == args[0]->getPrimitiveString(context));
                } else {
                    return makeValue(false);
                };
            }));

            this->binaryNotEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveString(context) != args[0]->getPrimitiveString(context));
                } else {
                    return makeValue(true);
                };
            }));

            this->binaryAdd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveString(context) + args[0]->getPrimitiveString(context));
            }));

            this->binaryMultiply = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                float arg = args[0]->getPrimitiveNumber(context);
                int rounded = roundf(arg);

                if(fabs(rounded - arg) < 0.00001 && arg > 0){
                    string resultant;
                    for(int i = 0;i<rounded;i++)
                    {
                        resultant += boundRef->getPrimitiveString(context);
                    };
                    return makeValue(resultant);
                } else {
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Operand applied to binary multiply op of string must be a positive integer");
                };
            }));

            this->mutativeBinaryAdd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<string>(boundRef->primitive) += args[0]->getPrimitiveString(context);
                return makeValue();
            }));

            this->mutativeBinaryMultiply = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                float arg = args[0]->getPrimitiveNumber(context);
                int rounded = roundf(arg);
                if(fabs(rounded - arg) < 0.00001 && arg > 0){
                    string resultant;
                    for(int i = 0;i<rounded;i++)
                    {
                        resultant += boundRef->getPrimitiveString(context);
                    };
                    get<string>(boundRef->primitive) = resultant;
                    return makeValue();
                } else {
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Operand applied to binary multiply op of string must be a positive integer");
                };
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "string");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveString(context));
            }));
        };
    };
};