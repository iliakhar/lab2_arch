#include"MyTerm.h"
#include<iostream>

int MyTerm::mt_clrscr() {

	system("cls");
	return 0;
}

int MyTerm::mt_gotoXY(short y, short x) {
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

int MyTerm::showTerm(Ram& ram, Flag& reg) {

	ram.showRam(reg);
	mt_gotoXY(10, 82);
	reg.showFlags();


	FILE* data;
	int res;
	char ch[30];
	data = fopen("menu.txt", "rb");
	if (data == NULL)
		return 1;
	short strNumb = 13;
	while (fgets(ch, 29, data)) {
		mt_gotoXY(strNumb, 50);
		std::cout << " " << ch;
		strNumb++;
	}
	fclose(data);
	bc_printBigNumber(0xFAB2, { 1, 13 });


	bc_box({ 0, 0 }, { 72, 12 }, "Memory");
	bc_box({ 72, 0 }, { 30, 3 }, "accumulator");
	bc_box({ 72, 3 }, { 30, 3 }, "instructionCounter");
	bc_box({ 72, 6 }, { 30, 3 }, "Operation");
	bc_box({ 72, 9 }, { 30, 3 }, "Flags");
	bc_box({ 0, 12 }, { 48, 10 });
	bc_box({ 49, 12 }, { 53, 10 }, "Keys");std::cout << "\n\n";
	return 0;
}

int MyTerm::runTerm(Ram& ram, Flag& reg) {

	posInRam = { 1, 0 };
	ramPosMove(-1, ram, reg);
	char key;
	while (1) {
		key = getch();
		if (key == 0 || key == 224)
			key = getch();
		switch (key) {
		case 75: ramPosMove(-1, ram, reg);
			break;
		case 77: ramPosMove(1, ram, reg);
			break;
		case 'e':return 0;
		default: break;
		}
	}
	return 0;
}

int MyTerm::ramPosMove(int move, Ram& ram, Flag& reg) {

	int numInRam;
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	mt_gotoXY(posInRam.Y + 1, (posInRam.X * 7) + 2);
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
	mt_gotoXY(posInRam.Y + 1, (posInRam.X * 7) + 2);
	ram.sc_memoryGet(posInRam.Y * 10 + posInRam.X, &numInRam, reg);
	ram.showNumInRam(numInRam);
	mt_setbgcolor(mt::Black);
	bc_printBigNumber(numInRam, { 1, 13 });
	mt_gotoXY(22, 0);
	return 0;
}