#include"MyTerm.h"

int TermVisual::clrscr() {

	system("cls");
	return 0;
}

int TermVisual::gotoXY(short x, short y) {
	SetConsoleCursorPosition(hConsole, { x, y });
	return 0;
}

int TermVisual::getscreensize(int* rows, int* cols) {
	*rows = inf.srWindow.Bottom + 1;
	*cols = inf.srWindow.Right + 1;

	return 0;
}

int TermVisual::setfgcolor(enum mt::colors cl) {
	GetConsoleScreenBufferInfo(hConsole, &inf);
	SetConsoleTextAttribute(hConsole, (inf.wAttributes & 0b11110000) | cl);
	return 0;
}

int TermVisual::setbgcolor(enum mt::colors cl) {
	GetConsoleScreenBufferInfo(hConsole, &inf);
	SetConsoleTextAttribute(hConsole, (cl << 4) | (inf.wAttributes & 0b1111));
	return 0;
}

int TermVisual::setCursorVisible(bool visible) {

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
	return 0;
}