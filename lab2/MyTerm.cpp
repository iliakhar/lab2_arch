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
	mt_gotoXY(0, 85);
	std::cout << "accumulator";
	mt_gotoXY(3, 82);
	std::cout << "instructionCounter";
	mt_gotoXY(6, 86);
	std::cout << "Operation";
	mt_gotoXY(9, 88);
	std::cout << "Flags";
	mt_gotoXY(10, 86);
	reg.showFlags();

	mt_gotoXY(12, 57);
	std::cout << "Keys:";

	FILE* data;
	int res;
	char ch[30];
	data = fopen("menu.txt", "rb");
	if (data == NULL)
		return 1;
	short strNumb = 13;
	while (fgets(ch, 29, data)) {
		mt_gotoXY(strNumb, 55);
		std::cout << ch;
		strNumb++;
	}
	fclose(data);


	return 0;
}