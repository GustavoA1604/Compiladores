#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "montador.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Please provide the input file" << std::endl;
        return 1;
    }

    std::ifstream file;

    file.open(argv[1]);
    if (!file) {
        std::cout << "Error: not possible to open file" << std::endl;
        return 2;
    } else {
        std::string line;
        std::string commands;
        std::unordered_map<std::string, int> symbolTable;
        int initialPosition = 1000;
        int currentPosition;
        int APValue = 999;
        int PCPosition = initialPosition;

        file.clear();
        file.seekg(0, std::ios::beg);
        currentPosition = initialPosition;

        while (std::getline(file, line)) {
            mtd::processSymbolTable(line, symbolTable, currentPosition, PCPosition);
        }
        std::cout << "MV-EXE" << std::endl;
        std::cout << currentPosition - initialPosition << " " << // TODO: check why +1
                     initialPosition << " " <<
                     APValue << " " <<
                     PCPosition << std::endl;

        file.clear();
        file.seekg(0, std::ios::beg);

        while (std::getline(file, line)) {
            commands += mtd::processLine(line, symbolTable, PCPosition);
        }
        // removing an extra space at the end
        commands.pop_back();
        std::cout << commands << std::endl;

        std::cout << "\nSYMB-TABLE" << std::endl;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
            std::cout << it->first << " " << it->second << std::endl;
        }
    }
    return 0;
}