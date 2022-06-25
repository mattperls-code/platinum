#include <iostream>
#include <fstream>
#include <iomanip>

#include "context.hpp"
#include "standardLibrary.hpp"

namespace Platinum
{
    namespace Interpreter
    {
        Context::Context(){};

        Context::Context(string id, int line, int pos)
        {
            this->timestamp = chrono::steady_clock::now();
            this->id = id;

            this->line = line;
            this->pos = pos;

            this->hasParent = false;
            this->parent = nullptr;

            this->variables = standardLibrary();
        };

        Context::Context(string id, int line, int pos, shared_ptr<Context> parent)
        {
            this->id = id;

            this->line = line;
            this->pos = pos;

            this->hasParent = true;
            this->parent = parent;
        };

        double Context::getExecutionTime()
        {
            auto now = chrono::steady_clock::now();
            if(this->hasParent){
                return this->parent->getExecutionTime();
            } else {
                return chrono::duration<double>(now - this->timestamp).count();
            };
        };

        string Context::path()
        {
            return this->hasParent ? this->parent->path() + " -> " + this->id : this->id;
        };

        shared_ptr<Context> Context::createChild(string id, int line, int pos)
        {
            return make_shared<Context>(id, line, pos, shared_from_this());
        };

        bool Context::hasVariable(string name)
        {
            if(this->variables.find(name) != this->variables.end()) {
                return true;
            } else if(this->hasParent){
                return this->parent->hasVariable(name);
            } else {
                return false;
            };
        };

        Data::Value Context::getVariable(string name)
        {
            unordered_map<string, Data::Value>::iterator variable = this->variables.find(name);
            if(variable != this->variables.end()){
                return variable->second;
            } else if(this->hasParent && this->parent->hasVariable(name)){
                return this->parent->getVariable(name);
            } else {
                this->variables[name] = Data::makeValue();
                return this->variables[name];
            };
        };

        void Context::overwriteVariable(string name, Data::Value value)
        {
            // TODO
            this->variables[name] = value;
        };

        void Context::merge()
        {
            if(this->hasParent){
                for(auto it : this->variables)
                {
                    this->parent->variables[it.first] = it.second;
                };
            };
        };

        void Context::throwError(int type, string reason)
        {
            string errorCode;
            string errorSender;
            switch(type)
            {
                case Context::ErrorTypes::FILE:
                    errorCode = "File";
                    errorSender = "Parser";
                    break;
                case Context::ErrorTypes::TOKEN:
                    errorCode = "Unterminated Token";
                    errorSender = "Parser";
                    break;
                case Context::ErrorTypes::BLOCK:
                    errorCode = "Unterminated Block";
                    errorSender = "Parser";
                    break;
                case Context::ErrorTypes::BASE_TYPE:
                    errorCode = "Base Type";
                    errorSender = "Interpreter";
                case Context::ErrorTypes::NO_OP_DEF:
                    errorCode = "No Op Def";
                    errorSender = "Interpreter";
                    break;
                case Context::ErrorTypes::CONTEXTUAL:
                    errorCode = "Contextual";
                    errorSender = "Interpreter";
                    break;
            };
            cout << "\033[31m" << errorCode << " Error (" << errorSender << "): @(Line: " << (this->line + 1) << ", Position: " << (this->pos + 1) << ") " << "[ " << this->path() << " ]" << "\033[37m" << endl << reason << "\033[36m" << endl << endl << "Execution Time: " << fixed << setprecision(8) << this->getExecutionTime() << " seconds" << endl;
            exit(0);
        };

        void Context::log(string message)
        {
            cout << "\033[32m" << "Log (Interpreter): " << "@(Line: " << (this->line + 1) << ", Position: " << (this->pos + 1) << ") " << "[ " << this->path() << " ]" << "\033[37m" << endl << message << endl << endl;
        };
    };
};