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
    std::vector<std::string> fullProgram;
    std::string line;
    std::string commands;
    int programSize;
    int initialPosition = 1000;
    int currentPosition = 0;
    int APValue = 999;
    int PCPosition = initialPosition;

    inputFiles.resize(argc - 1);
    for (int i = 0; i < argc - 1; i++) {
        inputFiles[i].open(argv[i + 1]);
        if (!inputFiles[i]) {
            std::cout << "Error: not possible to open file" << std::endl;
            return 2;
        } else {
            inputFiles[i].clear();
            inputFiles[i].seekg(0, std::ios::beg);

            std::getline(inputFiles[i], line);
            if (!line.compare("MV-EXE\n")) {
                std::cout << "Error: wrong file format, missing MV-EXE" << std::endl;
                return 3;
            }

            if (std::getline(inputFiles[i], line) && line.size()) {
                programSize = lgd::processMetaparameters(line);
            } else {
                std::cout << "Error: wrong file format, missing metaparameters line" << std::endl;
                return 4;
            }

            if (std::getline(inputFiles[i], line) && line.size()) {
                lgd::appendToProgram(line, fullProgram, currentPosition);
            } else {
                std::cout << "Error: wrong file format, missing commands line" << std::endl;
                return 5;
            }

            std::getline(inputFiles[i], line);
            std::getline(inputFiles[i], line);
            if (!line.compare("SYMB-TABLE\n")) {
                std::cout << "Error: wrong file format, missing SYMB-TABLE" << std::endl;
                return 6;
            }

            while (std::getline(inputFiles[i], line) && line.size()) {
                lgd::processSymbolTable(line, symbolTable, initialPosition, currentPosition);
            }

            currentPosition += programSize;
        }
        inputFiles[i].close();
    }

    // for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
    //     // std::cout << it->first << " " << it->second << std::endl;
    // }

    // for (int i = 0; i < argc-1; i++) {
    //     if (std::getline(inputFiles[i], line)) {
    //         commands += lgd::processCommands(line, symbolTable, PCPosition);
    //     } else {
    //         std::cout << "Error: wrong file format, missing commands line" << std::endl;
    //         return 6;
    //     }
    //     inputFiles[i].close();
    // }

    auto it = symbolTable.find("main");
    if (it == symbolTable.end()) {
        std::cout << "Error: no label main found" << std::endl;
        return 7;
    } else {
        PCPosition = it->second;
    }

    std::cout << "\nMV-EXE" << std::endl;
    std::cout << currentPosition << " " <<
                initialPosition << " " <<
                APValue << " " <<
                PCPosition << std::endl;

    std::cout << lgd::processFullProgram(fullProgram, symbolTable) << std::endl;

    return 0;
}
