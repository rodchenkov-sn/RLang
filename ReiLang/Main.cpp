#include <iostream>
#include "ReiParser.hpp"

int main(int argc, char* argv[])
{
    std::string expression;
    while (true) {
        try {
            std::getline(std::cin, expression, '$');
            if (expression.empty()) {
                break;
            }
            Parser parser{ expression };
            parser.parse();
            std::cout << "Done.\n";
        }
        catch (const std::exception& exc) {
            std::cout << exc.what() << "\n";
            //return 1;
        }
    }
    return 0;
}
