#include <stack>
#include <iostream>
#include <cstring>
#include "OpCodes.h"
#include "Interpreter.h"
#include "Utils.h"

//#define I_DEBUG

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
//    operations[op::VAR] = &Interpreter::var;
    operations[op::ASSIGN] = &Interpreter::assign;
}

inline void *offset(void *data, int offset) {
    return (void *) ((uintptr_t) data + offset);
}

inline void *Interpreter::stackAt(int offset) {
    return (void *) ((uintptr_t) stack + offset);
}

inline void Interpreter::stackMov(float *ptr) {
    memcpy(ptr, stackTop(), 4);
}

inline void *Interpreter::varAt(unsigned int offset) {
    return (void *) ((uintptr_t) res + offset);
}

void Interpreter::add() {
    float f1, f2;
    stackMov(&f1);
    stackPos -= 4;
    stackMov(&f2);
    stackPos -= 4;
    stackPush(f2 + f1);
}

void Interpreter::push(void *data) {
//    stackPush(*(float *) vars[(*(unsigned int *) data)]);
    stackPush(*(float *) varAt((*(unsigned int *) data)));
//    stackPush(*(float *) data);
}

void Interpreter::print() {
/*    std::cout << "=====vars=====\n";
    std::map<unsigned int, void *>::iterator it = vars.begin();
    while (it != vars.end()) {
        float f;
        memcpy(&f, it->second, 4);
        std::cout << it->first << " -> " << f << '\n';
        it = std::next(it);
    }

    std::cout << "==============\n";*/

    int pos = stackPos;
    std::cout << "^^^^^^stack^^^^^^\n";
    while (pos > 0) {
        std::cout << *(float*)stackAt(pos) << '\n';
        pos -= 4;
    }
    std::cout << "^^^^^^^^^^^^^^^^^\n";
}

void Interpreter::sub() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 - f1);
}

void Interpreter::mul() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 * f1);
}

void Interpreter::div() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 / f1);
}

void Interpreter::pop(void *data) {
    if (data != 0) {
        memcpy(varAt(*(unsigned int *) data), stackTop(), 4);
    }
    stackPos -= 4;
}

void Interpreter::less() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 < f1);
}

void Interpreter::less_equal() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 <= f1);
}

void Interpreter::equal() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 == f1);
}

void Interpreter::larger_equal() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
    stackPush(f2 >= f1);
}

void Interpreter::larger() {
    float f1, f2;
    stackMov(&f1);
    stackMov(&f2);
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
    stackMov(&f);
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
    memcpy(varAt(*(unsigned int *) data), varAt(*(unsigned int *) offset(data, 4)), 4);
}

void Interpreter::stackPush(float f) {
#ifdef I_DEBUG
    printf("pushing %f to stack\n", f);
#endif
    stackPos += 4;
    memcpy(stackAt(stackPos), &f, 4);
}

float *Interpreter::stackTop() {
    return (float *) stackAt(stackPos);
}

//void Interpreter::var(void *id) {
//    vars[*((unsigned int *) id)] = offset(res, resPos);
//    resPos += 4;
//}

void Interpreter::assign(void *data) {
#ifdef I_DEBUG
    printf("assigning %f to $%i\n",
           *(float *) offset(data, 4),
           *(unsigned int *) data);
#endif
    memcpy(varAt(*(unsigned int *) data), offset(data, 4), 4);
}
