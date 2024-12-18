#include <fstream>
#include <iostream>

#include "tokenize.h"
#include "parser.h"

int main()
{
    /*
var proje = (1 + 2) * 3;
while (true) {
    proje = proje + 1;
    break;
}
return 0;
     */
    const std::string sourceCode = "var proje = (1 + 2) * 3; while (true) { proje = proje + 1; break; } return 0;";
    // tokenize the source code
    auto tokens = tokenize(sourceCode);
    std::cout << "Input source: " + sourceCode << std::endl;


    for (const auto& [value, type, position] : tokens) {
        std::cout << "Token: " << value
                  << " | Type: " << tokenTypeToString(type)
                  << " | Position: " << position
                  << std::endl;
    }

    Parser parser(tokens);
    auto ast = parser.parse();

    return 0;
}


/*
const int NUM_ITERATIONS = 1000000;
Benchmarking Raw Pointer, Unique Pointer, and Shared Pointer
Raw Pointer Time: 0.0355525 seconds
Unique Pointer Time: 0.0790833 seconds
Shared Pointer Time: 0.133362 seconds
 */