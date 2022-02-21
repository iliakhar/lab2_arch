#include "mem.h"

int Operation::searchInOper(int command) {
	for (int i = 0; i < size; i++) {
		if (command == operations[i])
			return 1;
	}
	return 0;
}

int Operation::sc_commandEncode(int command, int operand, int* value, Flag& reg) {
	*value = 0;
	if (searchInOper(command))
		return *value = (command << 7) | operand;
	else {
		reg.sc_regSet(E, 1);
		return 1;
	}
}

int Operation::sc_commandDecode(int value, int* command, int* operand, Flag& reg) {
	*command = 0;
	*operand = 0;
	int tmp_comm, tmp_oper;
	int sign;
	sign = (value >> 14) & 1;

	if (sign)
		return 1;
	tmp_comm = (value >> 7) & 0x7F;
	tmp_oper = value & 0x7F;
	if (searchInOper(tmp_comm)) {
		*command = tmp_comm;
		*operand = tmp_oper;
	}
	else {
		reg.sc_regSet(E, 1);
		return 1;
	}
	return 0;
}