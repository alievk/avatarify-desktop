/****************************************************************************
*
* ViSP, open source Visual Servoing Platform software.
* Copyright (C) 2005 - 2019 by Inria. All rights reserved.
*
* This software is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* See the file LICENSE.txt at the root directory of this source
* distribution for additional information about the GNU GPL.
*
* For using ViSP with software that can not be combined with the GNU
* GPL, please contact Inria about acquiring a ViSP Professional
* Edition License.
*
* See http://visp.inria.fr for more information.
*
* This software was developed at:
* Inria Rennes - Bretagne Atlantique
* Campus Universitaire de Beaulieu
* 35042 Rennes Cedex
* France
*
* If you have questions regarding the use of this file, please contact
* Inria at visp@inria.fr
*
* This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
* WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* Description:
* Convert image types.
*
* Authors:
* Eric Marchand
* Fabien Spindler
* Anthony Saunier
*
*****************************************************************************/

/*!
  \file vpImageConvert.cpp
  \brief Convert image types
*/

#include <map>
#include <sstream>

#include "vpImageConvert.h"


#define vpSAT(c)                                                                                                       \
  if (c & (~255)) {                                                                                                    \
    if (c < 0)                                                                                                         \
      c = 0;                                                                                                           \
    else                                                                                                               \
      c = 255;                                                                                                         \
  }

/*!
  Convert an image from YUYV 4:2:2 (y0 u01 y1 v01 y2 u23 y3 v23 ...)
  to RGB24. Destination rgb memory area has to be allocated before.

  \sa YUV422ToRGB()
*/
void YUYVToRGB(unsigned char *yuyv, unsigned char *rgb, unsigned int width, unsigned int height) {
    unsigned char *s;
    unsigned char *d;
    int h, w;
    int r, g, b, cr, cg, cb, y1, y2;

    h = (int) height;
    w = (int) width;
    s = yuyv;
    d = rgb;
    while (h--) {
        int c = w >> 1;
        while (c--) {
            y1 = *s++;
            cb = ((*s - 128) * 454) >> 8;
            cg = (*s++ - 128) * 88;
            y2 = *s++;
            cr = ((*s - 128) * 359) >> 8;
            cg = (cg + (*s++ - 128) * 183) >> 8;

            r = y1 + cr;
            b = y1 + cb;
            g = y1 - cg;
            vpSAT(r);
            vpSAT(g);
            vpSAT(b);

            *d++ = static_cast<unsigned char>(r);
            *d++ = static_cast<unsigned char>(g);
            *d++ = static_cast<unsigned char>(b);

            r = y2 + cr;
            b = y2 + cb;
            g = y2 - cg;
            vpSAT(r);
            vpSAT(g);
            vpSAT(b);

            *d++ = static_cast<unsigned char>(r);
            *d++ = static_cast<unsigned char>(g);
            *d++ = static_cast<unsigned char>(b);
        }
    }
}


/*!
  Convert YUV 4:2:2 (u01 y0 v01 y1 u23 y2 v23 y3 ...) images into RGB images.
  Destination rgb memory area has to be allocated before.

  \sa YUYVToRGB()
*/
void YUV422ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size) {
    for (unsigned int i = size / 2; i; i--) {
        int U = (int) ((*yuv++ - 128) * 0.354);
        int U5 = 5 * U;
        int Y0 = *yuv++;
        int V = (int) ((*yuv++ - 128) * 0.707);
        int V2 = 2 * V;
        int Y1 = *yuv++;
        int UV = -U - V;

        //---
        int R = Y0 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        int G = Y0 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        int B = Y0 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;

        //---
        R = Y1 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        G = Y1 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        B = Y1 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;
    }
}

/*!
Convert YUV411 into RGB
yuv411 : u y1 y2 v y3 y4
*/
void YUV411ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size) {
    for (unsigned int i = size / 4; i; i--) {
        int U = (int) ((*yuv++ - 128) * 0.354);
        int U5 = 5 * U;
        int Y0 = *yuv++;
        int Y1 = *yuv++;
        int V = (int) ((*yuv++ - 128) * 0.707);
        int V2 = 2 * V;
        int Y2 = *yuv++;
        int Y3 = *yuv++;
        int UV = -U - V;

        // Original equations
        // R = Y           + 1.402 V
        // G = Y - 0.344 U - 0.714 V
        // B = Y + 1.772 U
        int R = Y0 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        int G = Y0 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        int B = Y0 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;

        //---
        R = Y1 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        G = Y1 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        B = Y1 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;

        //---
        R = Y2 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        G = Y2 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        B = Y2 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;

        //---
        R = Y3 + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        G = Y3 + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        B = Y3 + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;
    }
}

/*!
  Convert YUV420 [Y(NxM), U(N/2xM/2), V(N/2xM/2)] image into RGB image.
*/
void YUV420ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height) {
    int U, V, R, G, B, V2, U5, UV;
    int Y0, Y1, Y2, Y3;
    unsigned int size = width * height;
    unsigned char *iU = yuv + size;
    unsigned char *iV = yuv + 5 * size / 4;
    for (unsigned int i = 0; i < height / 2; i++) {
        for (unsigned int j = 0; j < width / 2; j++) {
            U = (int) ((*iU++ - 128) * 0.354);
            U5 = 5 * U;
            V = (int) ((*iV++ - 128) * 0.707);
            V2 = 2 * V;
            UV = -U - V;
            Y0 = *yuv++;
            Y1 = *yuv;
            yuv = yuv + width - 1;
            Y2 = *yuv++;
            Y3 = *yuv;
            yuv = yuv - width + 1;

            // Original equations
            // R = Y           + 1.402 V
            // G = Y - 0.344 U - 0.714 V
            // B = Y + 1.772 U
            R = Y0 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y0 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y0 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y1 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y1 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y1 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb + 3 * width - 5;

            //---
            R = Y2 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y2 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y2 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y3 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y3 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y3 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb - 3 * width + 1;
        }
        yuv += width;
        rgb += 3 * width;
    }
}

