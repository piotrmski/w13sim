#ifndef machine_state
#define machine_state

#include <stdbool.h>

struct MachineState {
    bool isRunning;
    unsigned short memory[0x1fff];
    unsigned short PC;
    unsigned short A;
};

struct MachineState getInitialState();

void step(struct MachineState* state);

#endif