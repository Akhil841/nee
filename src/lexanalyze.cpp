#include "lexanalyze.h"

Token::Token()
{
    t = TName::TINIT;
    value = "";
}

Token::~Token()
{
    //no memory is allocated so leave destructor undefined
}

Token::Token(TName t, std::string value)
{
    this->t = t;
    this->value = value;
}

std::vector<Token*> lexical_analyze(std::string & expr)
{
    //remove spaces from the expression (from https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c)
    expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
    //output vector
    std::vector<Token*> out;
    //location of NUMBER's beginning
    int nBeginLoc = -1;
    //all possible operators
    std::string operators = "+-*/";
    //all possible numbers
    std::string numbers = "0123456789";
    //lexical analyze each character
    for (int i = 0; i < expr.length(); i++)
    {
        //numbers can be greater than 1 character
        if (nBeginLoc < 0)
        {
            int bsz = out.size();
            if (expr[i] == '(')
                out.push_back(new Token(TName::PO, "("));
            if (expr[i] == ')')
                out.push_back(new Token(TName::PC, ")"));
            for (int j = 0; j < operators.length(); j++)
            {
                if (expr[i] == operators[j])
                {
                    out.push_back(new Token(TName::OPERATOR, std::string(1, operators[j])));
                    break;
                }
            }
            for (int j = 0; j < numbers.length(); j++)
            {
                if (expr[i] == numbers[j])
                {
                    nBeginLoc = i;
                    break;
                }
            }
            if (!(bsz != out.size() || nBeginLoc != -1))
            {
                std::cerr << "Lexical analyzer: Undefined symbol " << expr[i] << " at column " << i << std::endl;
                throw 1;
            }
        }
        //if we are currently in a number
        else
        {
            bool cont = false;
            int bsz = out.size();
            //if the current character is not a number,
            //then the number has ended
            //so push a number token to the list of vectors as well as the current character
            if (expr[i] == '(')
            {
                out.push_back(new Token(TName::NUMBER, expr.substr(nBeginLoc, i - nBeginLoc)));
                out.push_back(new Token(TName::PO, "("));
                nBeginLoc = -1;
            }
            if (expr[i] == ')')
            {
                out.push_back(new Token(TName::NUMBER, expr.substr(nBeginLoc, i - nBeginLoc)));
                out.push_back(new Token(TName::PC, ")"));
                nBeginLoc = -1;
            }
            for (int j = 0; j < operators.length(); j++)
            {
                if (expr[i] == operators[j])
                {
                    out.push_back(new Token(TName::NUMBER, expr.substr(nBeginLoc, i - nBeginLoc)));
                    out.push_back(new Token(TName::OPERATOR, std::string(1, operators[j])));
                    nBeginLoc = -1;
                    break;
                }
            }
            for (int j = 0; j < numbers.length(); j++)
            {
                if (expr[i] == numbers[j])
                {
                    cont = true;
                    break;
                }
            }
            if (!(bsz != out.size() || cont))
            {
                std::cerr << "Lexical analyzer: Undefined symbol " << expr[i] << " at column " << i << std::endl;
                throw 1;
            }
        }
    }
    if (nBeginLoc > -1)
        out.push_back(new Token(TName::NUMBER, expr.substr(nBeginLoc, expr.length() - nBeginLoc)));
    //return the tokens
    return out;
}