#include "tracking.hpp"

namespace Platinum
{
    namespace AST
    {
        TrackedChar::TrackedChar(){};

        TrackedChar::TrackedChar(char c, int line, int pos)
        {
            this->c = c;
            this->line = line;
            this->pos = pos;
        };
        
        TrackedCode::TrackedCode(){};

        TrackedCode::TrackedCode(vector<TrackedChar> trackedCode)
        {
            this->set = trackedCode;
        };

        TrackedCode::TrackedCode(string str)
        {
            int lineCount = 0;
            int charPos = 0;
            for(int i = 0;i<str.size();i++)
            {
                if(str[i] == '\n'){
                    lineCount++;
                    charPos = 0;
                } else {
                    this->set.push_back(TrackedChar(str[i], lineCount, charPos));
                    charPos++;
                };
            };
            bool inString = false;
            bool inMultilineComment = false;
            for(int i = 0;i<this->set.size();i++)
            {
                if(inString){
                    if(this->set[i].c == '\"'){
                        inString = false;
                    };
                } else if(!inMultilineComment && i >= 1 && this->set[i - 1].c == '/' && this->set[i].c == '*'){
                    this->set.erase(this->set.begin() + i);
                    i--;
                    inMultilineComment = true;
                } else if(inMultilineComment && i + 1 < this->set.size() && this->set[i].c == '*' && this->set[i + 1].c == '/'){
                    this->set.erase(this->set.begin() + i + 1);
                    this->set.erase(this->set.begin() + i);
                    i--;
                    inMultilineComment = false;
                } else if(!inMultilineComment && this->set[i].c == '\"'){
                    inString = true;
                };
                if(inMultilineComment){
                    this->set.erase(this->set.begin() + i);
                    i--;
                };
            };
            inString = false;
            bool inSingleLineComment = false;
            int commentLine;
            for(int i = 0;i<set.size();i++)
            {
                if(inString){
                    if(this->set[i].c == '\"'){
                        inString = false;
                    };
                } else if(i >= 1 && !inSingleLineComment && this->set[i - 1].c == '/' && this->set[i].c == '/'){
                    commentLine = this->set[i].line;
                    this->set.erase(this->set.begin() + i);
                    i--;
                    inSingleLineComment = true;
                } else if(!inSingleLineComment && this->set[i].c == '\"'){
                    inString = true;
                } else if(inSingleLineComment && this->set[i].line > commentLine){
                    inSingleLineComment = false;
                };
                if(inSingleLineComment){
                    this->set.erase(this->set.begin() + i);
                    i--;
                };
            };
        };

        string TrackedCode::toString()
        {
            string str;
            for(int i = 0;i<set.size();i++)
            {
                str += this->set[i].c;
            };
            return str;
        };

        TrackedCode TrackedCode::slice(int start, int end)
        {
            TrackedCode sliced;
            for(int i = start;i<end;i++)
            {
                sliced.set.push_back(this->set[i]);
            };
            return sliced;
        };
    };
};