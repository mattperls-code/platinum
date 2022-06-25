#include <iostream>

#include "../core/interpreter/standardLibrary.hpp"

namespace Platinum
{
    namespace Modules
    {
        Data::Value io()
        {
            unordered_map<string, Data::Value> components;
            
            components["log"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                string concat;

                for(int i = 0;i<args.size() - 1;i++)
                {
                    concat += args[i]->implicitToString(context)->getPrimitiveString(context) + "\n\n";
                };
                if(args.size() >= 1){
                    concat += args.back()->implicitToString(context)->getPrimitiveString(context);
                };

                context->log(concat);

                return Data::makeValue();
            })));

            components["input"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                if(args.size() != 2){
                    context->throwError(Interpreter::Context::ErrorTypes::CONTEXTUAL, "Expected exactly 2 args. input(prompt: string, callback: function)");
                };

                context->log(args[0]->getPrimitiveString(context));

                string inputString;

                cin >> inputString;

                cout << endl;

                args[1]->getPrimitiveFunction(context)(context, Data::makeValue(inputString));

                return Data::makeValue();
            })));

            return Data::makeValue(components);
        };
    };
};