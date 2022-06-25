#include <iostream>
#include <fstream>
#include <iomanip>

#include "evaluate.hpp"
#include "execute.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        void execute(vector<string> args)
        {
            executeFile(args[1]);
        };

        void executeFile(string path)
        {
            return executeFile(path, make_shared<Context>("ROOT", 0, 0));
        };

        void executeFile(string path, shared_ptr<Context> context)
        {
            ifstream runTarget(path);
            if(runTarget.fail()){
                context->throwError(Context::ErrorTypes::FILE, "File \"" + path + "\" was unable to be read");
            };
            string untrackedCode;
            string line;
            while(getline(runTarget, line))
            {
                untrackedCode += line + '\n';
            };
            runTarget.close();
            executeOnContext(context, AST::TrackedCode(untrackedCode));

            cout << "\033[36m" << "Execution Time: " << fixed << setprecision(8) << context->getExecutionTime() << " seconds" << endl;
        };

        StatementResolution executeOnContext(shared_ptr<Context> context, AST::ParsedCode blocks)
        {
            AST::ParsedCode currentBlockChain;
            for(int i = 0;i<blocks.size();i++)
            {
                currentBlockChain.push_back(blocks[i]);
                if(!blocks[i].isChaining){
                    StatementResolution resolvedStatement;
                    if(currentBlockChain.size() == 1){
                        string blockId = currentBlockChain[0].id;
                        if(blockId == "for"){
                            resolvedStatement = Statements::forStatement(context, currentBlockChain[0]);
                        } else if(blockId == "while"){
                            resolvedStatement = Statements::whileStatement(context, currentBlockChain[0]);
                        } else if(blockId == "return"){
                            resolvedStatement = Statements::returnStatement(context, currentBlockChain[0]);
                        } else if(blockId == "mutate"){
                            resolvedStatement = Statements::mutateStatement(context, currentBlockChain[0]);
                        } else if(blockId == "evaluate"){
                            resolvedStatement = Statements::evaluateStatement(context, currentBlockChain[0]);
                        } else if(blockId == "if"){
                            resolvedStatement = Statements::ifStatement(context, currentBlockChain[0]);
                        };
                    } else {
                        resolvedStatement = Statements::conditionalChainStatement(context, currentBlockChain);
                    };
                    currentBlockChain.clear();
                    
                    if(resolvedStatement.didReturn){
                        return resolvedStatement;
                    };
                };
            };

            return StatementResolution();
        };

        StatementResolution executeOnContext(shared_ptr<Context> context, AST::TrackedCode code)
        {
            return executeOnContext(context, AST::Parser::createExecutionBlocks(context, code));
        };
    };
};