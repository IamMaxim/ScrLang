#include <iostream>
#include "Interpreter.h"
#include "Utils.h"

#define I_DEBUG
//#define MEASURE_TIME
//#define MEASURE_TIME_MILLIS

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
#ifdef MEASURE_TIME_MILLIS
    long time = getCurrentTime();
#else
    clock_t time = clock();
#endif
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < 10000; j++) {
            interpreter.setOpPos(0);
#endif

            interpreter.run();

#ifdef MEASURE_TIME
        }
    }
#ifdef MEASURE_TIME_MILLIS
    time = getCurrentTime() - time;
#else
    time = clock() - time;
#endif
    printf("completed 100000000 iterations in %li\n", time);
#endif

    return 0;
}