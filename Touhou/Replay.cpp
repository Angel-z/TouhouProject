#include <Windows.h>

#include <fstream>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

// 调用前先删除已存在文件
void Record() {
    std::fstream file;

    file.open("record.dat", std::ios::out | std::ios::app);
    file << bPLeft << " " << bPRight << " " << bPUp << " " << bPDown << " " << bShift << " " << bPShoot << " "
         << win << "\n";
    file.close();
}

void Replay() {
}