#include "lexanalyze.h"
#pragma once

enum O
{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OINIT
};

class S
{
    public:
        S(S* s1, O op, S* s2);
        S(long long n);
        S();
        ~S();
        S* s1;
        O op;
        S* s2;
        long long n;
        enum Type
        {
            SOS,
            N,
            TYINIT
        };
        Type type;
};

S* parse(std::vector<Token*> tokens);

O oTokenConv(Token* token);

std::string otostr(O o);

void printTokens(std::vector<Token*> tokens);

void printParseTree(S* tree);

bool checkRedundancy(std::vector<Token*> tokens);