#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "ligador.h"

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        std::cout << "Please provide input files" << std::endl;
        return 1;
    }

    std::vector<std::ifstream> inputFiles;
    std::unordered_map<std::string, int> symbolTable;
    std::string line;
    std::string commands;
    int initialPosition = 1000;
    int currentPosition = initialPosition;
    int APValue = 999;
    int PCPosition = initialPosition;

    inputFiles.resize(argc-1);
    for (int i = 0; i < argc-1; i++) {
        inputFiles[i].open(argv[i+1]);
        if (!inputFiles[i]) {
            std::cout << "Error: not possible to open file" << std::endl;
            return 2;
        } else {
            inputFiles[i].clear();
            inputFiles[i].seekg(0, std::ios::beg);

            std::getline(inputFiles[i], line);
            if (!line.compare("SYMB-TABLE\n")) {
                std::cout << "Error: wrong file format, missing SYMB-TABLE" << std::endl;
                return 3;
            }

            while (std::getline(inputFiles[i], line) && line.size()) {
                lgd::processSymbolTable(line, symbolTable, initialPosition, currentPosition);
            }
            
            if (!std::getline(inputFiles[i], line)) {
                std::cout << "Error: wrong file format, missing MV-EXE" << std::endl;
                return 4;
            }
            
            if (std::getline(inputFiles[i], line)) {
                lgd::processMetaparameters(line, currentPosition);
            } else {
                std::cout << "Error: wrong file format, missing metaparameters line" << std::endl;
                return 5;
            }
        }
    }

    for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
        // std::cout << it->first << " " << it->second << std::endl;
    }

    for (int i = 0; i < argc-1; i++) {
        if (std::getline(inputFiles[i], line)) {
            commands += lgd::processCommands(line, symbolTable, PCPosition);
        } else {
            std::cout << "Error: wrong file format, missing commands line" << std::endl;
            return 6;
        }
        inputFiles[i].close();
    }

    std::cout << "\nMV-EXE" << std::endl;
    std::cout << currentPosition - initialPosition + 1 << " " <<
                initialPosition << " " <<
                APValue << " " <<
                PCPosition << std::endl;
    
    // removing an extra space at the end
    //commands.pop_back();
    std::cout << commands << std::endl;

    return 0;
}
