#ifndef PLATINUM_CORE_DATA_VALUE_HPP_
#define PLATINUM_CORE_DATA_VALUE_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <variant>
#include <memory>

#include "../ast/block.hpp"
#include "../interpreter/context.hpp"

using namespace std;

namespace Platinum
{
    namespace AST
    {
        class Block;
        typedef vector<Block> ParsedCode;
    };

    namespace Interpreter
    {
        class Context;
    };

    namespace Data
    {
        class Prototype;
        typedef shared_ptr<Prototype> Value;

        class NativeFunction
        {
            public:
                function<Value(shared_ptr<Interpreter::Context>, vector<Value>, Value boundRef)> implementation;
                
                NativeFunction();
                NativeFunction(function<Value(shared_ptr<Interpreter::Context> context, vector<Value> args, Value boundRef)> implementation);
        };

        class DynamicFunction
        {
            public:
                shared_ptr<Interpreter::Context> declarationContext;
                vector<string> argNames;
                AST::ParsedCode implementation;

                DynamicFunction();
                DynamicFunction(shared_ptr<Interpreter::Context> declarationContext, vector<string> argNames, AST::ParsedCode implementation);
        };
        
        class Function
        {
            private:
                Value boundRef;
                variant<NativeFunction, DynamicFunction> definition;

                bool isNativeFunction();
            public:
                Function();
                Function(Value boundRef, NativeFunction definition);
                Function(Value boundRef, DynamicFunction definition);

                Value operator()(shared_ptr<Interpreter::Context> context);
                Value operator()(shared_ptr<Interpreter::Context> context, Value arg);
                Value operator()(shared_ptr<Interpreter::Context> context, vector<Value> args);

                string toString();
        };

        enum BaseTypes { VOID, BOOLEAN, NUMBER, STRING, ARRAY, OBJECT, FUNCTION };

        class Prototype
        {
            private:
                void constructDefault(Value ref);
                
                void constructVoid(Value ref);
                void constructBoolean(Value ref);
                void constructNumber(Value ref);
                void constructString(Value ref);
                void constructArray(Value ref);
                void constructObject(Value ref);
                void constructFunction(Value ref);
            public:
                variant<monostate, bool, float, string, vector<Value>, unordered_map<string, Value>, Function> primitive;
                unordered_map<string, Value> members;

                Prototype();
                Prototype(bool primitiveBoolean);
                Prototype(float primitiveNumber);
                Prototype(string primitiveString);
                Prototype(vector<Value> primitiveArray);
                Prototype(unordered_map<string, Value> primitiveObject);
                Prototype(Function primitiveFunction);

                Function unaryNot;                  //  !
                Function unaryMinus;                //  -
                Function unaryCopy;                 //  *

                Function binaryEquals;              //  ==
                Function binaryNotEquals;           //  !=
                Function binaryLess;                //  <
                Function binaryGreater;             //  >
                Function binaryLessOrEquals;        //  <=
                Function binaryGreaterOrEquals;     //  >=

                Function binaryAdd;                 //  +
                Function binarySubtract;            //  -
                Function binaryMultiply;            //  *
                Function binaryDivide;              //  /
                Function binaryModulo;              //  %
                Function binaryAnd;                 //  &&
                Function binaryOr;                  //  ||

                Function mutativeBinaryAdd;         //  +=
                Function mutativeBinarySubtract;    //  -=
                Function mutativeBinaryMultiply;    //  *=
                Function mutativeBinaryDivide;      //  /=
                Function mutativeBinaryModulo;      //  %=
                Function mutativeBinaryAnd;         //  &=
                Function mutativeBinaryOr;          //  |=

                Function mutativeIncrement;         //  ++
                Function mutativeDecrement;         //  --

                Function access;                    //  []
                Function call;                      //  ()

                Function implicitType;
                Function implicitToString;

                void construct(Value ref);

                bool isVoid();
                bool isBoolean();
                bool isNumber();
                bool isString();
                bool isArray();
                bool isObject();
                bool isFunction();

                bool getPrimitiveBoolean(shared_ptr<Interpreter::Context> context);
                float getPrimitiveNumber(shared_ptr<Interpreter::Context> context);
                string getPrimitiveString(shared_ptr<Interpreter::Context> context);
                vector<Value> getPrimitiveArray(shared_ptr<Interpreter::Context> context);
                unordered_map<string, Value> getPrimitiveObject(shared_ptr<Interpreter::Context> context);
                Function getPrimitiveFunction(shared_ptr<Interpreter::Context> context);
        };

        Value makeValue();
        Value makeValue(bool primitiveBoolean);
        Value makeValue(float primitiveNumber);
        Value makeValue(string primitiveString);
        Value makeValue(vector<Value> primitiveArray);
        Value makeValue(unordered_map<string, Value> primitiveObject);
        Value makeValue(Function primitiveFunction);
    };
};

#endif