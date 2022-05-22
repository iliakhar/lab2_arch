#include "Translator.h"

void SBTranslator::Translator(std::string sbfilename, std::string safilename) {
	try {
		std::ifstream sbfile(sbfilename);
		
		std::string currentComment, currentLine, tmpLineNumb, command, saLine, ifLine, allSaLines;
		int currSbLineNumb(0), endCount(0);
		
		while (std::getline(sbfile, currentLine)) {
			if(endCount>0) throw std::string("Invalid End");
			for (int i(0); i < currentLine.size(); i++)
				if (currentLine[i] >= '0' && currentLine[i] <= '9')
					tmpLineNumb.push_back(currentLine[i]);
				else if (currentLine[i] == ' ')break;
				else throw std::string("Invalid line number");
			if (currSbLineNumb < stoi(tmpLineNumb))
				currSbLineNumb = stoi(tmpLineNumb);
			else throw std::string("Incorrect line numbering");
			std::string saLineNumb(GetStrSaLineNumber());
			correspondingLines.insert({ tmpLineNumb, saLineNumb });
			currentLine = currentLine.substr(currentLine.find_first_of(' ') + 1);
			command = GetCommand(currentLine);
				
			//std::cout << tmpLineNumb << " (" << saLineNumb << ")" << "\t" << command << "#\t" << currentLine << "#\n";

			if (command == "IF") {
				isIfCommand = true;
				ifLine = IfComand(currentLine);
				command = GetCommand(currentLine);
				isIfCommand = false;
				if (command == "REM")
					throw std::string("Invalid second command");
			}
			if (command == "INPUT") {
				saLine = InputComand(currentLine);
			}
			else if (command == "OUTPUT") {
				saLine = OutputComand(currentLine);
			}
			else if (command == "GOTO") {
				saLine = GotoComand(currentLine);
			}
			else if (command == "LET") {
				saLine = LetComand(currentLine);
				currSaLineNumb--;
			}
			else if (command == "END") {
				//if(endCount>0) throw std::string("End > 1");
				if(currentLine.empty())
					saLine = saLineNumb + " HALT 00";
				else throw std::string("Invalid End command");
				endCount++;
			}
			else if (command == "REM") {
				currentComment = "  ;" + currentLine;
				tmpLineNumb.clear();
				continue;
			}
			else throw std::string("Invalid command");
			if (!currentComment.empty()) {
				saLine += currentComment;
				currentComment.clear();
			}
			if (!ifLine.empty()) {
				ifLine += GetStrSaLineNumber(currSaLineNumb + 1) + "\n";
				saLine = ifLine + saLine;
				ifLine.clear();
			}
			allSaLines += saLine + "\n";
			saLine.clear();
			tmpLineNumb.clear();
			currSaLineNumb++;
		}
		sbfile.close();
		if(currSaLineNumb >= currVarPos) throw std::string("RAM overflow");

		std::list<std::string> saProg;
		while (allSaLines != "") {
			saProg.push_back(allSaLines.substr(0, allSaLines.find_first_of('\n')));
			allSaLines = allSaLines.substr(allSaLines.find_first_of('\n') + 1);
		}
		
		FixGoto(sbfilename, saProg);
		std::ofstream safile(safilename);
		for (auto it(saProg.begin()); it != saProg.end(); it++) {
			safile << *it;
			safile << "\n";
		}
	}
	catch (std::string strErr) {
		std::cout << strErr;
		exit(1);
	}
	catch (...) {
		std::cout << "\nHZ che ne tak\n";
		exit(1);
	}
}

void SBTranslator::FixGoto(std::string filename, std::list<std::string> &saProg) {
	std::string currentLine, tmpLineNumb, gotoVal;
	std::ifstream sbfile(filename);
	auto it = saProg.begin();
	while (std::getline(sbfile, currentLine)) {
		if (currentLine.find("GOTO") != std::string::npos) {
			currentLine = currentLine.substr(currentLine.find_first_of("GOTO") + 5);
			if(currentLine.size() == 1)currentLine = "0" + currentLine;
			gotoVal = correspondingLines[currentLine];
			if (correspondingLines.find(currentLine) == correspondingLines.end())
				throw std::string("Invalid Goto");
			while (it->find("JUMP ?") == std::string::npos) it++;
			*it = it->substr(0, it->find_first_of("JUMP") + 5) + gotoVal + it->substr(it->find_first_of("JUMP") + 6);
			it++;
		}
	}
}

std::string SBTranslator::GetCommand(std::string& currentLine) {
	std::string command;
	if (currentLine.find(' ') != std::string::npos) {
		command = currentLine.substr(0, currentLine.find_first_of(' '));
		currentLine = currentLine.substr(currentLine.find_first_of(' ') + 1);
	}
	else {
		command = currentLine;
		currentLine.clear();
	}
	return command;
}

