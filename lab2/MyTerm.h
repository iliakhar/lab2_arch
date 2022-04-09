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
        Right,  Left, F5
    };
}

struct MyTermInfo {
    bool canon = false;
    double vtime = 0.7;
    int vmin = 3;
    bool echo = true;
    bool sigint = 1;
};

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
    bool isAsync = false;
    MyTermInfo termInfo;
    std::map<std::string, rk::keys> keyMap = {
        {"r", rk::Load}, {"s", rk::Save},{"i", rk::Reset}, {std::string(1, 63), rk::F5}
    };
    int showTerm();
    int rk_readKeyGetch(rk::keys* key);
    int rk_readKeyCin(rk::keys* key);
    int rk_myTermRegime(bool canon) { termInfo.canon = canon; return 0; };
    int rk_myTermRegime(bool canon, int vtime, int vmin, int echo, int sigint);
    int rk_myTermSave();
    int rk_myTermRestore();
    int rk_writeAccum(int val);
    void Timer(int time);
    int clearRamNum();

public:
    MyTerm() : posInRam({ 0 ,0 }) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
        //rk_myTermRestore();
        //auto f = BackPosToBeg;
        //signal(100, f);
        //reg.sc_regSet(T, 1);
        ram.sc_memoryLoad("ram.txt");
        ram.sc_memorySet(100, 678, reg);
        if(!termInfo.echo)
            mt_setCursorVisible(false);
    }

    void BackPosToBeg(int a) {
        clearRamNum();
        posInRam = { 1, 0 };
        ramPosMove(-1);
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


//void PsevdoBackPosToBeg(MyTerm& trm);


