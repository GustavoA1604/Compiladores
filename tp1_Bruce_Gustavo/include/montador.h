#ifndef MONTADOR_H
#define MONTADOR_H

#include <string>
#include <unordered_map>
#include <tuple>

namespace mtd {

typedef enum commands {
  CMD_HALT,
  CMD_LOAD,
  CMD_STORE,
  CMD_READ,
  CMD_WRITE,
  CMD_COPY,
  CMD_PUSH,
  CMD_POP,
  CMD_ADD,
  CMD_SUB,
  CMD_MUL,
  CMD_DIV,
  CMD_MOD,
  CMD_AND,
  CMD_OR,
  CMD_NOT,
  CMD_JUMP,
  CMD_JZ,
  CMD_JN,
  CMD_CALL,
  CMD_RET,
  CMD_WORD,
  CMD_END,
  CMD_ERROR
} commandT;

std::string removeExtraSpaces(std::string str);

std::string getElement(std::string& str);

commandT getCommandType(std::string commandStr);

int getCommandCode(commandT cmd);

std::tuple<std::string, std::string> getLabelCmdAndArgs(std::string lineWOComments);

void processSymbolTable(std::string line, std::unordered_map<std::string, int>& symbolTable, int& currentPosition);

std::string processLine(std::string line, std::unordered_map<std::string, int>& symbolTable, int& currentPosition);

}

#endif
