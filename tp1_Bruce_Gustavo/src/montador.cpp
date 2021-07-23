#include <iostream>
#include <sstream>

#include "montador.h"

namespace mtd {

std::string removeExtraSpaces(std::string str) {
  return str.substr(str.find_first_not_of(" \r\n"), std::string::npos);
}

std::string getElement(std::string& str) {
  str = removeExtraSpaces(str);
  size_t firstSpaceAfterElement = str.find_first_of(" \r\n");
  std::string element = str.substr(0, firstSpaceAfterElement);
  str = firstSpaceAfterElement != std::string::npos
    ? str.substr(firstSpaceAfterElement, std::string::npos)
    : "";
  return element;
}

int getNumberOfOperandsPerCommand(commandT cmd) {
  switch (cmd) {
    case CMD_ERROR:
    case CMD_HALT:
    case CMD_RET:
    case CMD_END:
      return 1;
    case CMD_READ:
    case CMD_WRITE:
    case CMD_PUSH:
    case CMD_POP:
    case CMD_NOT:
    case CMD_JUMP:
    case CMD_JZ:
    case CMD_JN:
    case CMD_CALL:
    case CMD_WORD:
      return 2;
    default:
      return 3;
  }
}

int getMemorySpacePerCommand(commandT cmd) {
  switch (cmd) {
    case CMD_WORD:
    case CMD_END:
      return 0;
    default:
      return getNumberOfOperandsPerCommand(cmd);
  }
}

commandT getCommandType(std::string commandStr) {
  if (commandStr == "HALT") return CMD_HALT;
  if (commandStr == "LOAD") return CMD_LOAD;
  if (commandStr == "STORE") return CMD_STORE;
  if (commandStr == "READ") return CMD_READ;
  if (commandStr == "WRITE") return CMD_WRITE;
  if (commandStr == "PUSH") return CMD_PUSH;
  if (commandStr == "POP") return CMD_POP;
  if (commandStr == "ADD") return CMD_ADD;
  if (commandStr == "SUB") return CMD_SUB;
  if (commandStr == "MUL") return CMD_MUL;
  if (commandStr == "DIV") return CMD_DIV;
  if (commandStr == "MOD") return CMD_MOD;
  if (commandStr == "AND") return CMD_AND;
  if (commandStr == "OR") return CMD_OR;
  if (commandStr == "NOT") return CMD_NOT;
  if (commandStr == "JUMP") return CMD_JUMP;
  if (commandStr == "JZ") return CMD_JZ;
  if (commandStr == "JN") return CMD_JN;
  if (commandStr == "CALL") return CMD_CALL;
  if (commandStr == "RET") return CMD_RET;
  if (commandStr == "WORD") return CMD_WORD;
  if (commandStr == "END") return CMD_END;
  return CMD_ERROR;
}

int getCommandCode(commandT cmd) {
  switch (cmd) {
    case CMD_HALT: return 0;
    case CMD_LOAD: return 1;
    case CMD_STORE: return 2;
    case CMD_READ: return 3;
    case CMD_WRITE: return 4;
    case CMD_COPY: return 5;
    case CMD_PUSH: return 6;
    case CMD_POP: return 7;
    case CMD_ADD: return 8;
    case CMD_SUB: return 9;
    case CMD_MUL: return 10;
    case CMD_DIV: return 11;
    case CMD_MOD: return 12;
    case CMD_AND: return 13;
    case CMD_OR: return 14;
    case CMD_NOT: return 15;
    case CMD_JUMP: return 16;
    case CMD_JZ: return 17;
    case CMD_JN: return 18;
    case CMD_CALL: return 19;
    case CMD_RET: return 20;
    default: return -1;
  }
}

int getArgCode(std::string arg,
               std::unordered_map<std::string, int>& symbolTable) {

  // get the code for the general registers
  if (arg == "R0") return 0;
  if (arg == "R1") return 1;
  if (arg == "R2") return 2;
  if (arg == "R3") return 3;

  // get the code for labels
  std::unordered_map<std::string, int>::iterator it = symbolTable.find(arg);
  if (it != symbolTable.end()) return it->second;

  // fallback
  return -1;
}

std::tuple<std::string, std::string> getLabelCmdAndArgs(std::string lineWOComments) {
  size_t colon = lineWOComments.find(":");
  bool hasLabel = colon != std::string::npos;

  if (hasLabel) {
    return std::make_tuple(removeExtraSpaces(lineWOComments.substr(0, colon)),
                           removeExtraSpaces(lineWOComments.substr(colon+1, std::string::npos)));
  } else {
    return std::make_tuple("", removeExtraSpaces(lineWOComments));
  }
}

void processSymbolTable(std::string line,
                        std::unordered_map<std::string, int>& symbolTable,
                        int& currentPosition) {
  std::string lineWOComments = line.substr(0, line.find(";"));
  bool hasOnlySpaces = lineWOComments.find_first_not_of(" \r\n") == std::string::npos;

  if (hasOnlySpaces)
    return;

  std::string cmdAndArgs;
  std::string label;
  std::string command;
  commandT cmd;
  auto labelCmdAndArgsTuple = getLabelCmdAndArgs(lineWOComments);
  label = std::get<0>(labelCmdAndArgsTuple);
  cmdAndArgs = std::get<1>(labelCmdAndArgsTuple);
  command = getElement(cmdAndArgs);
  cmd = getCommandType(command);

  if (label != "") {
    symbolTable.insert(std::make_pair(label, currentPosition));
  }
  currentPosition += getMemorySpacePerCommand(cmd);
}

std::string processLine(std::string line,
                        std::unordered_map<std::string, int>& symbolTable,
                        int& PCPosition) {
  // std::cout << line << "\n";
  std::string lineWOComments = line.substr(0, line.find(";"));
  bool hasOnlySpaces = lineWOComments.find_first_not_of(" \r\n") == std::string::npos;

  if (hasOnlySpaces)
    return "";

  std::string cmdAndArgs;
  std::string label;
  std::string command;
  std::string commandCodeStr;
  commandT cmd;
  std::string arg1;
  std::string arg2;
  int commandCode, arg1Code, arg2Code;
  std::stringstream codeStream;

  // parse the assembly line
  auto labelCmdAndArgsTuple = getLabelCmdAndArgs(lineWOComments);
  label = std::get<0>(labelCmdAndArgsTuple);
  cmdAndArgs = std::get<1>(labelCmdAndArgsTuple);
  command = getElement(cmdAndArgs);
  cmd = getCommandType(command);
  switch (getNumberOfOperandsPerCommand(cmd)) {
    case 2:
      arg1 = getElement(cmdAndArgs);
      break;
    case 3:
      arg1 = getElement(cmdAndArgs);
      arg2 = getElement(cmdAndArgs);
    default:
      break;
  }

  // get the respective codes
  commandCode = getCommandCode(cmd);
  arg1Code = getArgCode(arg1, symbolTable);
  arg2Code = getArgCode(arg2, symbolTable);

  // update the program counter
  PCPosition += getMemorySpacePerCommand(cmd);

  // build the code line
  switch (cmd) {
    case CMD_HALT:
    case CMD_RET:
      codeStream << commandCode << " ";
      break;
    case CMD_LOAD:
    case CMD_STORE:
      codeStream << commandCode << " " << arg1Code << " " << (arg2Code - PCPosition) << " ";
      break;
    case CMD_READ:
    case CMD_WRITE:
    case CMD_PUSH:
    case CMD_POP:
    case CMD_NOT:
      codeStream << commandCode << " " << arg1Code << " ";
      break;
    case CMD_COPY:
    case CMD_ADD:
    case CMD_SUB:
    case CMD_MUL:
    case CMD_DIV:
    case CMD_MOD:
    case CMD_AND:
    case CMD_OR:
      codeStream << commandCode << " " << arg1Code << " " << arg2Code << " ";
      break;
    case CMD_JUMP:
    case CMD_JZ:
    case CMD_JN:
    case CMD_CALL:
      codeStream << commandCode << " " << (arg1Code - PCPosition) << " ";
      break;
    case CMD_WORD:
      codeStream << arg1 << " "; // TODO: check if this is correct
      break;
    case CMD_END:
    default:
      break;
  }
  return codeStream.str();
}
}
