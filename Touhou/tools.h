#pragma once
#include <atlimage.h>

bool LoadImg(CImage& image, UINT uResID);

bool C2C(int circle1x, int circle1y, int circle1r, int circle2x, int circle2y, int circle2r);

bool C2R(int cx, int cy, int cr, int rx, int ry, int rw, int rh);

bool R2R(int r1x, int r1y, int r1w, int r1h, int r2x, int r2y, int r2w, int r2h);

void LoadImageFromResource(CImage& img, HINSTANCE hInst, UINT uResID, LPCTSTR pszResType);

void LoadImageFromResource(CImage& img, HINSTANCE hInst, LPCTSTR pszResName, LPCTSTR pszResType);