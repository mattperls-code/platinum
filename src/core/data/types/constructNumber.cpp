#include <cmath>

#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructNumber(Value ref)
        {
            this->unaryMinus = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(-boundRef->getPrimitiveNumber(context));
            }));

            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context));
            }));

            this->binaryEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveNumber(context) == args[0]->getPrimitiveNumber(context));
                } else {
                    return makeValue(false);
                };
            }));

            this->binaryNotEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveNumber(context) != args[0]->getPrimitiveNumber(context));
                } else {
                    return makeValue(true);
                };
            }));

            this->binaryLess = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) < args[0]->getPrimitiveNumber(context));
            }));

            this->binaryGreater = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) > args[0]->getPrimitiveNumber(context));
            }));

            this->binaryLessOrEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) <= args[0]->getPrimitiveNumber(context));
            }));

            this->binaryGreaterOrEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) >= args[0]->getPrimitiveNumber(context));
            }));

            this->binaryAdd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) + args[0]->getPrimitiveNumber(context));
            }));

            this->binarySubtract = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) - args[0]->getPrimitiveNumber(context));
            }));

            this->binaryMultiply = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) * args[0]->getPrimitiveNumber(context));
            }));

            this->binaryDivide = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveNumber(context) / args[0]->getPrimitiveNumber(context));
            }));

            this->binaryModulo = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(fmod(boundRef->getPrimitiveNumber(context), args[0]->getPrimitiveNumber(context)));
            }));

            this->mutativeBinaryAdd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive) += args[0]->getPrimitiveNumber(context);
                return makeValue();
            }));

            this->mutativeBinarySubtract = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive) -= args[0]->getPrimitiveNumber(context);
                return makeValue();
            }));

            this->mutativeBinaryMultiply = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive) *= args[0]->getPrimitiveNumber(context);
                return makeValue();
            }));

            this->mutativeBinaryDivide = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive) /= args[0]->getPrimitiveNumber(context);
                return makeValue();
            }));

            this->mutativeBinaryModulo = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive) = fmod(get<float>(boundRef->primitive), args[0]->getPrimitiveNumber(context));
                return makeValue();
            }));

            this->mutativeIncrement = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive)++;
                return makeValue();
            }));

            this->mutativeDecrement = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<float>(boundRef->primitive)--;
                return makeValue();
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "number");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(to_string(boundRef->getPrimitiveNumber(context)));
            }));
        };
    };
};