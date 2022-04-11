#include"mem.h"

int Ram::sc_memoryInit() {
    for (int i = 0; i < 100; i++)
        arr[i] = 0;
    return 0;
}

int Ram::sc_memorySet(int adress, int value, Flag& reg) {
    if (adress > 99 || adress < 0) {
        reg.sc_regSet(M, 1);
        return 0;
    }
    else arr[adress] = value;
    return 1;
}

int Ram::sc_memoryGet(int adress, int* value, Flag& reg) {
    if (adress > 99 || adress < 0) {
        reg.sc_regSet(M, 1);
        return 0;
    }
    else *value = arr[adress];
    return 1;
}

int Ram::sc_memorySave(std::string filename) {
    FILE* saveData;
    int res;

    //unsigned int fd = open(filename.c_str(), );
    //res = write(fd, arr, sizeof(int) * 100);


    saveData = fopen(filename.c_str(), "wb");
    if (NULL == saveData)
        return 1;
    res = fwrite(arr, sizeof(arr), 1, saveData);
    //res = write(saveData, )
    fclose(saveData);
    if (res != 1)
        return 1;
    else
        return 0;
}

int Ram::sc_memoryLoad(std::string filename) {
    FILE* data;
    int res;

    data = fopen(filename.c_str(), "rb");
    if (data == NULL)
        return 1;
    res = fread(arr, sizeof(arr), 1, data);
    fclose(data);
    if (res != 1)
        return 2;
    else
        return 0;
}

int Ram::showRam(Flag &reg) {
    int emptyDigits, cell;
    std::cout << "\n  ";
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            sc_memoryGet(i * 10 + j, &cell, reg);
            showNumInRam(cell);
            std::cout << "  ";
        }
        std::cout << "\n  ";
    }
    return 0;
}

int Ram::showNumInRam(int num) {
    m.lock();
    if (num < 0) std::cout << "-";
    else std::cout << "+";
    num = abs(num);
    for (int decDegree = 0x1000; decDegree != 0; decDegree /= 16) {
        printf("%x", num / decDegree);
        num %= decDegree;
    }
    m.unlock();
    return 0;
}
