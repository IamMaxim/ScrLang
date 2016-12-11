#include <iostream>
#include "Interpreter.h"
#include "Utils.h"


int main(int argc, char *argv[]) {
    std::string filename;
    if (argc == 0 || argc == 1) {
        printf("Type filename:\n");
        getline(std::cin, filename);
    } else {
        filename = std::string(argv[1]);
    }

    std::ifstream ifs(filename);
    Interpreter interpreter(ifs);

    interpreter.load();

#ifdef MEASURE_TIME
    long startTime = getCurrentTime();
    for (int i = 0; i < 10000000; i++) {
        interpreter.setOpPos(0);
#endif

    interpreter.run();

#ifdef MEASURE_TIME
    }
    long delta = getCurrentTime() - startTime;
    printf("completed 10000000 iterations in %li ms\n", delta);
#endif

    return 0;
}