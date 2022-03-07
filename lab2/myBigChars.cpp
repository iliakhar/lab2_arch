#include "MyTerm.h"

int MyTerm::bc_box(COORD start, COORD size, std::string frameName) {
	mt_gotoXY(start.Y, start.X);
	char ch[7] = { -65,-64,-38,-39,-77,-60 };//���,��,��,���,����,���
	std::cout << ch[2];
	int halfLen = (size.X - frameName.size()) / 2;
	for (int i = 0; i < halfLen - 1; i++)
		std::cout << ch[5];
	std::cout << frameName;
	for (int i = halfLen + frameName.size(); i < size.X - 1; i++)
		std::cout << ch[5];
	std::cout << ch[0];
	for (int i = 0; i < size.Y - 2; i++) {
		mt_gotoXY(start.Y + 1 + i, start.X);
		std::cout << ch[4];
	}
	mt_gotoXY(start.Y + size.Y - 1, start.X);
	std::cout << ch[1];
	for (int i = 0; i < size.X - 2; i++)
		std::cout << ch[5];
	for (int i = 0; i < size.Y - 2; i++) {
		mt_gotoXY(start.Y + 1 + i, start.X + size.X - 1);
		std::cout << ch[4];
	}
	mt_gotoXY(start.Y + size.Y - 1, start.X + size.X - 1);
	std::cout << ch[3];
	return 0;
}

int MyTerm::bc_printBigChar(int num, COORD start, mt::colors font, mt::colors bg) {

	char symb, ch = -78;
	int val;
	std::string str;
	mt_setfgcolor(font);
	mt_setbgcolor(bg);

	mt_gotoXY(start.Y, start.X);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bc_getBigCharPos(bn[num], { (short)j, (short)i }, val);
			symb = val ? ch : ' ';
			std::cout << symb;
		}
		mt_gotoXY(start.Y + i + 1, start.X);
	}
	mt_setfgcolor(mt::LightGray);
	mt_setbgcolor(mt::Black);
	return 0;
}

int MyTerm::bc_getBigCharPos(bigNum bigN, COORD pos, int &val) {
	if (pos.Y <= 3)
		val = (bigN.first >> (pos.Y * 8 + pos.X)) & 1;
	else
		val = (bigN.second >> ((pos.Y + 4) * 8 + pos.X)) & 1;
	return 0;
}

int MyTerm::bc_setBigCharPos(bigNum &bigN, COORD pos, int val) {
	if (val == 1) {
		if (pos.Y <= 3)
			bigN.first |= 1 << (pos.Y * 8 + pos.X);
		else
			bigN.second |= 1 << ((pos.Y + 4) * 8 + pos.X);
	}
	else if (val == 0) {
		if (pos.Y <= 3)
			bigN.first &= ~(1 << (pos.Y * 8 + pos.X));
		else
			bigN.second &= ~(1 << ((pos.Y + 4) * 8 + pos.X));
	}
		
	return 0;
}

int MyTerm::bc_bigCharRead(std::string filename, int count) {

	std::ifstream data(filename, std::ios::binary);
	if (!data.is_open())
		throw "Error: file not open";
	for (int i = 0; i < count; i++) {

		data >> bn[i].first;
		data >> bn[i].second;
	}
	return 0;
}

int MyTerm::bc_printBigNumber(int num, COORD start, mt::colors font, mt::colors bg) {

	if (num >= 0)
		bc_printBigChar(16, start, font, bg);
	else
		bc_printBigChar(17, start, font, bg);
	num = abs(num);
	start.X += 9;
	for (int hexDegree = 0x1000; hexDegree != 0; hexDegree /= 16) {
		bc_printBigChar(num / hexDegree, start, font, bg);
		start.X += 9;
		//printf("%x", num / hexDegree);
		num %= hexDegree;
	}
	return 0;
}