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
    int iFileRadius;
    int iRadius;

    bool bCanBeDeleted;

    POINT ptLeftTop;

   public:
    Entity(int _x, int _y, int _iFileWidth, int _iFileHeight, int _iFileRadius);

    virtual void draw(HDC hdc);
    virtual void bulletStatusChange(bool stat);
    virtual Bullet *getBullet();
    virtual void bulletSpawn();
    virtual void bulletMoving();
    virtual bool isDead();
    virtual void changeDead();
    virtual int getHealth();
    virtual void changeHealth(int health);
    virtual void changeDir(int _dir);
    virtual int getDir();

    void xChange(int x);
    void yChange(int y);
    void xySet(int x, int y);
};

class Bullet : public Entity {
   public:
    CImage &ci;
    bool bulletUsable;
    ULONGLONG msBulletCold;
    ULONGLONG msLastShoot;

    std::vector<POINT> ptPos;

   public:
    Bullet(CImage &_ci, int _iFileWidth, int _iFileHeight, int _msBulletCold, int _iFileRadius);

    void draw(HDC hdc) override;
};

class Player : public Entity {
   public:
    static CImage &ci;

    int iUpDownIndex;
    int iLeftIndex;
    int iRightIndex;

    Bullet BulletPlayer;
    int BulletDamage;

   public:
    Player();

    void draw(HDC hdc) override;
    void bulletStatusChange(bool stat) override;
    Bullet *getBullet() override;
    void bulletSpawn() override;
    void bulletMoving() override;
};

class Enemy1 : public Entity {
   public:
    static CImage &ci;

    int iHealth;
    bool bDead;
    int dir;

    Bullet BulletEnemy;

   public:
    Enemy1();

    void draw(HDC hdc) override;
    void bulletStatusChange(bool stat) override;
    Bullet *getBullet() override;
    void bulletSpawn() override;
    void bulletMoving() override;
    bool isDead() override;
    void changeDead() override;
    int getHealth() override;
    void changeHealth(int health) override;
    void changeDir(int _dir) override;
    int getDir() override;
};

class Enemy2 : public Entity {
   public:
    static CImage &ci;

    int iHealth;
    bool bDead;
    int dir;

    Bullet BulletEnemy;

   public:
    Enemy2();

    void draw(HDC hdc) override;
    void bulletStatusChange(bool stat) override;
    Bullet *getBullet() override;
    void bulletSpawn() override;
    void bulletMoving() override;
    bool isDead() override;
    void changeDead() override;
    int getHealth() override;
    void changeHealth(int health) override;
    void changeDir(int _dir) override;
    int getDir() override;
};