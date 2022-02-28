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
		mt_gotoXY(strNumb, 58);
		std::cout << " " << ch;
		strNumb++;
	}
	fclose(data);

	bc_printBigChar(10, { 1, 13 });
	for (short i = 10; i <= 46; i += 9)
		bc_printBigChar(0, {  i, 13 });

	bc_box({ 0, 0 }, { 72, 12 }, "Memory");
	bc_box({ 72, 0 }, { 30, 3 }, "accumulator");
	bc_box({ 72, 3 }, { 30, 3 }, "instructionCounter");
	bc_box({ 72, 6 }, { 30, 3 }, "Operation");
	bc_box({ 72, 9 }, { 30, 3 }, "Flags");
	bc_box({ 0, 12 }, { 56, 10 });
	bc_box({ 57, 12 }, { 46, 10 }, "Keys");std::cout << "\n\n";
	return 0;
}