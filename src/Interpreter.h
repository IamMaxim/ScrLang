#pragma once

#include <fstream>
#include <map>
#include <stack>
#include <vector>

#define RES_SIZE 4096
#define STACK_SIZE 4096

class Interpreter {
    typedef void (Interpreter::*opFunc)(void *data);
    struct Call {
        opFunc func;
        void *data;

        Call(opFunc func, void *data) {
            this->func = func;
            this->data = data;
        }
    };
private:
//    std::map<unsigned int, void*> vars;
    void *res;
    std::map<uint8_t, opFunc> operations;
    void* stack;
    unsigned int stackPos = 0, resPos = 0;
    std::vector<Call> callList;
//    std::map<unsigned int, unsigned int> labels;
    unsigned int currentOp = 0;
    std::ifstream *s;
    inline void* stackAt(int offset);
public:
    Interpreter(std::ifstream &s);
    void load();
    void run();
    void setOpPos(unsigned int pos);
    void stackPush(float f);
    float *stackTop();
    inline void stackMov(float *ptr);
    void *varAt(unsigned int offset);

    void pop(void* data);
    void push(void* data);
    void add();
    void sub();
    void mul();
    void div();
    void print();
    void less();
    void less_equal();
    void equal();
    void greater_equal();
    void greater();
    void _goto(void *data);
    void _if();
    void incr();
    void decr();
    void mov(void *data);
//    void var(void *id);
    void assign(void *data);
};