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
  \file vpImageConvert.h
  \brief Convert image types
*/

#ifndef vpIMAGECONVERT_H
#define vpIMAGECONVERT_H

#include <stdint.h>

#if defined(_WIN32)
// Include WinSock2.h before windows.h to ensure that winsock.h is not
// included by windows.h since winsock.h and winsock2.h are incompatible
#include <WinSock2.h>
#include <windows.h>

#endif

/*!
  \class vpImageConvert

  \ingroup group_core_image

  Convert image types.

  The following example available in tutorial-image-converter.cpp shows how to
  convert an OpenCV cv::Mat image into a vpImage:

  \include tutorial-image-converter.cpp

*/

void YUYVToRGB(unsigned char *yuyv, unsigned char *rgb, unsigned int width, unsigned int height);

void YUV411ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size);

void YUV422ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size);

void YUV420ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

void YUV444ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int size);

void YV12ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

void YVU9ToRGB(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

#endif
