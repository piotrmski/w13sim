#include "debug-runtime.h"
#include "../machine-state/machine-state.h"
#include "../keyboard-input/keyboard-input.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>

#define LABEL_NAME_MAX_LENGTH 32

static volatile bool isPaused = true;
static volatile bool isStepping = false;

static void handleSigInt(int _) {
    if (isPaused) {
        printf("\nQuitting\n");
        exit(0);
    } else {
        isPaused = true;
    }
}

static bool isPrintableChar(unsigned char ch) {
    return ch >= 32 && ch <= 126;
}

static char charUppercase(char ch) {
    if (ch >= 'a' && ch <= 'z') return ch - 0x20;
    else return ch;
}

static bool anyArgumentPresent(char* command) {
    for (int i = 1; command[i] != 0; ++i) {
        if (!isspace(command[i])) {
            printf("Command \"%c\" doesn't take any arguments. Type \"h\" to list all commands.\n", command[0]);
            return true;
        }
    }

    return false;
}

static void executeHelpCommand(char* command) {
    if (anyArgumentPresent(command)) return;

    printf("Commands:\n\
h     - prints this message\n\
l     - lists memory values from PC-5 to PC+5\n\
l X   - lists the memory value of X\n\
l X:Y - lists memory values from X to Y\n\
a     - lists all registered label names and their values\n\
r     - lists program counter value, instruction, memory value at instruction argument, and register A value\n\
b     - sets a breakpoint at PC\n\
b X   - sets a breakpoint at X\n\
d     - deletes a breakpoint at PC\n\
d X   - deletes a breakpoint at X\n\
c     - continues simulation\n\
s     - steps simulation (executes one instruction and pauses)\n\
q     - quits\n\
For a command argument value you may use one of the following:\n\
- a hexadecimal number starting with \"0x\",\n\
- PC,\n\
- PC+C or PC-C where C is a decimal number constant,\n\
- a label name,\n\
- L+C or L-C where L is a label name and C is a decimal number constant.\n");
}

static void executeListMemoryCommand(struct MachineState* state, char* command) {
    // TODO
}

static void executeListLabelsCommand(char* command) {
    if (anyArgumentPresent(command)) return;

    // TODO
}

static void executeListRegistersCommand(struct MachineState* state, char* command) {
    if (anyArgumentPresent(command)) return;

    // TODO
}

static void executeAddBreakpointCommand(char* command) {
    // TODO
}

static void executeDeleteBreakpointCommand(char* command) {
    // TODO
}

// Returns true if prompt interaction should continue, or false if simulation should resume
static bool executeContinueCommand(char* command) {
    if (anyArgumentPresent(command)) return true;

    return false;
}
// Returns true if prompt interaction should continue, or false if simulation should resume

static bool executeStepCommand(char* command) {
    if (anyArgumentPresent(command)) return true;

    isStepping = true;

    return false;
}

static void executeQuitCommand(char* command) {
    if (anyArgumentPresent(command)) return;

    printf("Quitting\n");
    exit(0);
}

// Returns true if prompt interaction should continue, or false if simulation should resume
static bool parseCommand(struct MachineState* state, char* command) {
    if (command[1] == 0 || isspace(command[1])) {
        switch (charUppercase(command[0]) ) {
            case 'H':
                executeHelpCommand(command);
                return true;
            case 'L':
                executeListMemoryCommand(state, command);
                return true;
            case 'A':
                executeListLabelsCommand(command);
                return true;
            case 'R':
                executeListRegistersCommand(state, command);
                return true;
            case 'B':
                executeAddBreakpointCommand(command);
                return true;
            case 'D':
                executeDeleteBreakpointCommand(command);
                return true;
            case 'C':
                return executeContinueCommand(command);
            case 'S':
                return executeStepCommand(command);
            case 'Q':
                executeQuitCommand(command);
                return true;
        }
    }

    printf("Unrecognized command. Type \"h\" to list all commands.\n");
    return true;
}

static void interactivePrompt(struct MachineState* state) {
    printf("Paused. TODO add the same data as from command R\n");
    bool interactive = true;
    while (interactive) {
        printf("> ");
        char command[128] = {0};
        fgets(command, 127, stdin);
        interactive = parseCommand(state, command);
    }
}

void runDebug(struct MachineState* state, const char* symbolsFilePath) {
    printf("Starting in debug mode. Type \"h\" to list all commands or \"c\" to begin simulation. Press ^C during simulation to pause.\n");

    signal(SIGINT, handleSigInt);

    startCharacterInput();

    do {
        if (isPaused || isStepping) {
            isPaused = true;
            isStepping = false;
            endCharacterInput();
            interactivePrompt(state);
            startCharacterInput();
            if (isStepping) {
                step(state);
            }
            isPaused = false;
        } else {
            step(state);
        }
    } while (!state->isUnconditionalInfiniteLoop);

    endCharacterInput();

    printf("Unconditional infinite loop detected. Ending simulation.\n");
}
