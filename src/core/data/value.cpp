#include <iostream>

#include "value.hpp"
#include "../interpreter/execute.hpp"

namespace Platinum
{
    namespace Data
    {
        NativeFunction::NativeFunction(){};
        NativeFunction::NativeFunction(function<Value(shared_ptr<Interpreter::Context> context, vector<Value> args, shared_ptr<Prototype> boundRef)> implementation)
        {
            this->implementation = implementation;
        };

        DynamicFunction::DynamicFunction(){};
        DynamicFunction::DynamicFunction(shared_ptr<Interpreter::Context> declarationContext, vector<string> argNames, AST::ParsedCode implementation)
        {
            this->declarationContext = declarationContext;
            this->argNames = argNames;
            this->implementation = implementation;
        };

        Function::Function()
        {
            this->boundRef = nullptr;
        };
        Function::Function(shared_ptr<Prototype> boundRef, NativeFunction definition)
        {
            this->boundRef = boundRef;
            this->definition = definition;
        };
        Function::Function(shared_ptr<Prototype> boundRef, DynamicFunction definition)
        {
            this->boundRef = boundRef;
            this->definition = definition;
        };

        bool Function::isNativeFunction()
        {
            return this->definition.index() == 0;
        };

        Value Function::operator()(shared_ptr<Interpreter::Context> context)
        {
            if(this->isNativeFunction()){
                return get<NativeFunction>(this->definition).implementation(context, vector<Value>(), this->boundRef);
            } else {
                DynamicFunction functionDefinition = get<DynamicFunction>(this->definition);

                shared_ptr<Interpreter::Context> executionContext = functionDefinition.declarationContext->createChild("EXECUTION", functionDefinition.implementation[0].sequence[0].sequence.set[0].line, functionDefinition.implementation[0].sequence[0].sequence.set[0].pos);

                executionContext->overwriteVariable("this", this->boundRef);

                return Interpreter::executeOnContext(executionContext, functionDefinition.implementation).returnValue;
            };
        };

        Value Function::operator()(shared_ptr<Interpreter::Context> context, Value arg)
        {
            if(this->isNativeFunction()){
                return get<NativeFunction>(this->definition).implementation(context, vector<Value>{ arg }, this->boundRef);
            } else {
                DynamicFunction functionDefinition = get<DynamicFunction>(this->definition);

                shared_ptr<Interpreter::Context> executionContext = functionDefinition.declarationContext->createChild("EXECUTION", functionDefinition.implementation[0].sequence[0].sequence.set[0].line, functionDefinition.implementation[0].sequence[0].sequence.set[0].pos);

                executionContext->overwriteVariable("this", this->boundRef);

                if(functionDefinition.argNames.size() >= 1){
                    executionContext->overwriteVariable(functionDefinition.argNames[0], arg);
                };

                return Interpreter::executeOnContext(executionContext, functionDefinition.implementation).returnValue;
            };
        };

        Value Function::operator()(shared_ptr<Interpreter::Context> context, vector<Value> args)
        {
            if(this->isNativeFunction()){
                return get<NativeFunction>(this->definition).implementation(context, args, this->boundRef);
            } else {
                DynamicFunction functionDefinition = get<DynamicFunction>(this->definition);

                if(functionDefinition.implementation.size() == 0){
                    return makeValue();
                };

                shared_ptr<Interpreter::Context> executionContext = functionDefinition.declarationContext->createChild("EXECUTION", functionDefinition.implementation[0].sequence[0].sequence.set[0].line, functionDefinition.implementation[0].sequence[0].sequence.set[0].pos);

                executionContext->overwriteVariable("this", this->boundRef);

                for(int i = 0;i<args.size();i++)
                {
                    if(i < functionDefinition.argNames.size()){
                        executionContext->overwriteVariable(functionDefinition.argNames[i], args[i]);
                    };
                };

                return Interpreter::executeOnContext(executionContext, functionDefinition.implementation).returnValue;
            };
        };

        string Function::toString()
        {
            return this->isNativeFunction() ? "{{ Native Function }}" : "{{ Dynamic Function }}";
        };

        Prototype::Prototype()
        {
            this->primitive = monostate();
        };
        Prototype::Prototype(bool primitiveBoolean)
        {
            this->primitive = primitiveBoolean;
        };
        Prototype::Prototype(float primitiveNumber)
        {
            this->primitive = primitiveNumber;
        };
        Prototype::Prototype(string primitiveString)
        {
            this->primitive = primitiveString;
        };
        Prototype::Prototype(vector<Value> primitiveArray)
        {
            this->primitive = primitiveArray;
        };
        Prototype::Prototype(unordered_map<string, Value> primitiveObject)
        {
            this->primitive = primitiveObject;
        };
        Prototype::Prototype(Function primitiveFunction)
        {
            this->primitive = primitiveFunction;
        };

        Function noDefinition(Value ref, string operatorName)
        {
            return Function(ref, NativeFunction([operatorName](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                context->throwError(Interpreter::Context::ErrorTypes::NO_OP_DEF, "Internal " + operatorName + " operator has no definition for type " + boundRef->implicitType(context)->getPrimitiveString(context));
            }));
        };

