#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructVoid(Value ref)
        {
            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue();
            }));

            this->binaryEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(args[0]->isVoid());
            }));

            this->binaryNotEquals = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(!args[0]->isVoid());
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "void");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "void");
            }));
        };
    };
};