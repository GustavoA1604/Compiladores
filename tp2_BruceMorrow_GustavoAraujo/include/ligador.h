#ifndef LIGADOR_H
#define LIGADOR_H

#include <string>
#include <unordered_map>

namespace lgd {

void processSymbolTable(std::string line, std::unordered_map<std::string, int>& symbolTable, int& initialPosition, int& currentPosition);

void processMetaparameters(std::string line, int& currentPosition);

std::string processCommands(std::string line, std::unordered_map<std::string, int>& symbolTable, int& PCPosition);

}

#endif
