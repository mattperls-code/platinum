#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructBoolean(Value ref)
        {
            this->unaryNot = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(!boundRef->getPrimitiveBoolean(context));
            }));

            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveBoolean(context));
            }));

            this->binaryEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveBoolean(context) == args[0]->getPrimitiveBoolean(context));
                } else {
                    return makeValue(false);
                };
            }));
            
            this->binaryNotEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(!args[0]->isVoid()){
                    return makeValue(boundRef->getPrimitiveBoolean(context) != args[0]->getPrimitiveBoolean(context));
                } else {
                    return makeValue(true);
                };
            }));

            this->binaryAnd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveBoolean(context) && args[0]->getPrimitiveBoolean(context));
            }));

            this->binaryOr = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveBoolean(context) || args[0]->getPrimitiveBoolean(context));
            }));

            this->mutativeBinaryAnd = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<bool>(boundRef->primitive) &= args[0]->getPrimitiveBoolean(context);
                return makeValue();
            }));

            this->mutativeBinaryOr = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<bool>(boundRef->primitive) |= args[0]->getPrimitiveBoolean(context);
                return makeValue();
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "boolean");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveBoolean(context) ? (string) "true" : (string) "false");
            }));
        };
    };
};