#include "tools.h"

#include <atlimage.h>

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