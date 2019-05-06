/**************************************************************************************************/
/* FileIO.h: header file for I/O module	                                                          */ 
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/

#ifndef FILE_IO_H
#define FILE_IO_H

#include "Image.h"

/* read image from a file */
/* the size of the image needs to be pre-set */
/* the memory spaces of the image will be allocated in this function */
/* return values: */
/* NULL: fail to load or create an image */
/* image: load or create an image successfully */
IMAGE *ReadImage(const char *fname);

/* save a processed image */
/* return values: */
/* 0: successfully saved the image */
/* 1: Cannot open the file for writing */
/* 2: File error while writing to file */
int SaveImage(IMAGE *image, const char *fname);

#endif
