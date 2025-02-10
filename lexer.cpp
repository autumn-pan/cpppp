#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

enum class TokenType {
    IDENTIFIER,
    NUM,
    OPERATOR,
    EOL,
    KEYWORD,
    END_OF_FILE,
    UNKNOWN
};


struct Token
{
    TokenType type;
    std::string str;
    int line,column;

};

class Lexer
{
    private:
        std::string sourceCode;
        size_t pos;
        int line,column;
        std::unordered_set<std::string>  keywords = {
            "if",
            "else",
            "return"
        };
        std::unordered_set<char>  operators = {
            '+',
            '-',
            '*',
            '/',
            '(',
            ')'
        };

    char checkPos()
    {
        if(pos < sourceCode.size())
            return sourceCode[pos];
        else
            return '\0';
    }

    void next() {
        char c = checkPos();
        pos++;
        column++;

        if(c == ';')
        {
            line++;
            column = 0;
        }
    }
    public:
    Lexer(const std::string str) : sourceCode(str), pos(0), line(1), column(1) {}

    void skipWhitespace() {
        while(std::isspace(checkPos()))
            next();
    }

    Token lexNumber()
    {
        std::string num;
        while(std::isdigit(checkPos()))
        {
            num += (checkPos());
            next();

        }
    return {TokenType::NUM, num, line, column};
    }

    Token lexIdentifier()
    {
        std::string str;
        while(std::isalnum(checkPos())||checkPos() == '_')
        {
            str += checkPos();
            next();
        }
            return {keywords.count(str) ? TokenType::KEYWORD : TokenType::IDENTIFIER, str, line, column};
    }

    Token lexOperator()
    {
        return {TokenType::OPERATOR,std::string(1,checkPos()), line, column};

    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < sourceCode.size())
        {
            skipWhitespace();
            char c = checkPos();
            if(std::isdigit(c))
                tokens.push_back(lexNumber());
            else if (std::isalnum(c))
                tokens.push_back(lexIdentifier());
            else if(operators.count(c) == 1)
            {
                tokens.push_back(lexOperator());
                next();
            }
            else
                next();
            
        }
        tokens.push_back({TokenType::END_OF_FILE, "", line, column});
        return tokens;
    }


};
