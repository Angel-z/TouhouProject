#include <Windows.h>
#include <stdlib.h>

#include <vector>

#include "entity.h"
#include "globalVariable.h"

int GameTurn;
bool stage1ini = true, stage2ini = true;
bool stage2_1 = true;
std::vector<Entity *> WaitToDelete;

enum class Dir { Left = 0, Right, Up, Down, Stop = -1 };

void GameProgress() {
    if (GameTurn <= Stage1_Loop) {
        if (stage1ini) {
            // TODO
            EnemyExists.clear();
            for (auto it = WaitToDelete.begin(); it != WaitToDelete.end(); ++it) {
                delete (*it);
            }
            WaitToDelete.clear();

            player.xySet(GWidth / 2 - player.iWidth / 2, GHeight - player.iHeight / 2);
            player.getBullet()->ptPos.clear();
            stage2ini = true;
            stage2_1 = true;

            stage1ini = false;
        } else {
            if (EnemyExists.empty()) {
                for (auto it = WaitToDelete.begin(); it != WaitToDelete.end(); ++it) {
                    delete (*it);
                }
                WaitToDelete.clear();

                if (++GameTurn > Stage1_Loop) {
                    return;
                }
                // Spawn
                for (int i = 0; i < 2; ++i) {
                    Enemy1 *tmp1 = new Enemy1();
                    tmp1->xySet(275 * i + 80, 50);
                    WaitToDelete.push_back(tmp1);
                    EnemyExists.push_back(tmp1);
                    Enemy2 *tmp2 = new Enemy2();
                    tmp2->xySet(275 * i + 225, 150);
                    WaitToDelete.push_back(tmp2);
                    EnemyExists.push_back(tmp2);
                }
            } else {
                for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
                    if ((*it)->ptLeftTop.x + 9 > GWidth) {
                        (*it)->xySet(-(*it)->iWidth / 2, (*it)->iy);
                    } else {
                        (*it)->xChange(9);
                    }
                }
            }
        }
    } else if (GameTurn <= Stage2_Loop + 1) {
        if (stage2ini) {
            EnemyExists.clear();
            for (auto it = WaitToDelete.begin(); it != WaitToDelete.end(); ++it) {
                delete (*it);
            }
            WaitToDelete.clear();
            stage2_1 = true;

            stage2ini = false;
        } else {
            if (EnemyExists.empty()) {
                for (auto it = WaitToDelete.begin(); it != WaitToDelete.end(); ++it) {
                    delete (*it);
                }
                WaitToDelete.clear();

                if (++GameTurn > Stage2_Loop + 1) {
                    return;
                }

                // Spawn
                for (int i = 0; i < 4; ++i) {
                    Enemy2 *tmp2 = new Enemy2();
                    tmp2->xySet(150 * (i + 1) - GWidth, 130);
                    WaitToDelete.push_back(tmp2);
                    EnemyExists.push_back(tmp2);
                }
                for (int i = 0; i < 3; ++i) {
                    Enemy1 *tmp1 = new Enemy1();
                    tmp1->xySet(170 * (i + 1) + 40 - GWidth, 50);
                    WaitToDelete.push_back(tmp1);
                    EnemyExists.push_back(tmp1);
                }
                for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
                    (*it)->changeDir(rand() % 4);
                }
                stage2_1 = true;
            } else {
                if (stage2_1) {
                    if (EnemyExists[0]->ix > 150) {
                        stage2_1 = false;
                    } else {
                        for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
                            (*it)->xChange(10);
                        }
                    }
                } else {
                    int EnemySpeed = 4;
                    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
                        if (rand() % 500 < 10) {
                            (*it)->changeDir(rand() % 4);
                        } else {
                            Dir dir = (Dir)(*it)->getDir();
                            if (dir == Dir::Left) {
                                if ((*it)->ptLeftTop.x - EnemySpeed < 0) {
                                    (*it)->changeDir(int(Dir::Right));
                                } else {
                                    (*it)->xChange(-EnemySpeed);
                                }
                            } else if (dir == Dir::Right) {
                                if ((*it)->ptLeftTop.x + (*it)->iWidth + EnemySpeed > GWidth) {
                                    (*it)->changeDir(int(Dir::Left));
                                } else {
                                    (*it)->xChange(EnemySpeed);
                                }
                            } else if (dir == Dir::Up) {
                                if ((*it)->ptLeftTop.y - EnemySpeed < 0) {
                                    (*it)->changeDir(int(Dir::Down));
                                } else {
                                    (*it)->yChange(-EnemySpeed);
                                }
                            } else if (dir == Dir::Down) {
                                if ((*it)->ptLeftTop.y + (*it)->iHeight + EnemySpeed > GHeight / 2) {
                                    (*it)->changeDir(int(Dir::Up));
                                } else {
                                    (*it)->yChange(EnemySpeed);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        win = true;
        fail = false;
        running = false;
    }
}