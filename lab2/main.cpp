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

	MyTerm term;
	Ram ram;
	Flag reg;
	ram.sc_memoryLoad("ram.txt");
	ram.sc_memorySet(100, 678, reg);
	term.showTerm(ram, reg);
	term.runTerm(ram, reg);

	//printf("%x", 77);
}
