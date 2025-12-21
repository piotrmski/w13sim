#ifndef program_input
#define program_input

#include <stdbool.h>

struct ProgramInput {
    bool debugMode;
    const char* binaryFilePath;
    const char* symbolsFilePath;
    int clockFrequencyKiloHz;
};

struct ProgramInput getProgramInput(int argc, const char * argv[]);

#endif