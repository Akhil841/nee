#include "evaluate.h"

long long eval(S* tree)
{
    if (tree->type == S::Type::N)
        return tree->n;
    else
        return numeval(eval(tree->s1), tree->op, eval(tree->s2));
}

long long numeval(long long larg, O op, long long rarg)
{
    if (op == O::PLUS)
        return larg + rarg;
    if (op == O::MINUS)
        return larg - rarg;
    if (op == O::MULTIPLY)
        return larg * rarg;
    if (op == O::DIVIDE)
    {
        if (rarg)
            return larg / rarg;
        else
        {
            std::cerr << "Evaluator: Cannot divide by 0!" << std::endl;
            throw 1;
        }
    }
    else
    {
        std::cerr << "Holy cow, how did you get here???" << std::endl;
        throw 1;
    }
}