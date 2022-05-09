#include "MyTerm.h"

int MyTerm::ALU(int command, int operand) {
	rk::keys key;
	int val;
	muteAsync.lock();
	switch (command) {
	case 0x10:
		try {
			mt.gotoXY(0, 22);
			std::cout << "Enter a number:";
			val = stoi(readNumber(23));
			ram.sc_memorySet(operand, val, reg);
			mt.gotoXY(2 + operand % 10 * 7, 1 + operand / 10);
			ram.showNumInRam(val);
			mt.gotoXY(0, 22);
		}
		catch (...) {
			mt.clrscr();
			std::cout << "ERROR ERROR ERROR";
			exit(1);
		}
		break;
	case 0x11:
		ram.sc_memoryGet(operand, &val, reg);
		clearLine(40, 23);
		mt.gotoXY(0, 23);
		printf("%x", val);
		break;
	case 0x20:
		ram.sc_memoryGet(operand, &val, reg);
		accum = val;
		rk_writeAccum(std::to_string(accum));
		break;
	case 0x21:
		ram.sc_memorySet(operand, accum, reg);
		mt.gotoXY(2 + operand % 10 * 7, 1 + operand / 10);
		ram.showNumInRam(accum);
		mt.gotoXY(0, 22);
		break;
	case 0x30:
		ram.sc_memoryGet(operand, &val, reg);
		accum += val;
		rk_writeAccum(std::to_string(accum));
		break;
	case 0x31:
		ram.sc_memoryGet(operand, &val, reg);
		accum -= val;
		rk_writeAccum(std::to_string(accum));
		break;
	case 0x32:
		ram.sc_memoryGet(operand, &val, reg);
		accum *= val;
		rk_writeAccum(std::to_string(accum));
		break;
	case 0x33:
		ram.sc_memoryGet(operand, &val, reg);
		accum /= val;
		rk_writeAccum(std::to_string(accum));
		break;
	case 0x40:
		SetPosInRam(operand % 10, operand / 10);
		printCounter();
		break;
	case 0x41:
		if (accum < 0) {
			SetPosInRam(operand % 10, operand / 10);
			printCounter();
		}
		break;
	case 0x42:
		if (accum == 0) {
			SetPosInRam(operand % 10, operand / 10);
			printCounter();
		}
			
		break;
	case 0x43:
		reg.sc_regSet(T, 1);
		mt.gotoXY(82, 10);
		reg.showFlags();
		break;
	}
	muteAsync.unlock();
};