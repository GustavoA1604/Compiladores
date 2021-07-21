#include <iostream>

#include "montador.h"

namespace mtd {

std::string removeExtraSpaces(std::string str) {
  return str.substr(str.find_first_not_of(" "), std::string::npos);
}

std::string getElement(std::string& str) {
  str = removeExtraSpaces(str);
  size_t firstSpaceAfterElement = str.find_first_of(" ");
  std::string element = str.substr(0, firstSpaceAfterElement);
  str = firstSpaceAfterElement != std::string::npos 
    ? str.substr(firstSpaceAfterElement, std::string::npos)
    : "";
  return element;
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

std::string processLine(std::string line) {
  std::cout << line << "\n";
  std::string lineWOComments = line.substr(0, line.find(";"));
  bool hasOnlySpaces = lineWOComments.find_first_not_of(" ") == std::string::npos;
  
  if (hasOnlySpaces)
    return "";

  std::string lineWCmdAndArgs;
  std::string label;
  std::string command;
  commandT cmd;
  std::string arg1;
  std::string arg2;
  size_t colon = lineWOComments.find(":");
  bool hasLabel = colon != std::string::npos;

  if (hasLabel) {
    label = removeExtraSpaces(lineWOComments.substr(0, colon));
    lineWCmdAndArgs = removeExtraSpaces(lineWOComments.substr(colon+1, std::string::npos));
  } else {
    lineWCmdAndArgs = removeExtraSpaces(lineWOComments);
  }

  command = getElement(lineWCmdAndArgs);
  cmd = getCommandType(command);
  switch (cmd) {
    case CMD_ERROR:
      std::cout << "Command " << command << " not supported" << std::endl;
      return "";
    case CMD_HALT:
    case CMD_RET:
    case CMD_END:
      break;
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
      arg1 = getElement(lineWCmdAndArgs);
      break;
    default:
      arg1 = getElement(lineWCmdAndArgs);
      arg2 = getElement(lineWCmdAndArgs);
      break;
  }

  switch (cmd) {
    case CMD_HALT:
      break;
    case CMD_LOAD:
      break;
    case CMD_STORE:
      break;
    case CMD_READ:
      break;
    case CMD_WRITE:
      break;
    case CMD_COPY:
      break;
    case CMD_PUSH:
      break;
    case CMD_POP:
      break;
    case CMD_ADD:
      break;
    case CMD_SUB:
      break;
    case CMD_MUL:
      break;
    case CMD_DIV:
      break;
    case CMD_MOD:
      break;
    case CMD_AND:
      break;
    case CMD_OR:
      break;
    case CMD_NOT:
      break;
    case CMD_JUMP:
      break;
    case CMD_JZ:
      break;
    case CMD_JN:
      break;
    case CMD_CALL:
      break;
    case CMD_RET:
      break;
    case CMD_WORD:
      break;
    case CMD_END:
      break; 
    default:
      break; 
  }

  //std::cout << "label: \"" << label << "\" command: \"" 
  //<< command << "\" arg1: \"" << arg1 << "\" arg2: \"" << arg2 << "\"\n";

  return (hasLabel ? label + ":" : "") + 
  ((cmd == CMD_HALT || cmd == CMD_RET) ? command 
  : (arg2 == "" ? command + " " + arg1
    : command + " " + arg1 + " " + arg2)) + " ";
}

}
