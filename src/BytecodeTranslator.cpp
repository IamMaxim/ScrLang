//
// Created by maxim on 12/10/16.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tic.h>
#include "BytecodeTranslator.h"
#include "Utils.h"
#include "OpCodes.h"

//#define PRINT_BYTECODE
//#define BT_DEBUG

static char lineSeparator = '\n';

/*
 * Translates human-readable assembler to bytecode
 * std::ifstream &i is input file
 * std::ofstream &o is output file
 */
void BytecodeTranslator::translate(std::ifstream &i, std::ofstream &o) {
#ifdef PRINT_BYTECODE
    std::stringstream ss;
#endif

    std::string comment("//");
    std::map<std::string, unsigned int> vars;
    std::map<std::string, unsigned int> labels;
    unsigned int varOffset = 0, label = 0;

    unsigned int curOp = 0;
    //read all labels
    while (!i.eof()) {
        std::string s;
        getline(i, s);
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        if (s.empty() || s.compare(0, comment.size(), comment) == 0) continue;

        std::vector<std::string> tokens = split(s);

        if (tokens[0] == "var")
            continue;
        if (tokens[0] == "label") {
#ifdef BT_DEBUG
            printf("read label %s to OP#%i\n", tokens[1].c_str(), curOp);
#endif
            labels[tokens[1]] = curOp;
            continue;
        }
        curOp++;
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

        if (opcode == op::ASSIGN) {
            o.write((char *) &opcode, 1);
            unsigned int varID = vars[tokens[1]];
            float val = std::stof(tokens[2]);
            o.write((char *) &varID, 4);
            o.write((char *) &val, 4);

#ifdef BT_DEBUG
            printf("assign %f -> $%i\n", val, varID);
#endif

#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
            ss << std::to_string(varID) << ' ';
            ss << std::to_string(varID);
#endif
        } else if (opcode == op::LABEL) {
            //skip
/*            o.write((char *) &opcode, 1);
            o.write((char *) &labels[tokens[1]], 4);
            printf("writing label %i\n", labels[tokens[1]]);*/
        } else if (opcode == op::GOTO) {
            o.write((char *) &opcode, 1);
            unsigned int labelID = labels[tokens[1]];
            o.write((char *) &labelID, 4);

#ifdef BT_DEBUG
            printf("writing goto %i\n", labelID);
#endif

#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
            ss << std::to_string(labelID);
#endif
        } else if (opcode == op::MOV) {
            o.write((char *) &opcode, 1);
            unsigned int varID1 = (unsigned int) std::stoul(tokens[1]),
                    varID2 = (unsigned int) std::stoul(tokens[2]);
            o.write((char *) &varID1, 4);
            o.write((char *) &varID2, 4);

#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
            ss << std::to_string(varID1) << ' ';
            ss << std::to_string(varID2);
#endif
        } else if (opcode == op::VAR) {
            vars[tokens[1]] = varOffset;
            varOffset += 4;
        } else if (opcode == op::PUSH) {
            o.write((char *) &opcode, 1);
            unsigned int varID = vars[tokens[1]];
            o.write((char *) &varID, 4);

#ifdef BT_DEBUG
            printf("writing push -> %i\n", varID);
#endif

#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
            ss << std::to_string(varID);
#endif
        } else if (opcode == op::POP) {
#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
#endif
            o.write((char *) &opcode, 1);
            if (tokens.size() > 1) {
                unsigned int varID = vars[tokens[1]];
                o.write((char *) &varID, 4);

#ifdef BT_DEBUG
                printf("writing pop -> %i\n", varID);
#endif

#ifdef PRINT_BYTECODE
                ss << std::to_string(varID);
#endif
            }
#ifdef BT_DEBUG
            else printf("writing pop\n");
#endif
        } else {
#ifdef PRINT_BYTECODE
            ss << std::to_string(opcode) << ' ';
#endif
            o.write((char *) &opcode, 1);
            for (int i = 1; i < tokens.size(); i++) {
                float f = std::stof(tokens[i]);
#ifdef PRINT_BYTECODE
                ss << std::to_string(f) << ' ';
#endif
                o.write((char *) &f, 4);
            }
        }
        if (opcode != op::VAR && opcode != op::LABEL) {
#ifdef PRINT_BYTECODE
            ss << '\n';
#endif
            o.write(&lineSeparator, 1);
        }
    }

#ifdef PRINT_BYTECODE
    std::cout << "SS:" << '\n';
    std::cout << ss.str() << '\n';
#endif
}
