#include "tools.h"

#include <atlimage.h>
#include <math.h>

bool LoadImg(CImage &image, CString &strPath) {
    image.Load(strPath);

    if (image.IsNull()) {
        return false;
    }

    // 判断是否需要透明显示，并做相应处理
    if (image.GetBPP() == 32) {
        for (int i = 0; i < image.GetWidth(); ++i) {
            for (int j = 0; j < image.GetHeight(); ++j) {
                byte *pByte = (byte *)image.GetPixelAddress(i, j);
                pByte[0] = pByte[0] * pByte[3] / 255;
                pByte[1] = pByte[1] * pByte[3] / 255;
                pByte[2] = pByte[2] * pByte[3] / 255;
            }
        }
    }

    return true;
}

bool C2C(int circle1x, int circle1y, int circle1r, int circle2x, int circle2y, int circle2r) {
    double dis = sqrt((circle1x - circle2x) * (circle1x - circle2x) + (circle1y - circle2y) * (circle1y - circle2y));
    if (dis - double(circle1r + circle2r) < 1e-7) {
        return true;
    } else {
        return false;
    }
}

bool C2R(int cx, int cy, int cr, int rx, int ry, int rw, int rh) {
    double x = cx - rx;
    double y = cy - ry;

    double minx = min(x, rw / 2.0);
    double maxx = max(minx, -(rw / 2.0));
    double miny = min(y, rh / 2.0);
    double maxy = max(miny, -(rh / 2.0));

    if ((maxx - x) * (maxx - x) + (maxy - y) * (maxy - y) <= double(cr * cr)) {
        return true;
    } else {
        return false;
    }
}

bool R2R(int r1x, int r1y, int r1w, int r1h, int r2x, int r2y, int r2w, int r2h) {
    bool flag = false;

    if (r1x >= r2x && r2x <= r1x - r1w / 2.0 - r2w / 2.0) {
        flag = false;
    } else if (r1x <= r2x && r2x >= r1x + r1w / 2.0 + r2w / 2.0) {
        flag = false;
    } else if (r1y >= r2y && r2y <= r1y - r1h / 2.0 - r2h / 2.0) {
        flag = false;
    } else if (r1y <= r2y && r2y >= r1y + r1h / 2.0 + r2h / 2.0) {
        flag = false;
    } else {
        flag = true;
    }

    return flag;
}
