#include"MyTerm.h"
#include<iostream>

void MyTerm::clearLine(int symbCount, int lineNumber) {
	mt.gotoXY(0, lineNumber);
	for (int i = 0; i < symbCount; i++)
		std::cout << " ";
	mt.gotoXY(0, lineNumber);
}

int MyTerm::showTerm() {

	ram.showRam(reg);
	mt.gotoXY(82, 10);
	reg.showFlags();

	FILE* data;
	int res;
	char ch[30];
	data = fopen("menu.txt", "rb");
	if (data == NULL)
		return 1;
	short strNumb = 13;
	while (fgets(ch, 29, data)) {
		mt.gotoXY(50, strNumb);
		std::cout << " " << ch;
		strNumb++;
	}
	fclose(data);
	termGraphics.bc_printBigNumber(0xFAB2, { 1, 13 });

	termGraphics.bc_box({ 0, 0 }, { 72, 12 }, "Memory");
	termGraphics.bc_box({ 72, 0 }, { 30, 3 }, "accumulator");
	termGraphics.bc_box({ 72, 3 }, { 30, 3 }, "instructionCounter");
	termGraphics.bc_box({ 72, 6 }, { 30, 3 }, "Operation");
	termGraphics.bc_box({ 72, 9 }, { 30, 3 }, "Flags");
	termGraphics.bc_box({ 0, 12 }, { 48, 10 });
	termGraphics.bc_box({ 49, 12 }, { 53, 10 }, "Keys");std::cout << "\n\n";
	return 0;
}

int MyTerm::CPU() {

	showTerm();
	posInRam = { 1, 0 };
	ramPosMove(-1);
	rk::keys key;
	std::future<void> f;
	rk_writeAccum("0");
	printOperation();
	while (1) {
		printCounter();
		rk_readKeyGetch(&key);
		int regTval;
		reg.sc_regGet(T, &regTval);
		if(regTval == 1)
			switch (key) {
			case rk::Left: 
				ramPosMove(-1);
				printOperation();
				break;
			case rk::Right:
				ramPosMove(1);
				printOperation();
				break;
			case rk::Save: rk_myTermSave();
				break;
			case rk::Step: ExecuteCell();
				break;
			case rk::Reset:
				muteAsync.lock();
				rk_writeAccum("0");
				ram.sc_memoryInit();
				mt.gotoXY(0, 0);
				ram.sc_memoryObjLoad(filename);
				ram.showRam(reg);
				termGraphics.bc_box({ 0, 0 }, { 72, 12 }, "Memory");
				SetPosInRam(0, 0);
				reg.sc_regInit();
				reg.sc_regSet(T, 1);
				muteAsync.unlock();
				break;
			case rk::Run:
				f = std::async(std::launch::async, &MyTerm::Timer, this, timerSpeed);
				reg.sc_regSet(T, 0);
				break;
			case rk::Load: rk_myTermRestore();
				break;
			case rk::SetSpeed: {
				muteAsync.lock();
				clearLine(40, 22);
				muteAsync.unlock();
				timerSpeed = stoi(readNumber(22));
				break;
			}
			case rk::F5: {
				muteAsync.lock();
				clearLine(40, 22);
				muteAsync.unlock();
				rk_writeAccum(readNumber(22));
				break;
			}
			case rk::F6: {
				muteAsync.lock();
				clearLine(40, 22);
				muteAsync.unlock();
				try {
					int newRamPos = stoi(readNumber(22));
					if (newRamPos >= 0 && newRamPos < 100) {
						SetPosInRam(newRamPos % 10, newRamPos / 10);
						printOperation();
					}
					else
						reg.sc_regSet(M, 1);
				}
				catch(...){}
				break;
			}
			default: break;
			}
		else if(key == rk::Run)
			reg.sc_regSet(T, 1);
		muteAsync.lock();
		mt.gotoXY(82, 10);
		reg.showFlags();
		clearLine(40, 22);
		clearLine(40, 23);
		muteAsync.unlock();
	}
	return 0;
}

void MyTerm::Timer(int time) {
	int val;
	while (1) {
		reg.sc_regGet(T, &val);
		if (val != 0)
			return;
		ExecuteCell();
		Sleep(time);
	}
}

std::pair<int, int> MyTerm::GetComAndOp() {
	int command, operand, operation;
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &operation, reg);
	oper.sc_commandDecode(operation, &command, &operand, reg);
	return { command, operand };
}

void MyTerm::printOperation() {
	int operation;
	muteAsync.lock();
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &operation, reg);
	mt.gotoXY(82, 7);
	oper.ShowCommandAndOperand(operation, reg);
	muteAsync.unlock();
}

void MyTerm::ExecuteCell() {
	COORD tmpCord;
	tmpCord = posInRam;
	muteAsync.lock();
	mt.gotoXY(82, 10);
	reg.showFlags();
	muteAsync.unlock();
	std::pair<int, int> comAndOp = GetComAndOp();
	ALU(comAndOp.first, comAndOp.second);
	if (tmpCord.Y == posInRam.Y && tmpCord.X == posInRam.X)
		ramPosMove(1);
	printOperation();
	printCounter();
}

int MyTerm::clearRamNum() {
	int numInRam;
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	mt.gotoXY((posInRam.X * 7) + 2, posInRam.Y + 1);
	ram.showNumInRam(numInRam);
	return numInRam;
}

int MyTerm::ramPosMove(int move) {
	muteAsync.lock();
	int numInRam = clearRamNum();
	mt.setbgcolor(mt::LightMagenta);
	int border = 9, incr = 1;
	if (move < 0) {
		border = 0;
		incr = -1;
	}
	if (posInRam.X != border || posInRam.Y != border)
		if (posInRam.X == border) {
			posInRam.X = 9 - border;
			posInRam.Y += incr;
		}
		else posInRam.X += incr;
	mt.gotoXY((posInRam.X * 7) + 2, posInRam.Y + 1);
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	ram.showNumInRam(numInRam);
	mt.setbgcolor(mt::Black);
	termGraphics.bc_printBigNumber(numInRam, { 1, 13 });
	
	mt.gotoXY(0, 22);
	muteAsync.unlock();
	return 0;
}

void MyTerm::printCounter() {
	muteAsync.lock();
	
	mt.gotoXY(86, 4);
	std::cout << "  ";
	mt.gotoXY(86, 4);
	std::cout << posInRam.X + posInRam.Y * 10;
	mt.gotoXY(0, 22);
	muteAsync.unlock();
}

void MyTerm::SetPosInRam(short x, short y) {
	muteAsync.lock();
	clearRamNum();
	if (!(y >= 0 && y < 10 && x >= 0 && x < 10)) {
		muteAsync.unlock();
		return;
	}
	if (x >= 0 && x < 9) {
		posInRam = { short(x + 1), y };
		ramPosMove(-1);
	}
	else if (x == 9) {
		posInRam = { short(x - 1), y };
		ramPosMove(1);
	}
	posInRam = { x,y };
	muteAsync.unlock();
}