        void Prototype::constructDefault(Value ref)
        {
            this->unaryNot = noDefinition(ref, "unary not");
            this->unaryMinus = noDefinition(ref, "unary minus");
            this->unaryCopy = noDefinition(ref, "unary copy");

            this->binaryEquals = noDefinition(ref, "binary equals");
            this->binaryNotEquals = noDefinition(ref, "binary not equals");
            this->binaryLess = noDefinition(ref, "binary less");
            this->binaryGreater = noDefinition(ref, "binary greater");
            this->binaryLessOrEquals = noDefinition(ref, "binary less or equals");
            this->binaryGreaterOrEquals = noDefinition(ref, "binary greater or equals");

            this->binaryAdd = noDefinition(ref, "binary add");
            this->binarySubtract = noDefinition(ref, "binary subtract");
            this->binaryMultiply = noDefinition(ref, "binary multiply");
            this->binaryDivide = noDefinition(ref, "binary divide");
            this->binaryModulo = noDefinition(ref, "binary modulo");
            this->binaryAnd = noDefinition(ref, "binary and");
            this->binaryOr = noDefinition(ref, "binary or");

            this->mutativeBinaryAdd = noDefinition(ref, "mutative binary add");
            this->mutativeBinarySubtract = noDefinition(ref, "mutative binary subtract");
            this->mutativeBinaryMultiply = noDefinition(ref, "mutative binary multiply");
            this->mutativeBinaryDivide = noDefinition(ref, "mutative binary divide");
            this->mutativeBinaryModulo = noDefinition(ref, "mutative binary modulo");
            this->mutativeBinaryAnd = noDefinition(ref, "mutative binary and");
            this->mutativeBinaryOr = noDefinition(ref, "mutative binary or");

            this->mutativeIncrement = noDefinition(ref, "mutative increment");
            this->mutativeDecrement = noDefinition(ref, "mutative decrement");

            this->access = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                string accessor = args[0]->getPrimitiveString(context);
                
                unordered_map<string, Value>::iterator entry = boundRef->members.find(accessor);

                if(entry != boundRef->members.end()){
                    return entry->second;
                } else {
                    Value value = makeValue();
                    boundRef->members.insert_or_assign(accessor, value);
                    return value;
                };
            }));
            this->call = noDefinition(ref, "call");
            
            this->implicitType = Function(ref, NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef) -> Value {
                context->throwError(Interpreter::Context::ErrorTypes::NO_OP_DEF, "Internal implicit type operator has no definition");
            }));
            this->implicitToString = noDefinition(ref, "implicit to string");
        };

        void Prototype::construct(Value ref)
        {
            this->constructDefault(ref);
            
            switch(this->primitive.index())
            {
                case BaseTypes::VOID:
                    this->constructVoid(ref);
                    break;
                case BaseTypes::BOOLEAN:
                    this->constructBoolean(ref);
                    break;
                case BaseTypes::NUMBER:
                    this->constructNumber(ref);
                    break;
                case BaseTypes::STRING:
                    this->constructString(ref);
                    break;
                case BaseTypes::ARRAY:
                    this->constructArray(ref);
                    break;
                case BaseTypes::OBJECT:
                    this->constructObject(ref);
                    break;
                case BaseTypes::FUNCTION:
                    this->constructFunction(ref);
                    break;
            };
        };

        bool Prototype::isVoid()
        {
            return this->primitive.index() == BaseTypes::VOID;
        };
        bool Prototype::isBoolean()
        {
            return this->primitive.index() == BaseTypes::BOOLEAN;
        };
        bool Prototype::isNumber()
        {
            return this->primitive.index() == BaseTypes::NUMBER;
        };
        bool Prototype::isString()
        {
            return this->primitive.index() == BaseTypes::STRING;
        };
        bool Prototype::isArray()
        {
            return this->primitive.index() == BaseTypes::ARRAY;
        };
        bool Prototype::isObject()
        {
            return this->primitive.index() == BaseTypes::OBJECT;
        };
        bool Prototype::isFunction()
        {
            return this->primitive.index() == BaseTypes::FUNCTION;
        };

        bool Prototype::getPrimitiveBoolean(shared_ptr<Interpreter::Context> context)
        {
            if(this->isBoolean()){
                return get<bool>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type boolean");
            };
        };
        float Prototype::getPrimitiveNumber(shared_ptr<Interpreter::Context> context)
        {
            if(this->isNumber()){
                return get<float>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type number");
            };
        };
        string Prototype::getPrimitiveString(shared_ptr<Interpreter::Context> context)
        {
            if(this->isString()){
                return get<string>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type string");
            };
        };
        vector<Value> Prototype::getPrimitiveArray(shared_ptr<Interpreter::Context> context)
        {
            if(this->isArray()){
                return get<vector<Value>>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type array");
            };
        };
        unordered_map<string, Value> Prototype::getPrimitiveObject(shared_ptr<Interpreter::Context> context)
        {
            if(this->isObject()){
                return get<unordered_map<string, Value>>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type object");
            };
        };
        Function Prototype::getPrimitiveFunction(shared_ptr<Interpreter::Context> context)
        {
            if(this->isFunction()){
                return get<Function>(this->primitive);
            } else {
                context->throwError(Interpreter::Context::ErrorTypes::BASE_TYPE, "Expected a value with primitive type function");
            };
        };

        Value makeValue()
        {
            Prototype* p = new Prototype();
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(bool primitiveBoolean)
        {
            Prototype* p = new Prototype(primitiveBoolean);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(float primitiveNumber)
        {
            Prototype* p = new Prototype(primitiveNumber);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(string primitiveString)
        {
            Prototype* p = new Prototype(primitiveString);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(vector<Value> primitiveArray)
        {
            Prototype* p = new Prototype(primitiveArray);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(unordered_map<string, Value> primitiveObject)
        {
            Prototype* p = new Prototype(primitiveObject);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
        Value makeValue(Function primitiveFunction)
        {
            Prototype* p = new Prototype(primitiveFunction);
            Value ref = shared_ptr<Prototype>(p);
            ref->construct(ref);
            return ref;
        };
    };
};