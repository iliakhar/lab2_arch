#pragma once
#include<windows.h>
#include<cmath>
#include"mem.h"

namespace mt {
    enum colors {
        Black,
        Blue,
        Green,
        Cyan,
        Red,
        Magenta,
        Brown,
        LightGray,
        DarkGray,
        LightBlue,
        LightGreen,
        LightCyan,
        LightRed,
        LightMagenta,
        Yellow,
        White
    };
}


class MyTerm {
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO inf;
public:
    MyTerm():hConsole(GetStdHandle(STD_OUTPUT_HANDLE)) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    }
    int mt_clrscr();
    int mt_gotoXY(short y, short x);
    int mt_getscreensize(int* rows, int* cols);
    int mt_setfgcolor(enum mt::colors cl);
    int mt_setbgcolor(enum mt::colors cl);
    int showTerm(Ram &ram, Flag &reg);
};

