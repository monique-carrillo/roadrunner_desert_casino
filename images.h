#ifndef _IMAGES_H_
#define _IMAGES_H_

class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image();
    Image(const char *fname);
};

unsigned char *buildAlphaData(Image *img);

#endif
