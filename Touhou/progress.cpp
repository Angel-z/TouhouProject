#include <Windows.h>

#include <vector>

#include "entity.h"
#include "globalVariable.h"

ULONGLONG tGameStart, tGameNow;
bool stage1ini = false, stage2ini = false, stage3ini = false, stage4ini = false;
std::vector<Entity *> WaitToDelete;

void GameProgress() {
    tGameNow = GetTickCount64() - tGameStart;
    if (tGameNow < Stage1_Time) {
        if (EnemyExists.empty()) {
            for (auto it = WaitToDelete.begin(); it != WaitToDelete.end(); ++it) {
                delete (*it);
            }
            WaitToDelete.clear();
            for (int i = 0; i < 5; ++i) {
                Enemy1 *tmp1 = new Enemy1();
                tmp1->xySet(125 * i + 80, 50);
                WaitToDelete.push_back(tmp1);
                EnemyExists.push_back(tmp1);
                Enemy2 *tmp2 = new Enemy2();
                tmp2->xySet(125 * i + 150, 150);
                WaitToDelete.push_back(tmp2);
                EnemyExists.push_back(tmp2);
            }
        } else {
            //
        }
    }
}