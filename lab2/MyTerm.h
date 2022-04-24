#pragma once
#include<windows.h>
#include<cmath>
#include<utility>
#include<fstream>
#include<conio.h>
#include<ctime>
#include<map>
#include <future>
#include <csignal>
#include <functional>
#include <mutex>
#include"mem.h"
using bigNum = std::pair<unsigned int, unsigned int>;
namespace mt {
    enum colors {
        Black,  Blue,   Green,
        Cyan,   Red,     Magenta,
        Brown,  LightGray,  DarkGray,
        LightBlue,  LightGreen, LightCyan,
        LightRed,   LightMagenta,
        Yellow, White
    };
}

namespace rk {
    enum keys {
        Load, Save, Run,
        Step, Reset, Accumulator,
        InstructionCounter, ERR,
        Right,  Left, F5, F6
    };
}

struct MyTermInfo {
    bool canon = false;
    double vtime = 0.8;
    int vmin = 5;
    bool echo = true;
    bool sigint = 1;
};

class TermVisual {
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO inf;
public:
    TermVisual():hConsole(GetStdHandle(STD_OUTPUT_HANDLE)) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    }
    int clrscr();
    int setCursorVisible(bool visible);
    int gotoXY(short x, short y);
    int getscreensize(int* rows, int* cols);
    int setfgcolor(enum mt::colors cl);
    int setbgcolor(enum mt::colors cl);
};

class PseudoGraphics {
    bigNum bn[18];
    TermVisual *termVis;
public:
    PseudoGraphics(TermVisual &tvis):termVis(&tvis) {
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
    
    COORD posInRam;
    PseudoGraphics termGraphics;
    Ram ram;
    Flag reg; 
    Operation oper;
    int accum = 0;
    std::recursive_mutex muteAsync;
    MyTermInfo termInfo;
    TermVisual mt;
    std::map<std::string, rk::keys> keyMap = {
        {"r", rk::Load}, {"s", rk::Save}, {"i", rk::Reset}, {"t", rk::Step}, {std::string(1, 63), rk::F5}
        , {std::string(1, 64), rk::F6}
    };
    int showTerm();
    std::string rk_readKeyGetch(rk::keys* key, bool isAutoEnter);

    int rk_myTermRegime(bool canon) { termInfo.canon = canon; return 0; };
    int rk_myTermRegime(bool canon, int vtime, int vmin, int echo, int sigint);
    int rk_myTermSave();
    int rk_myTermRestore();

    int rk_writeAccum(std::string val);
    void Timer(int time);
    int clearRamNum();
    void clearLine(int symbCount, int lineNumber);
    void printCounter();


public:
    MyTerm() : posInRam({ 0 ,0 }),termGraphics(mt) {
        ram.sc_memoryLoad("ram.txt");
        if(!termInfo.echo)
            mt.setCursorVisible(false);
    }

    void SetPosInRam(short x, short y);
    int runTerm();
    int ramPosMove(int move);
};