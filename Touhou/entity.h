#pragma once
#include <Windows.h>
#include <atlimage.h>

#include <vector>

class Bullet;

class Entity {
   public:
    int ix;
    int iy;
    int iFileWidth;
    int iFileHeight;
    int iWidth;
    int iHeight;
    int iRadius;

    POINT ptLeftTop;

   public:
    Entity(int _x, int _y, int _iFileWidth, int _iFileHeight, int _iRadius);

    virtual void draw(HDC hdc, int x, int y);
    virtual void bulletStatusChange(bool stat);
    virtual Bullet *getBullet();
    virtual void bulletSpawn();
    virtual void bulletMoving();

    void xChange(int x);
    void yChange(int y);
};

class Bullet : public Entity {
   public:
    CImage &ci;
    bool bulletUsable;
    ULONGLONG msBulletCold;
    ULONGLONG msLastShoot;

    std::vector<POINT> ptPos;

   public:
    Bullet(CImage &_ci, int _iFileWidth, int _iFileHeight, int _msBulletCold);

    void draw(HDC hdc);
};

class Player : public Entity {
   public:
    static CImage &ci;

    int iUpDownIndex;
    int iLeftIndex;
    int iRightIndex;

    Bullet BulletPlayer;

   public:
    Player();

    void draw(HDC hdc, int x, int y) override;
    void bulletStatusChange(bool stat) override;
    Bullet *getBullet() override;
    void bulletSpawn() override;
    void bulletMoving() override;
};
