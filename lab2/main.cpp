#include<iostream>
#include<windows.h>
#include<string>
#include"MyTerm.h"
#include"mem.h"
#include "Disk.h"
#include "Translator.h"

int main() {
	SBTranslator sbtr;
	sbtr.Translator("basic.sb", "basic.sa");
	SAtranslator("basic.sa", "basic.o");
	MyTerm term("basic.o");
	term.CPU();
	//PolishNotation("5*3/4*2/3-2+2");
	//PolishNotation("-A*(2*(-B+1))/132-C");
	
}
