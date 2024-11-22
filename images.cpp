#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include "images.h"

Image::~Image() { delete [] data; }
Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;
    //printf("fname **%s**\n", fname);
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
        ppmFlag = 1;
    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen-4] = '\0';
        //printf("name **%s**\n", name);
        //sprintf(ppmname,"%s.ppm", name);
        snprintf(ppmname, sizeof(ppmname), "%s.ppm", name);
        //printf("ppmname **%s**\n", ppmname);
        char ts[100];
        //system("convert eball.jpg eball.ppm");
        //sprintf(ts, "convert %s %s", fname, ppmname);
        snprintf(ts, sizeof(ts), "convert %s %s", fname, ppmname);
        // modified convert -> magick, to fix convert deprecated warning
        //snprintf(ts, sizeof(ts), "magick %s %s", fname, ppmname);
        system(ts);
    }
    //sprintf(ts, "%s", name);
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        //skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        //get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];
        for (int i=0; i<n; i++)
            data[i] = fgetc(fpi);
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n",ppmname);
        exit(0);
    }
    if (!ppmFlag)
        unlink(ppmname);
}

unsigned char *buildAlphaData(Image *img)
{
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        // code contributed by student: Chris Smith
        *(ptr+3) = (a|b|c);
        ptr += 4;
        data += 3;
    }
    return newdata;
}
