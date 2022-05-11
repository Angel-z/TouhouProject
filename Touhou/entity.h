#pragma once
#include <Windows.h>
#include <atlimage.h>

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
    Entity();
    Entity(int _x, int _y, int _iFileWidth, int _iFileHeight, int _iRadius);

    virtual void draw(HDC hdc, int x, int y);
};



class Player : public Entity {
   public:
    static CImage &ci;

    int iUpDownIndex;
    int iLeftIndex;
    int iRightIndex;

   public:
    Player();

    void draw(HDC hdc, int x, int y) override;
    void xChange(int x);
    void yChange(int y);

};
