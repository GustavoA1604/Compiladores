#include <sstream>
#include <iostream>
#include <algorithm>

#include "ligador.h"

namespace lgd {

void processSymbolTable(std::string line,
                        std::unordered_map<std::string, int>& symbolTable,
                        int& initialPosition,
                        int& currentPosition) {
  std::stringstream ss(line);
  std::string symbol;
  int position;
  ss >> symbol;
  ss >> position;
  symbolTable.insert(std::make_pair(symbol, position + currentPosition));
}


int processMetaparameters(std::string line) {
  std::stringstream ss(line);
  std::string first;
  std::getline(ss, first, ' ');
  return std::stoi(first);
}

std::string processCommands(std::string line,
                            std::unordered_map<std::string, int>& symbolTable,
                            int& PCPosition) {

  return "";
}

void appendToProgram(std::string commandsLine, std::vector<std::string>& fullProgram, int& currentPosition) {
  std::stringstream ss(commandsLine);
  std::string command;
  while (std::getline(ss, command, ' ')) {
    if(command.rfind("#", 0) == 0) {
      fullProgram.push_back(command.substr(1));
    } else if (command.find("&") == std::string::npos) {
      fullProgram.push_back(isNumber(command) ? std::to_string(std::stoi(command) + currentPosition) : command);
    } else {
      std::stringstream ssAux(command);
      std::stringstream newCommand;
      std::string commandAux;
      std::getline(ssAux, commandAux, '&');
      newCommand << commandAux << "&";
      std::getline(ssAux, commandAux, '&');
      newCommand << std::to_string(std::stoi(commandAux) + currentPosition);
      fullProgram.push_back(newCommand.str());
    }
  }
}

bool isNumber(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && *it == '-'))) ++it;
  return !s.empty() && it == s.end();
}

std::string processFullProgram(std::vector<std::string>& fullProgram,
                               std::unordered_map<std::string, int>& symbolTable) {
  std::stringstream ss;
  int auxPosition;
  int symbPos, PCPos;
  std::string programStr;
  for (auto const& cmd : fullProgram) {
    if (isNumber(cmd)) {
      ss << cmd;
    } else {
      auxPosition = cmd.find("&");
      symbPos = symbolTable.find(cmd.substr(0, auxPosition))->second;
      PCPos = std::stoi(cmd.substr(auxPosition + 1));
      ss << (symbPos - PCPos);
    }
    ss << " ";
  }
  programStr = ss.str();
  programStr.pop_back();
  return programStr;
}

}