std::string SBTranslator::InputComand(std::string& currentLine) {
	std::string saLine;
	if (currentLine.size() != 1)
		throw std::string("Invalid operand");
	if (!(currentLine[0] >= 'A' && currentLine[0] <= 'Z'))
		throw std::string("Invalid operand");
	saLine = GetStrSaLineNumber() + " READ ";
	if (variable.find(currentLine.substr(0, 1)) == variable.end()) {
		variable.insert({ currentLine.substr(0, 1), currVarPos });
		currVarPos--;
	}
	saLine += std::to_string(variable[currentLine.substr(0, 1)]);
	return saLine;
}

std::string SBTranslator::OutputComand(std::string& currentLine) {
	std::string saLine;
	if (currentLine.size() == 1 && currentLine[0] >= 'A' && currentLine[0] <= 'Z') {
		if (variable.find(currentLine.substr(0, 1)) == variable.end())
			CreateVar(currentLine.substr(0, 1), saLine, 0);
		saLine = GetStrSaLineNumber() + " WRITE " + std::to_string(variable[currentLine.substr(0, 1)]);
	}
	else if (currentLine.size() >= 1) {
		saLine = GetStrSaLineNumber() + " WRITE ";
		for (int i(0); i < currentLine.size(); i++)
			if (!(currentLine[i] >= '0' && currentLine[i] <= '9'))
				throw std::string("Invalid operand");
		saLine += currentLine;
	}
	return saLine;
}

std::string SBTranslator::GotoComand(std::string& currentLine) {
	std::string saLine(GetStrSaLineNumber() + " JUMP ?");
	if (currentLine.empty())
		throw std::string("Invalid operand");
	for (int i(0); i < currentLine.size(); i++)
		if (!(currentLine[i] >= '0' && currentLine[i] <= '9'))
			throw std::string("Invalid operand");
	/*if (currentLine.size() == 1)currentLine = "0" + currentLine;
	if (correspondingLines.find(currentLine) != correspondingLines.end())
		saLine += correspondingLines[currentLine];
	else throw std::string("Invalid operand");*/
	return saLine;
}

std::string SBTranslator::IfComand(std::string& currentLine) {
	std::string condition, saLine;
	int symbPos(-1);
	char allSymb[3] = { '>','<','=' };
	char symb(0);
	if (currentLine.find(' ') != std::string::npos) {
		condition = currentLine.substr(0, currentLine.find_first_of(' '));
		currentLine = currentLine.substr(currentLine.find_first_of(' ') + 1);
	}
	else throw std::string("Invalid condition");
	for (int i(0); i < 3; i++)
		if (condition.find_first_of(allSymb[i]) != std::string::npos) {
			symbPos = condition.find_first_of(allSymb[i]);
			break;
		}
	if(symbPos == -1) throw std::string("Invalid condition");
	symb = condition[symbPos];
	condition[symbPos] = '-';
	condition = condition.substr(0, symbPos + 1) + "(" + condition.substr(symbPos + 1) + ")";
	condition = "@=" + condition;
	saLine = LetComand(condition);
	if (symb == '=') {
		saLine += "\n" + GetStrSaLineNumber() + " JZ " + GetStrSaLineNumber(currSaLineNumb + 2) + "\n";
		currSaLineNumb++;
	}
	else if (symb == '<') {
		saLine += "\n" + GetStrSaLineNumber() + " JNEG " + GetStrSaLineNumber(currSaLineNumb + 2) + "\n";
		currSaLineNumb++;
	}
	else if (symb == '>') {
		/*saLine += "\n" + InvertNumbToAccum(GetVarOrLiterAdress("@"));
		saLine += GetStrSaLineNumber() + " JNEG " + GetStrSaLineNumber(currSaLineNumb + 2) + "\n";
		currSaLineNumb++;*/
		saLine += "\n" + GetStrSaLineNumber() + " JNS " + GetStrSaLineNumber(currSaLineNumb + 2) + "\n";
		currSaLineNumb++;
	}
	saLine += GetStrSaLineNumber() + " JUMP ";
	currSaLineNumb++;
		
	return saLine;
}

std::string SBTranslator::GetStrSaLineNumber() {
	std::string saLineNumb("00");
	saLineNumb[0] = '0' + currSaLineNumb / 10;
	saLineNumb[1] = '0' + currSaLineNumb % 10;
	return saLineNumb;
}

std::string SBTranslator::GetStrSaLineNumber(int numb) {
	std::string saLineNumb("00");
	saLineNumb[0] = '0' + numb / 10;
	saLineNumb[1] = '0' + numb % 10;
	return saLineNumb;
}

void SBTranslator::CreateVar(std::string name, std::string& saLine, int val) {
	variable.insert({ name, currVarPos });
	currVarPos--;
	if(val>=0) saLine += std::to_string(currVarPos+1) + " = +"+decToHex(val)+"\n";
	else saLine += std::to_string(currVarPos + 1) + " = -" + decToHex(val) + "\n";
}

