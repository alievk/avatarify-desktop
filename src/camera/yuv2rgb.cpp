#include "yuv2rgb.h"

//------------------------------------------------------------------------------
template<typename trait>
bool decode_yuv(unsigned char *out, unsigned char const *yuv, int width, int height, unsigned char alpha = 0xff) {
    // pre-condition : width and height must be even
    if (0 != (width & 1) || width < 2 || 0 != (height & 1) || height < 2 || !out || !yuv)
        return false;

    unsigned char *dst0 = out;

    unsigned char const *y0 = yuv;
    unsigned char const *uv = yuv + (width * height);
    int const halfHeight = height >> 1;
    int const halfWidth = width >> 1;

    int Y00, Y01, Y10, Y11;
    int V, U;
    int tR, tG, tB;
    for (int h = 0; h < halfHeight; ++h) {
        unsigned char const *y1 = y0 + width;
        unsigned char *dst1 = dst0 + width * trait::bytes_per_pixel;
        for (int w = 0; w < halfWidth; ++w) {
            // shift
            Y00 = (*y0++) - 16;
            Y01 = (*y0++) - 16;
            Y10 = (*y1++) - 16;
            Y11 = (*y1++) - 16;

            // U,V or V,U? our trait will make the right call
            trait::loadvu(U, V, uv);

            // temps
            Y00 = (Y00 > 0) ? (298 * Y00) : 0;
            Y01 = (Y01 > 0) ? (298 * Y01) : 0;
            Y10 = (Y10 > 0) ? (298 * Y10) : 0;
            Y11 = (Y11 > 0) ? (298 * Y11) : 0;
            tR = 128 + 409 * V;
            tG = 128 - 100 * U - 208 * V;
            tB = 128 + 516 * U;

            // 2x2 pixels result
            trait::store_pixel(dst0, Y00 + tR, Y00 + tG, Y00 + tB, alpha);
            trait::store_pixel(dst0, Y01 + tR, Y01 + tG, Y01 + tB, alpha);
            trait::store_pixel(dst1, Y10 + tR, Y10 + tG, Y10 + tB, alpha);
            trait::store_pixel(dst1, Y11 + tR, Y11 + tG, Y11 + tB, alpha);
        }
        y0 = y1;
        dst0 = dst1;
    }
    return true;
}

//------------------------------------------------------------------------------
class NV21toRGB {
public:
    enum {
        bytes_per_pixel = 3
    };

    static void loadvu(int &U, int &V, unsigned char const *&uv) {
        V = (*uv++) - 128;
        U = (*uv++) - 128;
    }

    static void store_pixel(unsigned char *&dst, int iR, int iG, int iB, unsigned char/*alpha*/) {
        *dst++ = (iR > 0) ? (iR < 65535 ? (unsigned char) (iR >> 8) : 0xff) : 0;
        *dst++ = (iG > 0) ? (iG < 65535 ? (unsigned char) (iG >> 8) : 0xff) : 0;
        *dst++ = (iB > 0) ? (iB < 65535 ? (unsigned char) (iB >> 8) : 0xff) : 0;
    }
};

bool nv21_to_rgb(unsigned char *rgb, unsigned char const *nv21, int width, int height) {
    return decode_yuv<NV21toRGB>(rgb, nv21, width, height);
}

//------------------------------------------------------------------------------
class NV12toRGB {
public:
    enum {
        bytes_per_pixel = 3
    };

    static void loadvu(int &U, int &V, unsigned char const *&uv) {
        U = (*uv++) - 128;
        V = (*uv++) - 128;
    }

    static void store_pixel(unsigned char *&dst, int iR, int iG, int iB, unsigned char/*alpha*/) {
        *dst++ = (iR > 0) ? (iR < 65535 ? (unsigned char) (iR >> 8) : 0xff) : 0;
        *dst++ = (iG > 0) ? (iG < 65535 ? (unsigned char) (iG >> 8) : 0xff) : 0;
        *dst++ = (iB > 0) ? (iB < 65535 ? (unsigned char) (iB >> 8) : 0xff) : 0;
    }
};

bool nv12_to_rgb(unsigned char *rgb, unsigned char const *nv21, int width, int height) {
    return decode_yuv<NV12toRGB>(rgb, nv21, width, height);
}