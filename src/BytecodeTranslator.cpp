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
    std::string comment("//");
    std::map<std::string, unsigned int> vars;
    unsigned int varOffset = 0;

    while (!i.eof()) {
        std::string s;
        getline(i, s);
        if (s.empty() || s.compare(0, comment.size(), comment) == 0) continue;
        std::vector<std::string> tokens = split(s);
        uint8_t opcode = op::ops[tokens[0]];
        printf("writing %s -> %i\n", s.c_str(), opcode);

        if (opcode == op::ASSIGN)
        {
            o.write((char *) &opcode, 1);
//            unsigned int varID = (unsigned int) std::stoul(tokens[1]);
            unsigned int varID = vars[tokens[1]];
            float val = std::stof(tokens[2]);
            printf("assign %f -> $%i\n", val, varID);
            o.write((char *) &varID, 4);
            o.write((char *) &val, 4);
        } else if (
                opcode == op::LABEL ||
                opcode == op::GOTO)
        {
            o.write((char *) &opcode, 1);
            unsigned int varID = (unsigned int) std::stoul(tokens[1]);
            o.write((char *) &varID, 4);
        } else if (opcode == op::MOV)
        {
            o.write((char *) &opcode, 1);
            unsigned int varID1 = (unsigned int) std::stoul(tokens[1]),
                         varID2 = (unsigned int) std::stoul(tokens[2]);
            o.write((char *) &varID1, 4);
            o.write((char *) &varID2, 4);
        } else if (opcode == op::VAR)
        {
            vars[tokens[1]] = varOffset;
            varOffset += 4;
        } else if (opcode == op::PUSH ||
                opcode == op::POP)
        {
            o.write((char *) &opcode, 1);
            unsigned int varID = vars[tokens[1]];
            o.write((char *) &varID, 4);
            if (opcode == op::PUSH)
                printf("writing push -> %i\n", varID);
            else
                printf("writing pop -> %i\n", varID);
        }
        else {
            o.write((char *) &opcode, 1);
            for (int i = 1; i < tokens.size(); i++) {
                float f = std::stof(tokens[i]);
                o.write((char *) &f, 4);
            }
        }
        if (opcode != op::VAR)
            o.write(&lineSeparator, 1);
    }
}
