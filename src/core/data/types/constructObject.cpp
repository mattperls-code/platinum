#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructObject(Value ref)
        {
            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveObject(context));
            }));

            this->access = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                string accessor = args[0]->getPrimitiveString(context);
                
                unordered_map<string, Value> primitiveObject = boundRef->getPrimitiveObject(context);
                unordered_map<string, Value>::iterator primitiveEntry = primitiveObject.find(accessor);

                if(primitiveEntry != primitiveObject.end()){
                    return primitiveEntry->second;
                } else {
                    unordered_map<string, Value>::iterator membersEntry = boundRef->members.find(accessor);

                    if(membersEntry != boundRef->members.end()){
                        return membersEntry->second;
                    } else {
                        Value value = makeValue();
                        get<unordered_map<string, Value>>(boundRef->primitive)[accessor] = value;
                        return value;
                    };
                };
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "object");
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "object{" + to_string(boundRef->getPrimitiveObject(context).size()) + "}");
            }));
        };
    };
};