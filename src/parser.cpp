#include "parser.h"
#include <stack>
S::S()
{
    s1 = nullptr;
    op = O::OINIT;
    s2 = nullptr;
    n = 0;
    type = Type::TYINIT;
}

S::S(S* s1, O op, S* s2)
{
    this->s1 = s1;
    this->op = op;
    this->s2 = s2;
    type = Type::SOS;
}

S::S(long long n)
{
    this->n = n;
    type = Type::N;
}

S::~S()
{
    if (this->type == Type::SOS)
    {
        delete s1;
        delete s2;
    }
}

O oTokenConv(Token* token)
{
    if (token->t == TName::OPERATOR)
    {
        if (token->value == "+")
            return O::PLUS;
        if (token->value == "-")
            return O::MINUS;
        if (token->value == "*")
            return O::MULTIPLY;
        if (token->value == "/")
            return O::DIVIDE;
        return O::OINIT;
    }
    std::cerr << "Parser: Syntax error: Cannot convert non-operator token " << token->value 
    << " to an operator" << std::endl;
    throw 1;
}

std::string otostr(O o)
{
    if (o == O::PLUS)
        return "+";
    if (o == O::MINUS)
        return "-";
    if (o == O::MULTIPLY)
        return "*";
    if (o == O::DIVIDE)
        return "/";
    return "OINIT(!!!!)";
}

//checks if outer parentheses are redundant (adapted from https://www.geeksforgeeks.org/expression-contains-redundant-bracket-not/)
bool checkRedundancy(std::vector<Token*> tokens)
{
    int cnt = 0;
    // count number of paren tokens
    for (auto it = tokens.begin(); it != tokens.end(); it++)
        if ((*it)->t == TName::PO || (*it)->t == TName::PC)
            cnt++;
    if (cnt == 2 && (*(tokens.begin()))->t == TName::PO && tokens.back()->t == TName::PC)
        return true;
    // create a stack of characters
    std::stack<Token*> st;
 
    // Iterate through the given expression
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
 
        // if current character is close parenthesis ')'
        if ((*it)->value == ")") {
            Token* top = st.top();
            st.pop();
 
            // If immediate pop have open parenthesis '('
            // duplicate brackets found
            bool flag = true;
 
            while (!st.empty() and top->value != "(") 
            {
 
                // Check for operators in expression
                if (top->value == "+" || top->value == "-" ||
                    top->value == "*" || top->value == "/" || top->value.length() > 1)
                    flag = false;
 
                // Fetch top element of stack
                top = st.top();
                st.pop();
            }
 
            // If operators not found
            if (flag)
                return true;
        }
 
        else
            st.push(*it); // push open parenthesis '(',
                  // operators and operands to stack
    }
    return false;
}

