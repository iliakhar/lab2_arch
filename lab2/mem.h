#pragma once
#pragma warning(disable : 4996)
#include<string>
#include <mutex>
#include <iostream>


#define P 1		//переаолнение при выполнении операции
#define O 2		//ошибка деления на 0
#define M 4		//ошибка выхода за границы памяти
#define T 8		//игнорирование тактовых импульсов
#define E 16	//указана неверная команда

class Flag {
	int flag;
public:
	Flag() {
		sc_regInit();
	}
	Flag(const Flag& reg) :flag(reg.flag) {};
	int sc_regInit();
	int sc_regSet(int reg, int value);
	int sc_regGet(int reg, int* value);

	int showFlags();

};

class Ram {
	int arr[100];
	//Flag reg;
	std::mutex m;
public:
	Ram() {
		sc_memoryInit();
	}
	int sc_memoryInit();
	int sc_memorySet(int adress, int value, Flag &reg);
	int sc_memoryGet(int adress, int* value, Flag &reg);
	int sc_memorySave(std::string filename);
	int sc_memoryLoad(std::string filename);

	int showRam(Flag &reg);
	int showNumInRam(int num);

};

class Operation {
	const int size;
	const int operations[38] = {
		0x10, 0x11,
		0x20, 0x21,
		0x30, 0x31, 0x32 , 0x33,
		0x40, 0x41, 0x42, 0x43,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
		0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
	};
	int searchInOper(int command);
public:
	Operation() :size(39) {};
	int sc_commandEncode(int command, int operand, int* value, Flag& reg);
	int sc_commandDecode(int value, int* command, int* operand, Flag& reg);
};