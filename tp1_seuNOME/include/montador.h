#ifndef MONTADOR_H
#define MONTADOR_H

#include <string>

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

std::string processLine(std::string line);

}

#endif
