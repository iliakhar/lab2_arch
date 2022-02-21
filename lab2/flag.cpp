#include "mem.h"

int Flag::sc_regInit() {
    flag = 0;
    return 0;
}

int Flag::sc_regSet(int reg, int value) {
    if (!(value == 1 || value == 0))
        return 1;
    if (value == 1)
        flag |= reg;
    else
        flag |= ~reg;
    return 0;
}

int Flag::sc_regGet(int reg, int* value) {
    if (!(reg == P || reg == O || reg == M || reg == T || reg == E))
        return 1;
    if ((flag & reg) == 0) *value = 0;
    else
        *value = 1;
    return 0;
}

int Flag::showFlags() {

    int regVal;
    char chReg[6] = { "POMTE" };
    for (int i = 1, ind = 0; i <= 16; i *= 2, ind++) {
        sc_regGet(i, &regVal);
        if (regVal)
            std::cout << chReg[ind];
        else
            std::cout << "_";
        std::cout << " ";
    }
    return 0;
}