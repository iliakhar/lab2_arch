#pragma once

#include<fstream>
#include<map>
#include<list>
#include "mem.h"
void SAtranslator(std::string safilename, std::string ofilename);

bool hexToDec(std::string hex, int& dec);
std::string decToHex(int dec);
std::list<std::string> PolishNotation(std::string eqation);
bool checkEqationFormat(std::string equation);

class SBTranslator {
	int currVarPos;
	int currSaLineNumb;
	std::map<std::string, int> variable;
	std::map<std::string, std::string> correspondingLines;
	std::string InputComand(std::string& currentLine, std::string& saLineNumb);
	std::string OutputComand(std::string& currentLine, std::string& saLineNumb);
	std::string GotoComand(std::string& currentLine, std::string& saLineNumb);
	std::string IfComand(std::string& currentLine, std::string& saLineNumb);
	std::string LetComand(std::string& currentLine);
	std::string GetCommand(std::string& currentLine);
	std::string GetStrSaLineNumber();
	std::string GetVarOrLiterAdress(std::string numb, std::string& saLine);
	void CreateVar(std::string name, std::string& saLine);
public:
	void Translator(std::string sbfilename, std::string safilename);
	SBTranslator() :currVarPos(97), currSaLineNumb(0) {}
};
