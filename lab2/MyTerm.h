#pragma once
#include<windows.h>
#include<cmath>
#include<utility>
#include<fstream>
#include<conio.h>
#include"mem.h"
using bigNum = std::pair<unsigned int, unsigned int>;
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

class PseudoGraphics {
    bigNum bn[18];
public:
    PseudoGraphics() {
        bc_bigCharRead("BigNum.txt", 18);
    }
    int bc_box(COORD start, COORD size, std::string frameName = "");
    int bc_printBigChar(int num, COORD start, mt::colors font = mt::LightGray, mt::colors bg = mt::Black);
    int bc_printBigNumber(int num, COORD start, mt::colors font = mt::LightGray, mt::colors bg = mt::Black);
    int bc_getBigCharPos(bigNum bigN, COORD pos, int& val);
    int bc_setBigCharPos(bigNum& bigN, COORD pos, int val);
    int bc_bigCharRead(std::string filename, int count);
};

class MyTerm {
    static HANDLE hConsole;
    static CONSOLE_SCREEN_BUFFER_INFO inf;
    COORD posInRam;
    PseudoGraphics termGraphics;
    Ram ram;
    Flag reg;
    int showTerm();
public:
    MyTerm() :posInRam({ 0 ,0 }) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
        ShowCursor(false);
        ram.sc_memoryLoad("ram.txt");
        ram.sc_memorySet(100, 678, reg);
        mt_setCursorVisible(false);
    }

    static int mt_clrscr();
    static int mt_setCursorVisible(bool visible);
    static int mt_gotoXY(short x, short y);
    static int mt_getscreensize(int* rows, int* cols);
    static int mt_setfgcolor(enum mt::colors cl);
    static int mt_setbgcolor(enum mt::colors cl);
    int runTerm();
    int ramPosMove(int move);
};


