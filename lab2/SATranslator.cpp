#include "Translator.h"

void SAtranslator(std::string safilename, std::string ofilename) {
	try {
		Operation oper;
		Flag reg;
		std::map<std::string, int> commandMap = {
		{"READ", 0x10}, {"WRITE", 0x11}, {"LOAD", 0x20}, {"STORE", 0x21}, {"ADD", 0x30}
		, {"SUB", 0x31}, {"DIVIDE", 0x32}, {"MUL", 0x33}, {"=", 0x34}, {"JUMP", 0x40},
			{"JNEG", 0x41}, {"JZ", 0x42}, {"HALT", 0x43}, {"JNS", 0x55}, {"MOVCA", 0x73}
		};
		std::ifstream safile(safilename);
		std::ofstream ofile(ofilename, std::ios::binary);
		std::string tmp, commandName;
		int operand(0), command, placeInRam, tmpSize, comAndOper;
		bool isHalt(false);
		while (std::getline(safile, tmp)) {
			if(isHalt) throw std::string("\nHALT error\n");
			tmpSize = tmp.size();
			if (tmp[0] <= '9' && tmp[0] >= '0' && tmp[1] <= '9' && tmp[1] >= '0' && tmp[2] == ' ')
				placeInRam = (tmp[0] - '0') * 10 + tmp[1] - '0';
			else throw std::string("\nInvalid placeInRam format\n");
			tmp.erase(0, tmp.find_first_of(' ') + 1);

			commandName = tmp.substr(0, tmp.find_first_of(' '));
			if (commandMap.find(commandName) != commandMap.end())
				command = commandMap[commandName];
			else throw std::string("\nInvalid command format\n");
			tmp.erase(0, tmp.find_first_of(' ') + 1);

			bool isCorrectComment = true;
			for (int i(command == 0x34 ? 5 : 2); i < tmp.size(); i++)
				if (tmp[i] != ' ' && tmp[i] != ';') isCorrectComment = false;
				else if (tmp[i] == ';') break;
			if (command != 0x34) {
				if (tmp[0] <= '9' && tmp[0] >= '0' && tmp[1] <= '9' && tmp[1] >= '0' && isCorrectComment)
					operand = (tmp[0] - '0') * 10 + tmp[1] - '0';
				else throw std::string("\nInvalid operand format\n");
				oper.sc_commandEncode(command, operand, &comAndOper, reg);
				if (command == 0x43)isHalt = true;
			}
			else if (isCorrectComment && tmp.size() >= 5) {
				if(!hexToDec(tmp.substr(1, 4),comAndOper))
					throw std::string("\nInvalid operand format\n");
				if (tmp[0] == '-')comAndOper *= -1;
			}
			else throw std::string("\nInvalid operand format\n");

			//std::cout << placeInRam << "\t#" << commandName << " ( " << command << " )\t" << operand << "\n";
			
			ofile.write(reinterpret_cast<char*>(&placeInRam), sizeof(int));
			ofile.write(reinterpret_cast<char*>(&comAndOper), sizeof(int));
		}
	}
	catch (std::string strErr) {
		std::cout << strErr;
		exit(1);
	}
	catch (...) {
		std::cout << "\nInvalid format\n";
		exit(1);
	}

}
bool hexToDec(std::string hex, int& dec) {
	dec = 0;
	if (hex.size() != 4) return false;
	int hexDegree = 4096;
	for (int i(0); i < 4; i++) {
		if (hex[i] <= '9' && hex[i] >= '0')
			dec += (hex[i] - '0') * hexDegree;
		else if (hex[i] <= 'f' && hex[i] >= 'a')
			dec += (hex[i] - 'a' + 10) * hexDegree;
		else return false;
		hexDegree /= 16;
	}
	return true;
}

//ab + c / 
//(a + b / d) / c
// abd/+c/

