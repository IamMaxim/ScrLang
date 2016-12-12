//
// Created by maxim on 12/10/16.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "BytecodeTranslator.h"
#include "Utils.h"
#include "OpCodes.h"

static char lineSeparator = '\n';

/*
 * Translates human-readable assembler to bytecode
 * std::ifstream &i is input file
 * std::ofstream &o is output file
 */
void BytecodeTranslator::translate(std::ifstream &i, std::ofstream &o) {
    while (!i.eof()) {
        std::string s;
        getline(i, s);
        if (s.empty()) continue;
        std::vector<std::string> tokens = split(s);
        uint8_t opcode = op::ops[tokens[0]];
        printf("writing %s -> %i\n", s.c_str(), opcode);
        o.write((char*) &opcode, 1);
        for (int i = 1; i < tokens.size(); i++) {
            float f = std::stof(tokens[i]);
            o.write((char *) &f, 4);
        }
        o.write(&lineSeparator, 1);
    }
}
