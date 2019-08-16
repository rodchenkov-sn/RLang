#include <iostream>
#include "ReiInterpreter.hpp"

int main(int argc, char* argv[])
{
    std::string expression;
    while (true) {
        try {
            std::getline(std::cin, expression, '$');
            if (expression.empty()) {
                break;
            }
            Interpreter interpreter{ expression };
            interpreter.interpret();
            std::cout << "done\n";
        }
        catch (const std::exception& exc) {
            std::cout << exc.what() << "\n";
            //return 1;
        }
    }
    return 0;
}
