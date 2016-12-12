#include <stack>
#include <iostream>
#include <cstring>
#include "OpCodes.h"
#include "Interpreter.h"
#include "Utils.h"

inline void* Interpreter::stackAt(unsigned int offset) {
    return (void *)((uintptr_t)stack + offset);
}

void Interpreter::add() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 + f1);
}

void Interpreter::push(void *data) {
    float f;
    memcpy(&f, data, 4);
    stackPush(f);
}

void Interpreter::print() {
    int pos = stackPos;
    std::cout << "^^^^^^stack^^^^^^\n";
    while (pos > 0) {
        float f = *stackTop();
        std::cout << f << '\n';
        pos-=4;
    }
    std::cout << "^^^^^^^^^^^^^^^^^\n";
}

Interpreter::Interpreter(std::ifstream &s) {
    this->s = &s;
    stack = malloc(STACK_SIZE);
    res = malloc(RES_SIZE);

    operations[op::POP] = &Interpreter::pop;
    operations[op::PUSH] = &Interpreter::push;
    operations[op::ADD] = (void (Interpreter::*)(void *)) &Interpreter::add;
    operations[op::SUB] = (void (Interpreter::*)(void *)) &Interpreter::sub;
    operations[op::MUL] = (void (Interpreter::*)(void *)) &Interpreter::mul;
    operations[op::DIV] = (void (Interpreter::*)(void *)) &Interpreter::div;
    operations[op::PRINT] = (void (Interpreter::*)(void *)) &Interpreter::print;
    operations[op::LESS] = (void (Interpreter::*)(void *)) &Interpreter::less;
    operations[op::LESS_EQUAL] = (void (Interpreter::*)(void *)) &Interpreter::less_equal;
    operations[op::EQUAL] = (void (Interpreter::*)(void *)) &Interpreter::equal;
    operations[op::LARGER_EQUAL] = (void (Interpreter::*)(void *)) &Interpreter::larger_equal;
    operations[op::LARGER] = (void (Interpreter::*)(void *)) &Interpreter::larger;
    operations[op::GOTO] = &Interpreter::_goto;
    operations[op::IF] = (void (Interpreter::*)(void *)) &Interpreter::_if;
    operations[op::INCR] = (void (Interpreter::*)(void *)) &Interpreter::incr;
    operations[op::DECR] = (void (Interpreter::*)(void *)) &Interpreter::decr;
    operations[op::MOV] = &Interpreter::mov;
}

void Interpreter::sub() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 - f1);
}

void Interpreter::mul() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 * f1);
}

void Interpreter::div() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 / f1);
}

void Interpreter::pop(void *data) {
    if (data != 0) {
        float *f = stackTop();
        memcpy(data, f, 4);
    }
    stackPos-=4;
}

void Interpreter::less() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 < f1);
}

void Interpreter::less_equal() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 <= f1);
}

void Interpreter::equal() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 == f1);
}

void Interpreter::larger_equal() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 >= f1);
}

void Interpreter::larger() {
    float f1, f2;
    pop(&f1);
    pop(&f2);
    stackPush(f2 > f1);
}

void Interpreter::load() {
    unsigned int curOp = 0;
    while (true) {
        uint8_t opcode;
        s->read((char *) &opcode, 1);

        if (!s->good())
            return;

        std::string str;
        getline(*s, str);

        if (opcode == op::LABEL) {
#ifdef I_DEBUG
            printf("loaded label: %i\n", *str.data());
#endif
            labels[*str.data()] = curOp;
            continue;
        }

        unsigned long size = str.size() * sizeof(str[0]);
        void *dataMem = malloc(size);
        memcpy(dataMem, str.data(), size);
#ifdef I_DEBUG
        printf("loaded %i; data size: %i\n", opcode, (int) size);
#endif
        opFunc f = operations[opcode];
        callList.push_back(Call(f, dataMem));
    }
}

void Interpreter::run() {
    unsigned long size = callList.size();
    while (currentOp < size) {
        Call c = callList[currentOp++];
        (this->*c.func)(c.data);
    }
}

void Interpreter::setOpPos(unsigned int pos) {
    currentOp = pos;
}

void Interpreter::_goto(void *data) {
    currentOp = labels[*((unsigned int *) data)];
}

void Interpreter::_if() {
    float f;
    pop(&f);
    if (f != 1)
        currentOp++;
}

void Interpreter::incr() {
    (*stackTop())++;
}

void Interpreter::decr() {
    (*stackTop())--;
}

void Interpreter::mov(void *data) {
    memcpy(data, (void *) ((uintptr_t) data + 4), 4);
}

void Interpreter::stackPush(float f) {
    stackPos+=4;
    memcpy((void *) ((uintptr_t)stack + stackPos), &f, 4);
}

float *Interpreter::stackTop() {
    return &((float*)stack)[stackPos];
}