/*!
  Convert YUV444 (u y v) image into RGB image.
*/
void YUV444ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        int U = (int) ((*yuv++ - 128) * 0.354);
        int U5 = 5 * U;
        int Y = *yuv++;
        int V = (int) ((*yuv++ - 128) * 0.707);
        int V2 = 2 * V;
        int UV = -U - V;

        // Original equations
        // R = Y           + 1.402 V
        // G = Y - 0.344 U - 0.714 V
        // B = Y + 1.772 U
        int R = Y + V2;
        if ((R >> 8) > 0)
            R = 255;
        else if (R < 0)
            R = 0;

        int G = Y + UV;
        if ((G >> 8) > 0)
            G = 255;
        else if (G < 0)
            G = 0;

        int B = Y + U5;
        if ((B >> 8) > 0)
            B = 255;
        else if (B < 0)
            B = 0;

        *rgb++ = (unsigned char) R;
        *rgb++ = (unsigned char) G;
        *rgb++ = (unsigned char) B;
    }
}


/*!
  Convert YV12 [Y(NxM), V(N/2xM/2), U(N/2xM/2)] image into RGB image.
*/
void YV12ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int height, unsigned int width) {
    int U, V, R, G, B, V2, U5, UV;
    int Y0, Y1, Y2, Y3;
    unsigned int size = width * height;
    unsigned char *iV = yuv + size;
    unsigned char *iU = yuv + 5 * size / 4;
    for (unsigned int i = 0; i < height / 2; i++) {
        for (unsigned int j = 0; j < width / 2; j++) {
            U = (int) ((*iU++ - 128) * 0.354);
            U5 = 5 * U;
            V = (int) ((*iV++ - 128) * 0.707);
            V2 = 2 * V;
            UV = -U - V;
            Y0 = *yuv++;
            Y1 = *yuv;
            yuv = yuv + width - 1;
            Y2 = *yuv++;
            Y3 = *yuv;
            yuv = yuv - width + 1;

            // Original equations
            // R = Y           + 1.402 V
            // G = Y - 0.344 U - 0.714 V
            // B = Y + 1.772 U
            R = Y0 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y0 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y0 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y1 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y1 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y1 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb + 3 * width - 5;

            //---
            R = Y2 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y2 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y2 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y3 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y3 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y3 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb - 3 * width + 1;
        }
        yuv += width;
        rgb += 3 * width;
    }
}

/*!
  Convert YV12 [Y(NxM),  V(N/4xM/4), U(N/4xM/4)] image into RGB image.
*/
void YVU9ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int height, unsigned int width) {
    int U, V, R, G, B, V2, U5, UV;
    int Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8, Y9, Y10, Y11, Y12, Y13, Y14, Y15;
    unsigned int size = width * height;
    unsigned char *iV = yuv + size;
    unsigned char *iU = yuv + 17 * size / 16;
    for (unsigned int i = 0; i < height / 4; i++) {
        for (unsigned int j = 0; j < width / 4; j++) {
            U = (int) ((*iU++ - 128) * 0.354);
            U5 = 5 * U;
            V = (int) ((*iV++ - 128) * 0.707);
            V2 = 2 * V;
            UV = -U - V;
            Y0 = *yuv++;
            Y1 = *yuv++;
            Y2 = *yuv++;
            Y3 = *yuv;
            yuv = yuv + width - 3;
            Y4 = *yuv++;
            Y5 = *yuv++;
            Y6 = *yuv++;
            Y7 = *yuv;
            yuv = yuv + width - 3;
            Y8 = *yuv++;
            Y9 = *yuv++;
            Y10 = *yuv++;
            Y11 = *yuv;
            yuv = yuv + width - 3;
            Y12 = *yuv++;
            Y13 = *yuv++;
            Y14 = *yuv++;
            Y15 = *yuv;
            yuv = yuv - 3 * width + 1;

            // Original equations
            // R = Y           + 1.402 V
            // G = Y - 0.344 U - 0.714 V
            // B = Y + 1.772 U
            R = Y0 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y0 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y0 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y1 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y1 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y1 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y2 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y2 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y2 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y3 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y3 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y3 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb + 3 * width - 11;

            R = Y4 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y4 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y4 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y5 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y5 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y5 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y6 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y6 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y6 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y7 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y7 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y7 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb + 3 * width - 11;

            R = Y8 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y8 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y8 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y9 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y9 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y9 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y10 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y10 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y10 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y11 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y11 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y11 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb = (unsigned char) B;
            rgb = rgb + 3 * width - 11;

            R = Y12 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y12 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y12 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y13 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y13 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y13 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y14 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y14 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y14 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;

            //---
            R = Y15 + V2;
            if ((R >> 8) > 0)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y15 + UV;
            if ((G >> 8) > 0)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y15 + U5;
            if ((B >> 8) > 0)
                B = 255;
            else if (B < 0)
                B = 0;

            *rgb++ = (unsigned char) R;
            *rgb++ = (unsigned char) G;
            *rgb++ = (unsigned char) B;
            rgb = rgb - 9 * width + 1;
        }
        yuv += 3 * width;
        rgb += 9 * width;
    }
}
