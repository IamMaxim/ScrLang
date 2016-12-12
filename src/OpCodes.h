#pragma once

#include <string>
#include <map>

namespace op {
    extern uint8_t POP, PUSH, ADD, SUB, MUL, DIV, PRINT, LARGER, LARGER_EQUAL, EQUAL, LESS_EQUAL, LESS, LABEL, GOTO, IF, INCR, DECR, MOV, VAR;
    extern std::map<std::string, uint8_t> ops;
    void initMap();
};