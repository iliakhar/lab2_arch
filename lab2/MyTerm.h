#pragma once
#include<windows.h>
#include<cmath>
#include<utility>
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

class MyTerm {
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO inf;
    bigNum bn[12] = { {3284402046, 2130691011}, {1987868784, 4168118384}, {4276158463,  4294903679},
                      {4292935551, 2147475711}, {4293388263, 3772834047}, {2131230719, 2147475711},
                      {2131216254, 2127022055}, {1893793791, 118365240}, {4291035006, 2130691071},
                      {4291035006, 473460990}, {4265097216, 3684606}, {4261412864, 254} };
public:
    MyTerm():hConsole(GetStdHandle(STD_OUTPUT_HANDLE)) {
        //bc_bigCharRead("BigNum.txt", 12);
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    }
    int bc_box(COORD start, COORD size, std::string frameName = "");
    int bc_printBigChar(int num, COORD start, mt::colors font = mt::LightGray, mt::colors bg = mt::Black);
    int bc_getBigCharPos(bigNum bigN, COORD pos, int &val);
    int bc_setBigCharPos(bigNum& bigN, COORD pos, int val);
    int bc_bigCharRead(std::string filename, int count);
    int mt_clrscr();
    int mt_gotoXY(short y, short x);
    int mt_getscreensize(int* rows, int* cols);
    int mt_setfgcolor(enum mt::colors cl);
    int mt_setbgcolor(enum mt::colors cl);
    int showTerm(Ram &ram, Flag &reg);
};