//ab + cd +*
//#0 cd +*
//#0 #1 *
//Та да!!!!!!!!!!!!
std::string SBTranslator::LetComand(std::string& currentLine) {
	std::string saLine, address;
	int pseudoVarCount(0);
	std::map<char, std::string>allOperat = { {'+', "ADD"}, {'-', "SUB"}, {'*', "MUL"}, {'/', "DIVIDE"} };
	if (!(currentLine.size() >= 3 && (currentLine[0] >= 'A' && currentLine[0] <= 'Z' || (currentLine[0] == '@') && isIfCommand) && currentLine[1] == '='))
		throw std::string("Invalid equation");
	if (variable.find(currentLine.substr(0, 1)) == variable.end()) {
		CreateVar(currentLine.substr(0, 1), saLine, 0);
	}
	std::string answerVar(currentLine.substr(0,1));
	currentLine = currentLine.substr(currentLine.find_first_of('=') + 1);
	std::list<std::string> equation = PolishNotation(currentLine);
	auto iter = equation.begin();
	while (iter != equation.end()) {
		if (allOperat.find((*iter)[0]) == allOperat.end()) {
			int copyFrom(0);
			if ((*iter)[0] == '_') copyFrom = 1;
			if (variable.find((*iter).substr(copyFrom)) == variable.end())
				if ((*iter)[1] >= 'A' && (*iter)[1] <= 'Z')
					CreateVar((*iter).substr(copyFrom), saLine, 0);
				else CreateVar((*iter).substr(copyFrom), saLine, stoi((*iter).substr(copyFrom)));
		}
		iter++;
	}
	if (equation.size() == 1) {
		iter = equation.begin();
		saLine += PutVarToAccum(*iter);
	}
	else {
		iter = equation.begin();
		while (iter != equation.end()) {
			if (*iter == "+" || *iter == "-" || *iter == "*" || *iter == "/") {
				char operat = (*iter)[0];
				iter--;
				iter--;
				saLine += PutVarToAccum(*iter);
				iter = equation.erase(iter);
				address = GetVarOrLiterAdress(*iter);
				if ((*iter)[0] == '_')
					if (operat == '-') operat = '+';
					else if (operat == '+') operat = '-';
				saLine += GetStrSaLineNumber() + " " + allOperat[operat] + " " + address + "\n";
				currSaLineNumb++;
				if ((*iter)[0] == '_' && (operat == '*' || operat == '/')) {
					saLine += GetStrSaLineNumber() + " STORE 98\n";
					currSaLineNumb++;
					saLine += InvertNumbToAccum("98");
				}
				
				
				iter = equation.erase(iter);
				iter = equation.erase(iter);
				if (equation.size() > 1) {
					saLine += GetStrSaLineNumber() + " STORE " + GetVarOrLiterAdress("#" + std::to_string(pseudoVarCount)) + "\n";
					currSaLineNumb++;
				}
				iter = equation.insert(iter, "#" + std::to_string(pseudoVarCount));
				pseudoVarCount++;
			}
			iter++;
		}
	}
	
	saLine += GetStrSaLineNumber() + " STORE " + std::to_string(variable[answerVar]);
	currSaLineNumb++;
	//if (saLine[saLine.size() - 1] == '\n') saLine.pop_back();
	return saLine;
}
std::string SBTranslator::InvertNumbToAccum(std::string adr) {
	std::string saLine;
	saLine += GetStrSaLineNumber() + " LOAD 99\n";
	currSaLineNumb++;
	saLine += GetStrSaLineNumber() + " SUB "+ adr + "\n";
	currSaLineNumb++;
	return saLine;
}

std::string SBTranslator::GetVarOrLiterAdress(std::string numb) {
	if (numb[0] == '_') numb = numb.substr(1);
	if (variable.find(numb) != variable.end())
		return std::to_string(variable[numb]);
	else {
		variable.insert({ numb, currVarPos });
		currVarPos--;
		return std::to_string(currVarPos + 1);
	}
}

std::string SBTranslator::PutVarToAccum(std::string& numb) {
	std::string address = GetVarOrLiterAdress(numb);
	std::string saLine;
	if (numb[0] == '_') {
		saLine += GetStrSaLineNumber() + " LOAD 99\n";
		currSaLineNumb++;
		saLine += GetStrSaLineNumber() + " SUB " + address + "\n";
		currSaLineNumb++;
	}
	else {
		saLine += GetStrSaLineNumber() + " LOAD " + address + "\n";
		currSaLineNumb++;
	}
	return saLine;
}


std::string decToHex(int dec) {
	std::string hex;
	int remainder;
	for (int decDegree = 0x1000; decDegree != 0; decDegree /= 16) {
		remainder = dec / decDegree;
		if (remainder >= 0 && remainder <= 9)
			hex.push_back('0' + remainder);
		else hex.push_back('a' + remainder - 10);
		dec %= decDegree;
	}
	return hex;
}
