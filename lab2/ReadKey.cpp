#include"MyTerm.h"

int MyTerm::rk_readKeyGetch(rk::keys* key) {
	std::string newKey;
	char ch;
	int symbCount = 0;
	while (_kbhit())getch();
	newKey.push_back(getch());
	if (newKey[0] == 0 || newKey[0] == -32 || newKey[0] == 224)
		newKey[0] = getch();

	switch (newKey[0]) {
	case 75: *key = rk::Left;
		return 0;
	case 77: *key = rk::Right;
		return 0;
	}
	if (termInfo.echo)
		std::cout << newKey[0];
	clock_t start = clock();
	while ((double)(clock() - start) / CLOCKS_PER_SEC < termInfo.vtime && symbCount < termInfo.vmin - 1) {
		if (_kbhit()) {
			ch = getch();
			if (ch == 0 || ch == -32 || ch == 224)
				getch();
			else {
				newKey.push_back(ch);
				symbCount++;
				if (termInfo.echo)
					std::cout << newKey[symbCount];
			}
			while (_kbhit())getch();
		}
	}
	if(symbCount == termInfo.vmin - 1)
		Sleep(termInfo.vtime * 1000);
	auto it = keyMap.find(newKey);
	if (it != keyMap.end())
		*key = it->second;
	else *key = rk::ERR;
	return 0;
}

int MyTerm::rk_readKeyCin(rk::keys* key) {
	std::string newKey;
	std::cin >> newKey;
	auto it = keyMap.find(newKey);
	if (it != keyMap.end())
		*key = it->second;
	else *key = rk::ERR;
	return 0;
}

int MyTerm::rk_myTermRegime(bool canon, int vtime, int vmin, int echo, int sigint) {
	termInfo.canon = canon;
	termInfo.vtime = vtime;
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
	file.close();
	return 0;
}