//abс /+ 
//a + b / c
//-a  _a
//-A, (-A+2)
//A+-B=>A+_B
std::list<std::string> PolishNotation(std::string equation) {
	std::string dop, s;
	std::list<std::string> finalEquation;
	std::string symbols[2] = { "+-*/", "0011"};
	try {
		if (equation[0] == '-')
			equation[0] = '_';
		for (int i(1); i < equation.size(); i++)
			if (equation[i] == '-' && !((equation[i - 1] >= 'A' && equation[i - 1] <= 'Z') || (equation[i - 1] >= '0'
				&& equation[i - 1] <= '9') || equation[i - 1] == ')'))
				equation[i] = '_';
		//std::cout << equation << "\n";
		if (!checkEqationFormat(equation))
			throw std::string("Invalid equation format");

		
		for (int i(0); i < equation.size(); i++) {
			
			if (symbols[0].find(equation[i]) != std::string::npos) {
				if (!s.empty()) {
					finalEquation.push_back(s);
					s.clear();
				}
				//a*b*c-3*2 => abc**32*-
				while (!dop.empty() && dop[dop.size() - 1] != '(' && symbols[1][symbols[0].find(equation[i])] < symbols[1][symbols[0].find(dop[dop.size() - 1])]) {
					s.push_back(dop[dop.size() - 1]);
					finalEquation.push_back(s);
					s.clear();
					dop.pop_back();
				}
				dop.push_back(equation[i]);
			}
			else if (equation[i] == '(') {
				dop.push_back(equation[i]);
			}
			else if (equation[i] == ')') {
				if (!s.empty()) {
					finalEquation.push_back(s);
					s.clear();
				}
				while (dop[dop.size() - 1] != '(') {
					s.push_back(dop[dop.size() - 1]);
					finalEquation.push_back(s);
					s.clear();
					dop.pop_back();
				}
				dop.pop_back();
			}
			else s.push_back(equation[i]);
			
		}
		if (!s.empty()) {
			finalEquation.push_back(s);
			s.clear();
		}
		s.push_back(' ');
		if (!dop.empty())
			for (int i(dop.size() - 1); i >= 0; i--) {
				s[0] = dop[i];
				finalEquation.push_back(s);
			}
	}
	catch (std::string strErr) {
		std::cout << strErr;
		exit(1);
	}
	
	for (auto iter = finalEquation.begin(); iter != finalEquation.end(); iter++)
		std::cout << *iter << " ";
	return finalEquation;
}

bool checkEqationFormat(std::string equation) {
	int bracket(0);
	std::string symbols = "()+-_*/";

	// проверка на первое корректное значение
	if (!((equation[0] >= 'A' && equation[0] <= 'Z') || (equation[0] >= '0' && equation[0] <= '9') || equation[0] == '(' || equation[0] == '_'))
		return false;

	// проверка последнего значения на символы 
	if (symbols.find(equation[equation.size() - 1]) != std::string::npos && equation[equation.size() - 1] != ')')
		return false;


	for (int i(0); i < equation.size() - 1; i++) {

		//Проверка на переменные, числа и операторы
		if (!((equation[i] >= 'A' && equation[i] <= 'Z') || (equation[i] >= '0' && equation[i] <= '9') || symbols.find(equation[i]) != std::string::npos))
			return false;

		//Проверка на символ после цифр
		if (equation[i] >= '0' && equation[i] <= '9' && (equation[i + 1] >= 'A' && equation[i + 1] <= 'Z' ||
			equation[i + 1] == '(' || equation[i + 1] == '_'))
			return false;

		//Проверка на символ после переменных
		if (equation[i] >= 'A' && equation[i] <= 'Z' && (equation[i + 1] >= '0' && equation[i + 1] <= '9' || equation[i + 1] >= 'A' && equation[i + 1] <= 'Z' ||
			equation[i + 1] == '_' || equation[i + 1] == '('))
			return false;

		//Если произошли не состыковачки в плюс, удачки, выходим братва
		if (bracket < 0) return false;

		//Только цифо(е)рки (Илья сказал, "Как хочешь..."), да буковки подходят
		if (equation[i] == '_' && !((equation[i + 1] >= 'A' && equation[i + 1] <= 'Z') || (equation[i + 1] >= '0' && equation[i + 1] <= '9')))
			return false;

		//После "(", "+-*/)" - это не ставить
		if (equation[i] == '(')
			if (equation[i + 1] == '+' || equation[i + 1] == '-' || equation[i + 1] == '*' || equation[i + 1] == '/' || equation[i + 1] == ')')
				return false;
		//Подсчёт открывающихся скобок
			else bracket++;

		//После ")"  цифо(е)рки (Илья сказал, "Как хочешь...", буковки, да "_" - не ставить
		if (equation[i] == ')')
			if (equation[i + 1] >= 'A' && equation[i + 1] <= 'Z' || equation[i + 1] >= '0' && equation[i + 1] <= '9' || equation[i + 1] == '_')
				return false;
		//Подсчёт закрывающихся скобок
			else bracket--;

		//После операндов только цифо(е)рки (Илья сказал, "Как хочешь..."), буковки, "_", да "("
		if ((equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/') &&
			!((equation[i + 1] >= 'A' && equation[i + 1] <= 'Z') || (equation[i + 1] >= '0' && equation[i + 1] <= '9') || equation[i + 1] == '_' || equation[i + 1] == '('))
			return false;
	}

	// Если, ребятки, закрываемся в конце, то всё чики-пуки (1)
	if (equation[equation.size() - 1] == ')') bracket--;
	else if (equation[equation.size() - 1] == '(') return false;

	//Ссылкаемся на (1)
	if (bracket == 0)
		return true;
	return false;
}
