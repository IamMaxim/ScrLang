#include <iostream>
#include "Interpreter.h"
#include "Utils.h"

//#define I_DEBUG
#define MEASURE_TIME
#define MEASURE_TIME_MILLIS
#define MEASURE_COUNT 1000000000

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
    for (int i = 0; i < MEASURE_COUNT; i++) {
        interpreter.setOpPos(0);
#endif

        interpreter.run();

#ifdef MEASURE_TIME
    }
#ifdef MEASURE_TIME_MILLIS
    time = getCurrentTime() - time;
#else
    time = clock() - time;
    time /= CLOCKS_PER_SEC;
#endif
    printf("completed %i iterations in %li\n", MEASURE_COUNT, time);
#endif

    return 0;
}