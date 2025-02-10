#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "lexer.cpp"

struct ASTNode
{
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;

};
struct NumberNode : ASTNode
{
    double value;
    NumberNode(double val) : value(val) {};

    double evaluate() const override {return value;}
};
struct BinaryOpNode : ASTNode {
    ASTNode* left;
    ASTNode* right;


    std::string op;

    BinaryOpNode(ASTNode* l, std::string oper, ASTNode* r) : left(std::move(l)), op(oper), right(std::move(r)) {}

    double evaluate() const override
    {
        double leftVal = left -> evaluate();
        double rightVal = right -> evaluate();

        if(op == "+")
            return leftVal + rightVal;
        if(op == "-")
            return leftVal - rightVal;
        if(op == "*")
            return leftVal * rightVal;
        if(op == "/")
            return leftVal / rightVal;
        throw std::runtime_error("unknown binary operator: " + op);
    }
};
class Parser
{
    public:
    std::vector<Token> tokens;
    Parser(std::vector<Token> t) : tokens(std::move(t)) {}

    ASTNode* parseExpression()
    {
        ASTNode* left = parseTerm();

        while(tokens.at(0).type == TokenType::OPERATOR && (tokens.at(0).str == "+" || tokens.at(0).str == "-"))
        {
            std::string op = tokens.at(0).str;
            tokens.erase(tokens.begin());
            ASTNode* right = parseTerm();
            left = new BinaryOpNode(left, op, right);
        }
        return left;
    }
    ASTNode* parseTerm()
    {
        ASTNode* left = parseFactor();

        while(tokens.at(0).type == TokenType::OPERATOR && (tokens.at(0).str == "*" || tokens.at(0).str == "/"))
        {
            std::string op = tokens.at(0).str;
            tokens.erase(tokens.begin());
            ASTNode* right = parseFactor();
            left = new BinaryOpNode(left, op, right);
        }
        return left;
    }
    ASTNode* parseFactor()
    {   
        if(tokens.at(0).type == TokenType::NUM)
        {
            double val = std::stod(tokens.at(0).str);
            tokens.erase(tokens.begin());
            return new NumberNode(val);
        }
        else if(tokens.at(0).type == TokenType::OPERATOR && tokens.at(0).str == "(")
        {
            tokens.erase(tokens.begin());
            ASTNode* expression = parseExpression();
            if(tokens.at(0).type != TokenType::OPERATOR || tokens.at(0).str != ")")
                throw std::runtime_error("expected ')'");
            tokens.erase(tokens.begin());
            return expression;
        }
        else
        {
            throw std::runtime_error("expected number or '('");
        }
    }
};
int main()
{
    std::string code = "(2 * 1) + 2";
    Lexer lexer(code);
    Parser parser = Parser(lexer.tokenize());
    ASTNode* ast = parser.parseExpression();
    std::cout << ast -> evaluate() << std::endl;
    std::vector<Token> tokens = lexer.tokenize();
    for(int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].str << std::endl;
    }
    return 0;
}