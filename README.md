# NEE
NEE (Numerical Expression Evaluator) evaluates any numerical expression that complies with its context-free grammar.

See doc/cfg.md to see its context-free grammar.

## Dependencies
If you're using a mainstream compiler like CLion or g++ or Visual Studio to compile your code, then you don't need any extra dependencies.
If you're using a custom C++ compiler or some other niche compiler, you need `std::iostream`, `std::ifstream`, `std::string`, `std::algorithm`, `std::stdexcept`, `std::vector`, and `std::stack`.

## Compiling NEE
Clone this repository and enter its root.
On Linux/Mac, enter `make`.

On Windows, compile with the C++ compiler of your choice.

## Running NEE

Enter `./nee.o` followed by a file containing your input expression.

For now, the program will print the contents of the file, the lexically analyzed input, the parsed input, and then the output.