S* parse(std::vector<Token*> tokens)
{
    //remove outer parentheses if they are redundant
    if (checkRedundancy(tokens))
    {
        //remove (
        tokens.erase(tokens.begin());
        //remove )
        tokens.pop_back();
    }
    //if the remaining character is just a number, then return a S* with just that number
    if (tokens.size() == 1 && tokens.back()->t == TName::NUMBER)
    {
        printTokens(tokens);
        return new S(std::stoll(tokens.back()->value));
    }
    //otherwise, we have a syntax error
    if (tokens.size() == 1 && tokens.back()->t != TName::NUMBER)
    {
        std::cerr << "Parser: Syntax error at character " << tokens.back()->value << ": Attempt to apply an operator or parenthesis" 
        << " on an operator" << std::endl;
        throw 1; 
    }
    //check for multiple coinciding operators / operator followed by PC
    //check various syntax errors related to PO and PC
    //number of POs
    int npo = 0;
    //number of PCs
    int npc = 0;
    //check each token
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        //checking (
        if ((*it)->t == TName::PO)
        {
            if ((it + 1) != tokens.end() && (*(it + 1))->t != TName::NUMBER && (*(it + 1))->t != TName::PO)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Non-number immediately following "
                << "opening parenthesis" << std::endl;
                throw 1;
            }
            if (it != tokens.begin() && (*(it - 1))->t != TName::OPERATOR && (*(it - 1))->t != TName::PO)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Non-operator immediately preceding "
                << "opening parenthesis" << std::endl;
                throw 1;
            }
            npo++;
        }
        //checking )
        if ((*it)->t == TName::PC)
        {
            if ((it + 1) != tokens.end() && (*(it + 1))->t != TName::OPERATOR && (*(it + 1))->t != TName::PC)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Non-operator immediately following "
                << "closing parenthesis" << std::endl;
                throw 1;
            }
            if (it != tokens.begin() && (*(it - 1))->t != TName::NUMBER && (*(it - 1))->t != TName::PC)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Non-number immediately preceding "
                << "closing parenthesis" << std::endl;
                throw 1;
            }
            npc++;
        }
        //checking operator
        if ((*it)->t == TName::OPERATOR)
        {
            if ((it + 1) != tokens.end() && (*(it + 1))->t == TName::OPERATOR)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Operator following "
                << "an operator" << std::endl;
                throw 1;
            }
            if ((it + 1) != tokens.end() && (*(it + 1))->t == TName::PC)
            {
                std::cerr << "Parser: Syntax error at character " << (*it)->value << ": Closing parenthesis following "
                << "an operator" << std::endl;
                throw 1;
            }
        }
    }
    //check for #PO != #PC
    if (npo != npc)
    {
        std::cerr << "Parser: Syntax error: Number of opening parentheses does not match number of closing parentheses"
        << std::endl;
    }
    //check for correct size of no-parentheses expression
    int parenLevel = 0;
    std::vector<int> plevels;
    //if parenLevel was ever modified
    bool mod = false;
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if ((*it)->t == TName::PO)
        {
            mod = true;
            parenLevel++;
            plevels.push_back(parenLevel);
        }
        if ((*it)->t == TName::PC)
        {
            mod = true;
            plevels.push_back(parenLevel);
            parenLevel--;
        }
        else
            plevels.push_back(0);
    }
    //if not modded, tokens must have 3 members
    if (!mod && tokens.size() != 3)
    {
        std::cerr << "Parser: Syntax error: Ambiguous expression" << std::endl;
        throw 1;
    }
    //if tokens has 3 members, return S*=SOS(parse(token0),operator(token1),parse(token2))
    if (!mod)
    {
        std::vector<Token*> t0;
        t0.push_back(tokens[0]);
        std::vector<Token*> t1;
        t1.push_back(tokens[1]);
        std::vector<Token*> t2;
        t2.push_back(tokens[2]);
        std::cout << "middle" << std::endl;
        printTokens(t0);
        printTokens(t1);
        printTokens(t2);
        return new S(parse(t0), oTokenConv(tokens[1]), parse(t2));
    }
    //find middle operator
    //find first 1-level PO and first 1-level PC
    //the middle operator is after the first 1-level PC

    //first 1-level open paren found
    bool ff = false;
    std::vector<int>::iterator moloc = plevels.end();
    int moloci = 0;
    for (auto it = plevels.begin(); it != plevels.end(); it++)
    {
        //std::cout << *it << std::endl;
        if ((*it) == 1 && !ff)
        {
            ff = true;
            continue;
        }
        if ((*it) == 1 && ff)
        {
            moloci++;
            moloc = it + 1;
            break;
        }
        moloci++;
    }
    //if middle operator is not found, return S*=parse(tokens) to remove another layer of parentheses
    if (moloc == plevels.end())
    {
        printTokens(tokens);
        return parse(tokens);
    }
    //check lvalue tokens exist
    //check rvalue tokens exist
    if (moloc == plevels.begin() || moloc == plevels.end() - 1)
    {
        std::cerr << "Parser: Syntax error: Operator only provided one argument" << std::endl;
    }
    //create new S*=SOS with S(parse(ltokens), middle operator, parse(rtokens))
    //get ltokens vector
    std::vector<Token*> ltokens;
    for (int i = 0; i < moloci; i++)
    {
        ltokens.push_back(tokens[i]);
    }
    //get rtokens vector
    std::vector<Token*> rtokens;
    for (int i = moloci + 1; i < tokens.size(); i++)
    {
        rtokens.push_back(tokens[i]);
    }
    std::vector<Token*> otoken;
    otoken.push_back(tokens[moloci]);
    std::cout << "end" << std::endl;
    printTokens(ltokens);
    printTokens(otoken);
    printTokens(rtokens);
    return new S(parse(ltokens), oTokenConv(tokens[moloci]), parse(rtokens));
}

void printTokens(std::vector<Token*> tokens)
{
    for (std::vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::string tokenName;
        tokenName = ((*it)->t == PO) ? "openParen" : (((*it)->t == PC) ? "closeParen" : (((*it)->t == OPERATOR) ? "operator" : (((*it)->t == NUMBER) ? "number" : "init(!!!!)")));
        if (it + 1 != tokens.end())
        {
            std::cout << "(" << tokenName << ", '" << (*it)->value << "'), ";
        }
        else
        {
            std::cout << "(" << tokenName << ", '" << (*it)->value << "')" << std::endl;
        }
    }
}

void printParseTree(S* tree)
{
    std::cout << "S(";
    if (tree->type == S::Type::SOS)
    {
        printParseTree(tree->s1);
        std::cout << ",O(" << otostr(tree->op) << "),";
        printParseTree(tree->s2);
    }
    if (tree->type == S::Type::N)
        std::cout << "N(" << tree->n << ")";
    std::cout << ")";
}