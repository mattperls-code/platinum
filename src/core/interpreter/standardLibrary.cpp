#include "standardLibrary.hpp"
#include "context.hpp"

#include "../../modules/plaTypes.hpp"
#include "../../modules/plaIO.hpp"
#include "../../modules/plaMath.hpp"
#include "../../modules/plaFS.hpp"
#include "../../modules/plaNetwork.hpp"
#include "../../modules/plaEval.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        unordered_map<string, Data::Value> standardLibrary()
        {
            unordered_map<string, Data::Value> exposed;

            exposed["outWrite"] = Data::makeValue(Data::Function(Data::makeValue(), Data::NativeFunction([](shared_ptr<Interpreter::Context> context, vector<Data::Value> args, Data::Value boundRef) -> Data::Value {
                for(int i = 0;i<args.size();i++)
                {
                    context->log(args[i]->implicitToString(context)->getPrimitiveString(context));
                };
                return Data::makeValue();
            })));

            return exposed;
        };
    };
};