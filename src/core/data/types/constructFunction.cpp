#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructFunction(Value ref)
        {
            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveFunction(context));
            }));

            this->call = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return boundRef->getPrimitiveFunction(context)(context, args);
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "function");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveFunction(context).toString());
            }));
        };
    };
};