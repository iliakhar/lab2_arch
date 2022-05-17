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
			if (val > 0xFFFF) reg.sc_regSet(P, 1);
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
		if(val>=0) printf("%x", val);
		else printf("-%x", abs(val));
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
		if (abs(accum) < 0xFFFF)
			rk_writeAccum(std::to_string(accum));
		else handleOverflow(operand);
		break;
	case 0x31:
		ram.sc_memoryGet(operand, &val, reg);
		accum -= val;
		if (abs(accum) < 0xFFFF)
			rk_writeAccum(std::to_string(accum));
		else handleOverflow(operand);
		break;
	case 0x32:
		ram.sc_memoryGet(operand, &val, reg);
		if(val != 0) accum /= val;
		else reg.sc_regSet(O, 1);
		if (abs(accum) < 0xFFFF)
			rk_writeAccum(std::to_string(accum));
		else handleOverflow(operand);
		break;
	case 0x33:
		ram.sc_memoryGet(operand, &val, reg);
		accum *= val;
		if (abs(accum) < 0xFFFF)
			rk_writeAccum(std::to_string(accum));
		else handleOverflow(operand);
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
	case 0x55:
		if (accum > 0) {
			SetPosInRam(operand % 10, operand / 10);
			printCounter();
		}
		break;
	case 0x73:
		int val, cell2Address;
		ram.sc_memoryGet(operand, &val, reg);
		ram.sc_memoryGet(accum, &cell2Address, reg);
		ram.sc_memorySet(cell2Address, val, reg);
		mt.gotoXY((cell2Address%10 * 7) + 2, cell2Address /10 + 1);
		ram.showNumInRam(val);
		break;
	}
	muteAsync.unlock();
}
void MyTerm::handleOverflow(int address) {
	reg.sc_regSet(T, 1);
	mt.gotoXY(82, 10);
	reg.showFlags();
	ram.sc_memorySet(address, 0, reg);
	reg.sc_regSet(P, 1);
	mt.gotoXY(82, 10);
	reg.showFlags();
}