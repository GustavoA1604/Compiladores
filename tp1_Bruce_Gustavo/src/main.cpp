#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "montador.h"

int main(int argc, char *argv[]) {

    std::ifstream file("tst/ex.amv");
    if (!file) {
        std::cout << "Error: not possible to open file" << std::endl;
        return 1;
    } else {
        std::string line;
        std::string commands;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            commands += mtd::processLine(line);
        }
        std::cout << commands;
    }
    return 0;
}
