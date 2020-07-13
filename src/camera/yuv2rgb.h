#ifndef YUV_TO_RGB
#define YUV_TO_RGB

bool nv21_to_rgb(unsigned char *rgb, unsigned char const *nv21, int width, int height);

bool nv12_to_rgb(unsigned char *rgb, unsigned char const *nv12, int width, int height);

#endif