#include <cmath>

#include "../value.hpp"

namespace Platinum
{
    namespace Data
    {
        void Prototype::constructArray(Value ref)
        {
            this->unaryCopy = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue(boundRef->getPrimitiveArray(context));
            }));

            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((string) "array");
            }));

            this->access = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                if(args[0]->isNumber()){
                    float arg = args[0]->getPrimitiveNumber(context);
                    int rounded = roundf(arg);
                    if(fabs(rounded - arg) < 0.00001 && arg >= 0){
                        if(rounded < boundRef->getPrimitiveArray(context).size()){
                            return boundRef->getPrimitiveArray(context)[rounded];
                        } else {
                            context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Attempted out of bounds access of array");
                        };
                    } else {
                        context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Numeric operand applied to access op of array must be a non negative integer");
                    };
                } else if(args[0]->isString()){
                    string accessor = args[0]->getPrimitiveString(context);
                
                    unordered_map<string, Value>::iterator entry = boundRef->members.find(accessor);

                    if(entry != boundRef->members.end()){
                        return entry->second;
                    } else {
                        Value value = makeValue();
                        boundRef->members.insert_or_assign(accessor, value);
                        return value;
                    };
                } else {
                    context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type number or string");
                };
            }));

            this->implicitToString = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                vector<Value> array = boundRef->getPrimitiveArray(context);
                if(array.size() == 0){
                    return makeValue((string) "[]");
                } else if(array.size() == 1){
                    return makeValue("[ " + array[0]->implicitToString(context)->getPrimitiveString(context) + "] ");
                } else {
                    string concat;
                    for(int i = 0;i<array.size() - 1;i++)
                    {
                        concat += array[i]->implicitToString(context)->getPrimitiveString(context) + ", ";
                    };
                    concat += array.back()->implicitToString(context)->getPrimitiveString(context);
                    return makeValue("[ " + concat + " ]");
                };
            }));

            this->members["length"] = makeValue(Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                return makeValue((float) boundRef->getPrimitiveArray(context).size());
            })));

            this->members["pushBack"] = makeValue(Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                for(int i = 0;i<args.size();i++)
                {
                    get<vector<Value>>(boundRef->primitive).push_back(args[i]);
                };
                return makeValue();
            })));

            this->members["popBack"] = makeValue(Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                get<vector<Value>>(boundRef->primitive).pop_back();
                return makeValue();
            })));

            this->members["forEach"] = makeValue(Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                vector<Value> array = boundRef->getPrimitiveArray(context);

                Function callback = args[0]->getPrimitiveFunction(context);
                
                for(int i = 0;i<array.size();i++)
                {
                    callback(context, vector<Value>{ array[i], makeValue((float) i) });
                };

                return makeValue();
            })));
        };
    };
};