#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include "RGBType.h"

class Image {
    const char *filename;
    int width, height, dpi;
    RGBType* data;
    unsigned char bmp_file_header[14] = {'B', 'M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmp_info_header[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    public:
        Image(const char*, int, int, int, RGBType*);

        void bmp_file_headerr(int filesize) {
            bmp_file_header[ 2] = (unsigned char)(filesize);
            bmp_file_header[ 3] = (unsigned char)(filesize>>8);
            bmp_file_header[ 4] = (unsigned char)(filesize>>16);
            bmp_file_header[ 5] = (unsigned char)(filesize>>24);
        }

        void bmp_info_headerr(int s, int ppm) {
            bmp_info_header[ 4] = (unsigned char)(width);
            bmp_info_header[ 5] = (unsigned char)(width>>8);
            bmp_info_header[ 6] = (unsigned char)(width>>16);
            bmp_info_header[ 7] = (unsigned char)(width>>24);

            bmp_info_header[ 8] = (unsigned char)(height);
            bmp_info_header[ 9] = (unsigned char)(height>>8);
            bmp_info_header[ 10] = (unsigned char)(height>>16);
            bmp_info_header[ 11] = (unsigned char)(height>>24);

            bmp_info_header[ 21] = (unsigned char)(s);
            bmp_info_header[ 22] = (unsigned char)(s>>8);
            bmp_info_header[ 23] = (unsigned char)(s>>16);
            bmp_info_header[ 24] = (unsigned char)(s>>24);

            bmp_info_header[ 25] = (unsigned char)(ppm);
            bmp_info_header[ 26] = (unsigned char)(ppm>>8);
            bmp_info_header[ 27] = (unsigned char)(ppm>>16);
            bmp_info_header[ 28] = (unsigned char)(ppm>>24);

            bmp_info_header[ 29] = (unsigned char)(ppm);
            bmp_info_header[ 30] = (unsigned char)(ppm>>8);
            bmp_info_header[ 31] = (unsigned char)(ppm>>16);
            bmp_info_header[ 32] = (unsigned char)(ppm>>24);
        }

        void savebmp() {

            FILE *f;
            int k = width*height;
            int s = 4*k;
            int filesize = 54 + s;

            double factor = 39.375;
            int m = static_cast<int>(factor);

            int ppm = dpi*m;

            bmp_file_headerr(filesize);
            bmp_info_headerr(s, ppm);

            f = fopen(filename, "wb");

            fwrite(bmp_file_header,1,14,f);
            fwrite(bmp_info_header,1,40,f);

            for(int i = 0; i < k; i++) {
                RGBType rgb = data[i];

                double red = (data[i].r)*255;
                double green = (data[i].g)*255;
                double blue = (data[i].b)*255;

                unsigned char color[3] = {(unsigned char)floor(blue), (unsigned char)floor(green), (unsigned char)floor(red)};
                fwrite(color, 1, 3, f);
            }

            fclose(f);
        }
};

Image::Image(const char* filename, int width, int height, int dpi, RGBType* data) {
    this->filename = filename;
    this->width = width;
    this->height = height;
    this->dpi = dpi;
    this->data = data;
}

#endif