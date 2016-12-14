//
// Created by maxim on 12/10/16.
//

#ifndef INTERPRETER_COMPILER_H
#define INTERPRETER_COMPILER_H


#include <fstream>

class Compiler {
public:
    void compile(std::ifstream &in, std::ofstream &out);
};


#endif //INTERPRETER_COMPILER_H
