#pragma once

#include <stdio.h>
#include <iostream>

typedef struct CHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tCHS;

typedef struct ECHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tLARGE;

typedef struct IDECHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tIDECHS;

typedef struct LBA {
    unsigned int lba;
} tLBA;

typedef struct PART {
    int activ;
    tCHS beg;
    tCHS end;
    tLBA lba_beg;
    int size;
} tPART;

class Disk {
    int selectSize;
    int count;
    double disk;
    
public:
    Disk(double dsk):count(0),disk(dsk), selectSize(512) {}
    void show_partitions(tPART* part);
    void enter_partitions(tPART* part, tCHS geo, int num_parts);
    void setDisk(double dsk) { disk = dsk; }
    double getDisk() { return disk; }

    int g_lba2chs(tLBA orig, tCHS* dest);
    int g_lba2large(tLBA orig, tLARGE* dest);
    int g_lba2idechs(tLBA orig, tIDECHS* dest);
    int g_chs2large(tCHS orig, tLARGE* dest);
    int g_chs2idechs(tCHS orig, tIDECHS* dest);
    int g_chs2lba(tCHS orig, tLBA* dest);
    int g_large2chs(tLARGE orig, tCHS* dest);
    int g_large2idechs(tLARGE orig, tIDECHS* dest);
    int g_large2lba(tLARGE orig, tLBA* dest);
    int g_idechs2chs(tIDECHS orig, tCHS* dest);
    int g_idechs2large(tIDECHS orig, tLARGE* dest);
    int g_idechs2lba(tIDECHS orig, tLBA* dest);
    int a_lba2chs(tCHS geometry, tLBA orig, tCHS* dest);
};


