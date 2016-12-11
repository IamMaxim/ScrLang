#pragma once

#include <vector>
#include <sstream>
#include <chrono>

static long getCurrentTime() {
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return ms.count();
}

static std::vector<std::string> split(std::string &s) {
    std::vector<std::string> tokens;
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (getline(ss, item, ' ')) {
        tokens.push_back(item);
    }
    return tokens;
}

static float readf(std::ifstream &s) {
    float f;
    s.read((char *) &f, 4);
    return f;
}

static int readi(std::ifstream &s) {
    int i;
    s.read((char *) &i, 4);
    return i;
}

static uint8_t readb(std::ifstream &s) {
    uint8_t b;
    s.read((char *) &b, 1);
    return b;
}