#include "OpCodes.h"

uint8_t op::POP = 0,
        op::PUSH = 1,
        op::ADD = 2,
        op::SUB = 3,
        op::MUL = 4,
        op::DIV = 5,
        op::PRINT = 6,
        op::LESS = 7,
        op::LESS_EQUAL = 8,
        op::EQUAL = 9,
        op::GREATER_EQUAL = 10,
        op::GREATER = 11,
        op::LABEL = 12,
        op::GOTO = 13,
        op::IF = 14,
        op::INCR = 15,
        op::DECR = 16,
        op::MOV = 17,
        op::VAR = 18,
        op::ASSIGN = 19;

std::map<std::string, uint8_t> op::ops;

void op::initMap() {
    ops["pop"] = POP;
    ops["push"] = PUSH;
    ops["add"] = ADD;
    ops["sub"] = SUB;
    ops["mul"] = MUL;
    ops["div"] = DIV;
    ops["print"] = PRINT;
    ops["less"] = LESS;
    ops["less_equal"] = LESS_EQUAL;
    ops["equal"] = EQUAL;
    ops["greater_equal"] = GREATER_EQUAL;
    ops["greater"] = GREATER;
    ops["label"] = LABEL;
    ops["goto"] = GOTO;
    ops["if"] = IF;
    ops["incr"] = INCR;
    ops["decr"] = DECR;
    ops["mov"] = MOV;
    ops["var"] = VAR;
    ops["assign"] = ASSIGN;
}