#include "Disk.h"
int Disk::g_lba2chs(tLBA orig, tCHS* dest)
{
    dest->cyl = orig.lba / 15 / 63;
    dest->head = orig.lba / 15 % 63;
    dest->sec = orig.lba % 63 + 1;
    return 0;
}

int Disk::g_lba2large(tLBA orig, tLARGE* dest)
{
    dest->cyl = orig.lba / 63 / 255;
    dest->head = orig.lba / 63 % 255;
    dest->sec = orig.lba % 255 + 1;
    return 0;
}

int Disk::g_lba2idechs(tLBA orig, tIDECHS* dest)
{
    dest->cyl = orig.lba / 16 / 256;
    dest->head = orig.lba / 16 % 256;
    dest->sec = orig.lba % 256 + 1;
    return 0;
}

int Disk::g_chs2large(tCHS orig, tLARGE* dest)
{
    tLBA tmp;
    g_chs2lba(orig, &tmp);
    g_lba2large(tmp, dest);
    return 0;
}

int Disk::g_chs2idechs(tCHS orig, tIDECHS* dest)
{
    tLBA tmp;
    g_chs2lba(orig, &tmp);
    g_lba2idechs(tmp, dest);
    return 0;
}

int Disk::g_chs2lba(tCHS orig, tLBA* dest)
{
    dest->lba = (orig.head + 15 * orig.cyl) * 63 + orig.sec - 1;
    return 0;
}

int Disk::g_large2chs(tLARGE orig, tCHS* dest)
{
    tLBA tmp;
    g_large2lba(orig, &tmp);
    g_lba2chs(tmp, dest);
    return 0;
}

int Disk::g_large2idechs(tLARGE orig, tIDECHS* dest)
{
    tLBA tmp;
    g_large2lba(orig, &tmp);
    g_lba2idechs(tmp, dest);
    return 0;
}

int Disk::g_large2lba(tLARGE orig, tLBA* dest)
{
    dest->lba = (orig.head + 256 * orig.cyl) * 63 + orig.sec - 1;
    return 0;
}

int Disk::g_idechs2chs(tIDECHS orig, tCHS* dest)
{
    tLBA tmp;
    g_idechs2lba(orig, &tmp);
    g_lba2chs(tmp, dest);
    return 0;
}

int Disk::g_idechs2large(tIDECHS orig, tLARGE* dest)
{
    tLBA tmp;
    g_idechs2lba(orig, &tmp);
    g_lba2large(tmp, dest);
    return 0;
}

int Disk::g_idechs2lba(tIDECHS orig, tLBA* dest)
{
    dest->lba = (orig.head + 15 * orig.cyl) * 255 + orig.sec - 1;
    return 0;
}

int Disk::a_lba2chs(tCHS geometry, tLBA orig, tCHS* dest)
{
    int tmp;
    dest->cyl = (orig.lba / geometry.sec) / geometry.head;
    if (dest->cyl > geometry.cyl) {
        dest->cyl = geometry.cyl;
        dest->head = geometry.head;
        dest->sec = geometry.sec;
        return 0;
    }
    tmp = orig.lba % (geometry.head * geometry.sec);
    dest->head = tmp / geometry.sec;
    dest->sec = tmp % geometry.sec + 1;
    return 0;
}


void Disk::enter_partitions(tPART* part, tCHS geo, int num_parts){
    int activ = 0;

    for (int i = 0; i < num_parts; i++) {
        if (!activ)
        {
            printf("is this part is active? (y\\n): ");
            char ans[3];
            std::cin >> ans;
            if (ans[0] == 'y') {
                part[i].activ = 1;
                activ++;
            }
        }

        if (i == 0)
            part[i].lba_beg.lba = 1;
        else {
            tLBA lba_tmp = { part[i - 1].lba_beg.lba + part[i - 1].size };
            part[i].lba_beg = lba_tmp;
        }

        printf("enter size: ");
        std::cin >> part[i].size;
        if (part[i].size == 0)
        {
            count++;
            part[i].size = disk;
            a_lba2chs(geo, part[i].lba_beg, &part[i].beg);
            tLBA lba_tmp = { part[i].lba_beg.lba + part[i].size - 1 };
            a_lba2chs(geo, lba_tmp, &part[i].end);
            break;
        }
        disk -= part[i].size;
        count++;
        if (disk <= 0)
        {
            break;
        }
        a_lba2chs(geo, part[i].lba_beg, &part[i].beg);
        tLBA lba_tmp = { part[i].lba_beg.lba + part[i].size - 1 };
        a_lba2chs(geo, lba_tmp, &part[i].end);
    }
    if (activ == 0)
        part[count - 1].activ = 1;
}

void Disk::show_partitions(tPART* part)
{

    for (int i = 0; i < count; i++) {

        printf("%d", part[i].activ);
        printf("\t");

        printf("%d-%d-%d", part[i].beg.cyl, part[i].beg.head, part[i].beg.sec);
        printf("\t");


        printf("%d-%d-%d", part[i].end.cyl, part[i].end.head, part[i].end.sec);
        printf("\t");

        printf("%d", part[i].lba_beg.lba);
        printf("\t");

        printf("%d", part[i].size);
        printf("\t");

        printf("\n");

    }
}