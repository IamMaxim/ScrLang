#pragma once

#include <fstream>
#include <map>
#include <stack>
#include <vector>

#define RES_SIZE 4096

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
    void *res;
    std::map<uint8_t, opFunc> operations;
    std::stack<float> stack;
    std::vector<Call> callList;
    std::map<unsigned int, unsigned int> labels;
    unsigned int currentOp = 0;
    std::ifstream *s;
public:
    Interpreter(std::ifstream &s);
    void load();
    void run();
    void setOpPos(unsigned int pos);

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
    void larger_equal();
    void larger();
    void _goto(void *data);
    void _if();
    void incr();
    void decr();
    void mov(void *data);
};