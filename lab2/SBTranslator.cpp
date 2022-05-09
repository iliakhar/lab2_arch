#include "Translator.h"

void SBTranslator::Translator(std::string sbfilename, std::string safilename) {
	try {
		std::ifstream sbfile(sbfilename);
		std::ofstream safile(safilename);
		std::string currentComment, currentLine, tmpLineNumb, command, saLine;
		int currSbLineNumb(0);

		while (std::getline(sbfile, currentLine)) {

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
				saLine = IfComand(currentLine, saLineNumb);
				command = GetCommand(currentLine);
			}

			if (command == "INPUT") {
				saLine = InputComand(currentLine, saLineNumb);
			}
			else if (command == "OUTPUT") {
				saLine = OutputComand(currentLine, saLineNumb);
			}
			else if (command == "GOTO") {
				saLine = GotoComand(currentLine, saLineNumb);
			}
			else if (command == "LET") {
				saLine = LetComand(currentLine);
				currSaLineNumb--;
			}
			else if (command == "END") {
				if(currentLine.empty())
					saLine = saLineNumb + " HALT 00";
				else throw std::string("Invalid End command");
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
			safile << saLine;
			safile << "\n";
			saLine.clear();
			tmpLineNumb.clear();
			currSaLineNumb++;
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

std::string SBTranslator::InputComand(std::string& currentLine, std::string& saLineNumb) {
	std::string saLine;
	if (currentLine.size() != 1)
		throw std::string("Invalid operand");
	if (!(currentLine[0] >= 'A' && currentLine[0] <= 'Z'))
		throw std::string("Invalid operand");
	saLine = saLineNumb + " READ ";
	if (variable.find(currentLine.substr(0, 1)) == variable.end()) {
		variable.insert({ currentLine.substr(0, 1), currVarPos });
		currVarPos--;
	}
	saLine += std::to_string(variable[currentLine.substr(0, 1)]);
	return saLine;
}

std::string SBTranslator::OutputComand(std::string& currentLine, std::string& saLineNumb) {
	std::string saLine;
	if (currentLine.size() == 1 && currentLine[0] >= 'A' && currentLine[0] <= 'Z') {
		if (variable.find(currentLine.substr(0, 1)) == variable.end())
			CreateVar(currentLine.substr(0, 1), saLine);
		saLine = saLineNumb + " WRITE " + std::to_string(variable[currentLine.substr(0, 1)]);
	}
	else if (currentLine.size() >= 1) {
		saLine = saLineNumb + " WRITE ";
		for (int i(0); i < currentLine.size(); i++)
			if (!(currentLine[i] >= '0' && currentLine[i] <= '9'))
				throw std::string("Invalid operand");
		saLine += currentLine;
	}
	return saLine;
}

std::string SBTranslator::GotoComand(std::string& currentLine, std::string& saLineNumb) {
	std::string saLine(saLineNumb + " JUMP ");
	if (currentLine.empty())
		throw std::string("Invalid operand");
	for (int i(0); i < currentLine.size(); i++)
		if (!(currentLine[i] >= '0' && currentLine[i] <= '9'))
			throw std::string("Invalid operand");
	if (correspondingLines.find(currentLine) != correspondingLines.end())
		saLine += correspondingLines[currentLine];
	else throw std::string("Invalid operand");
	return saLine;
}

std::string SBTranslator::IfComand(std::string& currentLine, std::string& saLineNumb) {
	return "Haha";
}

std::string SBTranslator::GetStrSaLineNumber() {
	std::string saLineNumb("00");
	saLineNumb[0] = '0' + currSaLineNumb / 10;
	saLineNumb[1] = '0' + currSaLineNumb % 10;
	return saLineNumb;
}

void SBTranslator::CreateVar(std::string name, std::string& saLine) {
	variable.insert({ name, currVarPos });
	currVarPos--;
	saLine += std::to_string(currVarPos+1) + " = +0000\n";
}

std::string SBTranslator::LetComand(std::string& currentLine) {
	std::string saLine, address;
	int pseudoVarCount(0);
	std::map<char, std::string>allOperat = { {'+', "ADD"}, {'-', "SUB"}, {'*', "MUL"}, {'/', "DIVIDE"} };
	if (!(currentLine.size() >= 3 && currentLine[0] >= 'A' && currentLine[0] <= 'Z' && currentLine[1] == '='))
		throw std::string("Invalid equation");
	if (variable.find(currentLine.substr(0, 1)) == variable.end()) {
		CreateVar(currentLine.substr(0, 1), saLine);
	}
	std::string answerVar(currentLine.substr(0,1));
	currentLine = currentLine.substr(currentLine.find_first_of('=') + 1);
	std::list<std::string> equation = PolishNotation(currentLine);
	auto iter = equation.begin();
	while (iter != equation.end()) {
		if (((*iter).size() > 1 && (*iter)[1] >= 'A' && (*iter)[1] <= 'Z')) {
			if (variable.find((*iter).substr(1, 1)) == variable.end())
				CreateVar((*iter).substr(1, 1), saLine);
		}
		else if((*iter)[0] >= 'A' && (*iter)[0] <= 'Z')
			if (variable.find(*iter) == variable.end())
				CreateVar(*iter, saLine);
		iter++;
	}
	iter = equation.begin();
	while (iter != equation.end()) {
		
		if (*iter == "+" || *iter == "-" || *iter == "*" || *iter == "/") {
			char operat = (*iter)[0];
			iter--;
			iter--;
			address = GetVarOrLiterAdress(*iter, saLine);
			if ((*iter)[0] == '_' && (*iter)[1] >='A' && (*iter)[1] <= 'Z') {
				saLine += GetStrSaLineNumber() + " LOAD 99\n";
				currSaLineNumb++;
				saLine += GetStrSaLineNumber() + " SUB " + address + "\n";
				currSaLineNumb++;
			}
			else {
				saLine += GetStrSaLineNumber() + " LOAD " + address + "\n";
				currSaLineNumb++;
			}
				
			iter = equation.erase(iter);
			address = GetVarOrLiterAdress(*iter, saLine);
			if ((*iter)[0] == '_' && (*iter)[1] >= 'A' && (*iter)[1] <= 'Z')
				if (operat == '-') operat = '+';
				else if (operat == '+') operat = '-';
			saLine += GetStrSaLineNumber() + " " + allOperat[operat] + " " + address + "\n";
			currSaLineNumb++;
			if ((*iter)[0] == '_' && (*iter)[1] >= 'A' && (*iter)[1] <= 'Z' && (operat == '*' || operat == '/')) {
				saLine += GetStrSaLineNumber() + " STORE 98\n";
				currSaLineNumb++;
				saLine += GetStrSaLineNumber() + " LOAD 99\n";
				currSaLineNumb++;
				saLine += GetStrSaLineNumber() + " SUB 98\n";
				currSaLineNumb++;
			}
			
			saLine += GetStrSaLineNumber() + " STORE "+ GetVarOrLiterAdress("#"+std::to_string(pseudoVarCount), saLine) + "\n";
			currSaLineNumb++;
			iter = equation.erase(iter);
			iter = equation.erase(iter);
			iter = equation.insert(iter, "#" + std::to_string(pseudoVarCount));
			pseudoVarCount++;
		}
		iter++;
	}
	saLine += GetStrSaLineNumber() + " STORE " + std::to_string(variable[answerVar]) + "\n";
	currSaLineNumb++;
	for (int i(0); i < pseudoVarCount; i++) {
		variable.erase("#" + std::to_string(i));
		currVarPos++;
	}
	if (saLine[saLine.size() - 1] = '\n') saLine.pop_back();
	return saLine;
}

std::string SBTranslator::GetVarOrLiterAdress(std::string numb, std::string&saLine) {
	bool isNeg(false);
	if (numb[0] == '_') {
		isNeg = true;
		numb = numb.substr(1);
	}
	if (numb[0] >= 'A' && numb[0] <= 'Z' || numb[0] == '#')
		if(variable.find(numb) != variable.end())
			return std::to_string(variable[numb]);
		else {
			variable.insert({ numb, currVarPos });
			currVarPos--;
			return std::to_string(currVarPos + 1);
		}
	else {
		std::string hex = decToHex(stoi(numb));
		hex = isNeg ? "-" + hex : "+" + hex;
		saLine += "98 = " + hex + "\n";
		return "98";
	}
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
