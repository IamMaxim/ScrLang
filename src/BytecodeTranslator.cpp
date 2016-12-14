//
// Created by maxim on 12/10/16.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
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
    std::map<std::string, unsigned int> labels;
    unsigned int varOffset = 0, label = 0;

    //read all labels
    while (!i.eof()) {
        std::string s;
        getline(i, s);
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        if (s.empty() || s.compare(0, comment.size(), comment) == 0) continue;
        std::vector<std::string> tokens = split(s);
        if (tokens[0] == "label") {
            printf("read label %s to ID %i\n", tokens[1].c_str(), label);
            labels[tokens[1]] = label++;
        }
    }
    i.clear();
    i.seekg(0, std::ios::beg);

    while (!i.eof()) {
        std::string s;
        getline(i, s);
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        if (s.empty() || s.compare(0, comment.size(), comment) == 0) continue;
        std::vector<std::string> tokens = split(s);
        uint8_t opcode = op::ops[tokens[0]];
//        printf("writing %s -> %i\n", s.c_str(), opcode);

        if (opcode == op::ASSIGN) {
            o.write((char *) &opcode, 1);
//            unsigned int varID = (unsigned int) std::stoul(tokens[1]);
            unsigned int varID = vars[tokens[1]];
            float val = std::stof(tokens[2]);
            printf("assign %f -> $%i\n", val, varID);
            o.write((char *) &varID, 4);
            o.write((char *) &val, 4);
        } else if (opcode == op::LABEL) {
            o.write((char *) &opcode, 1);
            o.write((char *) &labels[tokens[1]], 4);
            printf("writing label %i\n", labels[tokens[1]]);
        } else if (opcode == op::GOTO) {
            o.write((char *) &opcode, 1);
            unsigned int labelID = labels[tokens[1]];
            printf("writing goto %i\n", labelID);
            o.write((char *) &labelID, 4);
        } else if (opcode == op::MOV) {
            o.write((char *) &opcode, 1);
            unsigned int varID1 = (unsigned int) std::stoul(tokens[1]),
                    varID2 = (unsigned int) std::stoul(tokens[2]);
            o.write((char *) &varID1, 4);
            o.write((char *) &varID2, 4);
        } else if (opcode == op::VAR) {
            vars[tokens[1]] = varOffset;
            varOffset += 4;
        } else if (opcode == op::PUSH) {
            o.write((char *) &opcode, 1);
            unsigned int varID = vars[tokens[1]];
            o.write((char *) &varID, 4);
            printf("writing push -> %i\n", varID);
        } else if (opcode == op::POP) {
            o.write((char *) &opcode, 1);
            if (tokens.size() > 1) {
                unsigned int varID = vars[tokens[1]];
                o.write((char *) &varID, 4);
                printf("writing pop -> %i\n", varID);
            } else printf("writing pop\n");
        } else {
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
