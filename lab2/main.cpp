#include<iostream>
#include<windows.h>
#include<string>
#include"MyTerm.h"
#include"mem.h"

void showFlags(Flag& reg) {
	int* val = new int;
	for (int i = 1; i <= E; i *= 2) {
		reg.sc_regGet(i, val);
		std::cout << *val;
	}
	delete val;
}

int main() {

	//MyTerm term;
	//int a;
	//int* rows = new int;
	//int* cols = new int;
	//
	//term.mt_setbgcolor(mt::White);
	//term.mt_setfgcolor(mt::Red);
	//term.mt_getscreensize(rows, cols);
	//std::cout << "SSS";
	//term.mt_gotoXY(3, 7);
	//std::cin >> a;
	//term.mt_clrscr();
	//delete rows;
	//delete cols;

	MyTerm term;
	Ram ram;
	Flag reg;
	ram.sc_memorySet(5, 34, reg);
	ram.sc_memorySet(6, 9999, reg);
	ram.sc_memorySet(7, -1111, reg);
	ram.sc_memorySet(8, 678, reg);
	ram.sc_memorySet(100, 678, reg);
	term.showTerm(ram, reg);
	
	//MyTerm term;
	//term.bc_printBigChar(9, { 5, 8 });
	
}
