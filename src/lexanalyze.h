#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#pragma once

enum TName
{
    //open parenthesis
    PO,
    //number
    NUMBER,
    //operator
    OPERATOR,
    //close parenthesis
    PC,
    //initial token name
    TINIT
};

class Token
{
    public:
        Token();
        ~Token();
        Token(TName t, std::string value);
        TName t;
        std::string value;
};

std::vector<Token*> lexical_analyze(std::string & expr);