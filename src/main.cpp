#include <ifstream>
#include "parser.h"
#include "evaluate.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./nee [filename], where [filename] is a directory or file path to your query." << std::endl;
        return 1;
    }
    std::ifstream reader(std::string(*(argv + 1)));
    std::string input = "";
    if (reader)
    {
        std::getline(reader, input);
        reader.close();
    }
    else
    {
        std::cerr << "Main: Input file or directory " << std::string(*(argv + 1)) << " is bad or does not exist" << std::endl;
        return 1;
    }
    std::cout << input << std::endl;
    //lexically analyze the input string
    std::vector<Token*> tokens = lexical_analyze(input);
    printTokens(tokens);
    //parse the tokens
    S* tree = parse(tokens);
    //delete the tokens
    for (std::vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); it++)
        delete *it;
    printParseTree(tree);
    std::cout << std::endl;
    //evaluate the parse tree
    long long output = eval(tree);
    std::cout << output << std::endl;
    //delete the parse tree
    delete tree;
    return 0;
}