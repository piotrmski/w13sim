#include <stdio.h>
#include <stdbool.h>
#include "program-input/program-input.h"
#include "keyboard-input/keyboard-input.h"
#include "machine-state/machine-state.h"

int main(int argc, const char * argv[]) {
    struct ProgramInput programInput = getProgramInput(argc, argv);

    if (programInput.error) { return 1; }

    if (programInput.binaryFilePath == NULL) { return 0; }

    FILE* binaryFile = fopen(programInput.binaryFilePath, "rb");

    if (binaryFile == NULL) {
        printf("Error: could not read file \"%s\"\n.", programInput.binaryFilePath);
        return 1;
    }

    fseek(binaryFile, 0, SEEK_END);
    int fileSize = ftell(binaryFile);
    int programLength = fileSize / sizeof(unsigned short);

    if (fileSize % 2 != 0 || programLength > 0x1FFE) {
        printf("Error: The binary file size is invalid, should be even and less than 16382 bytes.\n"); // TODO make sure that the validity check and error message is correct.
        fclose(binaryFile);
        return 1;
    }

    struct MachineState state = getInitialState();

    fseek(binaryFile, 0, SEEK_SET);
    fread(state.memory, sizeof(unsigned short), programLength, binaryFile);
    fclose(binaryFile);

    // TODO check symbols file existence and validity, and create the debug runtime

    startCharacterInput();

    do {
        step(&state);
    } while (state.isRunning);

    endCharacterInput();

    return 0;
}