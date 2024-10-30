#ifndef _IMAGES_H_
#define _IMAGES_H_
/*
 * source: 3350 background framework
 * modified by: joshua garibay
 * purpose: images.h file to use in other cpp files
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>

class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image();
    Image(const char *fname);
};

#endif
