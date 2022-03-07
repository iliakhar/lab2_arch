#include"MyTerm.h"
#include<iostream>

HANDLE MyTerm::hConsole(GetStdHandle(STD_OUTPUT_HANDLE));
CONSOLE_SCREEN_BUFFER_INFO MyTerm::inf;

int MyTerm::mt_clrscr() {

	system("cls");
	return 0;
}

int MyTerm::mt_gotoXY(short x, short y) {
	SetConsoleCursorPosition(hConsole, {x, y});
	return 0;
}

int MyTerm::mt_getscreensize(int* rows, int* cols) {
	*rows = inf.srWindow.Bottom + 1;
	*cols = inf.srWindow.Right + 1;

	return 0;
}

int MyTerm::mt_setfgcolor(enum mt::colors cl) {
	GetConsoleScreenBufferInfo(hConsole, &inf);
	SetConsoleTextAttribute(hConsole, (inf.wAttributes & 0b11110000)|cl);
	return 0;
}

int MyTerm::mt_setbgcolor(enum mt::colors cl) {
	GetConsoleScreenBufferInfo(hConsole, &inf);
	SetConsoleTextAttribute(hConsole, (cl << 4)|(inf.wAttributes & 0b1111));
	return 0;
}

int MyTerm::mt_setCursorVisible(bool visible) {

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
	return 0;
}

int MyTerm::showTerm() {

	ram.showRam(reg);
	mt_gotoXY(82, 10);
	reg.showFlags();


	FILE* data;
	int res;
	char ch[30];
	data = fopen("menu.txt", "rb");
	if (data == NULL)
		return 1;
	short strNumb = 13;
	while (fgets(ch, 29, data)) {
		mt_gotoXY(50, strNumb);
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

int MyTerm::runTerm() {

	showTerm();
	posInRam = { 1, 0 };
	ramPosMove(-1);
	char key;
	while (1) {
		key = getch();
		if (key == 0 || key == 224)
			key = getch();
		switch (key) {
		case 75: ramPosMove(-1);
			break;
		case 77: ramPosMove(1);
			break;
		case 'e':return 0;
		default: break;
		}
	}
	return 0;
}

int MyTerm::ramPosMove(int move) {

	int numInRam;
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	mt_gotoXY((posInRam.X * 7) + 2, posInRam.Y + 1);
	ram.showNumInRam(numInRam);
	mt_setbgcolor(mt::LightMagenta);
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
	mt_gotoXY((posInRam.X * 7) + 2, posInRam.Y + 1);
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	ram.showNumInRam(numInRam);
	mt_setbgcolor(mt::Black);
	termGraphics.bc_printBigNumber(numInRam, { 1, 13 });
	mt_gotoXY(0, 22);
	return 0;
}