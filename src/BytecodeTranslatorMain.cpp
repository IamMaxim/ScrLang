#include <fstream>
#include <iostream>
#include "BytecodeTranslator.h"
#include "OpCodes.h"

int main(int argc, char* argv[]) {
    try {
        op::initMap();
        std::string in, out;

        if (argc == 3) {
            in = std::string(argv[1]);
            out = std::string(argv[2]);
        } else {
            printf("Type input filename:\n");
            getline(std::cin, in);
            printf("Type output filename:\n");
            getline(std::cin, out);
        }
        std::ifstream ifs(in);
        std::ofstream ofs(out);
        BytecodeTranslator translator;
        translator.translate(ifs, ofs);
        ifs.close();
        ofs.close();
    } catch (std::exception e) {
        std::cout << e.what() << '\n';
    }
    return 0;
}