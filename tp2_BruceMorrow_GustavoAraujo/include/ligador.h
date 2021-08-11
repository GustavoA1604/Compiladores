#ifndef LIGADOR_H
#define LIGADOR_H

#include <string>
#include <unordered_map>
#include <vector>

namespace lgd {

void processSymbolTable(std::string line, std::unordered_map<std::string, int>& symbolTable, int& initialPosition, int& currentPosition);

int processMetaparameters(std::string line);

std::string processCommands(std::string line, std::unordered_map<std::string, int>& symbolTable, int& PCPosition);

void appendToProgram(std::string commandsLine, std::vector<std::string>& fullProgram, int& currentPosition);

bool isNumber(const std::string &s);

std::string processFullProgram(std::vector<std::string>& fullProgram, std::unordered_map<std::string, int>& symbolTable);

}

#endif
