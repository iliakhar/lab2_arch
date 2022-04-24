#include"MyTerm.h"

std::string MyTerm::rk_readKeyGetch(rk::keys* key, bool isAutoEnter) {
	//muteAsync.lock();
	bool (*checkCannon)(clock_t start, double tm, int cnt, int min);
	if(isAutoEnter)
		checkCannon = [](clock_t start, double tm, int cnt, int min) {return true; };
	else
		checkCannon = [](clock_t start, double tm, int cnt,int min) {return (double)(clock() - start) / CLOCKS_PER_SEC < tm && cnt < min + 1; };
	std::string newKey;
	char ch;
	int symbCount = 0;
	while (_kbhit())getch();
	newKey.push_back(getch());
	
	if (newKey[0] == 0 || newKey[0] == -32 || newKey[0] == 224)
		newKey[0] = getch();

	switch (newKey[0]) {
	case 75: *key = rk::Left;
		return "";
	case 77: *key = rk::Right;
		return "";
	}
	if (newKey[0] != 63 && newKey[0] != 64) {
		symbCount++;
		if (termInfo.echo) {
			muteAsync.lock();
			std::cout << newKey[0];
			muteAsync.unlock();
		}
	}
	else if (newKey[0] == 63 || newKey[0] == 64) {
		symbCount++;
		if (termInfo.echo) {
			muteAsync.lock();
			if(newKey[0] == 63) std::cout << "!";
			else std::cout << "@";
			muteAsync.unlock();
		}
	}
		
	clock_t start = clock();
	while (checkCannon(start, termInfo.vtime, symbCount, termInfo.vmin)) {
		if (_kbhit()) {
			ch = getch();
			if (ch == '\r' && isAutoEnter)break;
			if (ch == '\r' && !isAutoEnter)continue;
			if (ch == '\b') {
				if (symbCount != 0) {
					symbCount--;
					newKey.pop_back();
				}
				if (termInfo.echo) {
					muteAsync.lock();
					//if (pos != 0)pos--;
					mt.gotoXY(0 + symbCount, 22);
					std::cout << " ";
					mt.gotoXY(0 + symbCount, 22);
					muteAsync.unlock();
					start = clock();
				}
				continue;
			}
			if (ch == 0 || ch == -32 || ch == 224) {
				ch = getch();
				if (ch == 63 || ch == 64) {
					newKey.push_back(ch);
					if (termInfo.echo) {
						muteAsync.lock();
						mt.gotoXY(0 + symbCount, 22);
						if(ch == 63) std::cout << "!";
						else std::cout << "@";
						mt.gotoXY(0 + symbCount, 22);
						muteAsync.unlock();
					}
					symbCount++;
				}
			}
				
			else {
				newKey.push_back(ch);
				
				if (termInfo.echo) {
					muteAsync.lock();
					mt.gotoXY(0 + symbCount, 22);
					std::cout << newKey[symbCount];
					muteAsync.unlock();
					start = clock();
				}
				symbCount++;
			}
			while (_kbhit())getch();
		}
	}
	if(symbCount == termInfo.vmin)
		Sleep(500);
	auto it = keyMap.find(newKey);
	if (it != keyMap.end())
		*key = it->second;
	else *key = rk::ERR;
	//muteAsync.unlock();
	return newKey;
}

int MyTerm::rk_myTermRegime(bool canon, int vtime, int vmin, int echo, int sigint) {
	termInfo.canon = canon;
	termInfo.vtime = vtime;
	mt.setCursorVisible(termInfo.echo);
	termInfo.vmin = vmin;
	termInfo.sigint = sigint;
	return 0;
}

int MyTerm::rk_myTermSave() {

	std::ofstream file("term.txt", std::ios::binary);
	if (!file.is_open())
		throw "Error Error Error Error ))";
	file.write((char*)&termInfo, sizeof(termInfo));
	file.close();
	return 0;
}

int MyTerm::rk_myTermRestore() {
	std::ifstream file("term.txt", std::ios::binary);
	if (!file.is_open())
		throw "Error Error Error Error ))";
	file.read((char*)&termInfo, sizeof(termInfo));
	mt.setCursorVisible(termInfo.echo);
	file.close();
	return 0;
}

int MyTerm::rk_writeAccum(std::string val) {
	
	mt.gotoXY(84, 1);
	try{
		int a = std::stoi(val);
		accum = a;
		ram.showNumInRam(a);
	}
	catch (...) {

	}
	mt.gotoXY(0, 22);
	return 0;
}