#pragma once

#include "ImageFilter.h"
#include <iostream>
using namespace std;

namespace IManip {

class ImageCropper : public ImageFilter {
private:

    //These values are used to get the pixel coordinates.

    int xPixel(int x, int y, int width, int height) {
        return (x);
    }

    int yPixel(int x, int y, int width, int height) {
        return (y);
    }

    //x1 and y1 are the 1st point for the Rectangle, and x2 and x2 are used to create the 2nd point for the Rectangle. 
    int x1;
    int y1;
    int x2;
    int y2;

public:


    //This gets 2 points which are used to form the rectangle/square dimensions of the image crop.

    ImageCropper(int get_x1, int get_y1, int get_x2, int get_y2)
    : x1(get_x1), y1(get_y1), x2(get_x2), y2(get_y2) {
    }

    //This creates a new image with the size newWidth and newHeight

    virtual RGBImage filter(const RGBImage& srcImg) {

        int newWidth = x2 - x1;
        int newHeight = y2 - y1;

        RGBImage Crop(newWidth, newHeight);

        /**By setting x to x1 and y to y1, a new image will be created starting from the 1st point 
         *given by the user to the 2nd point given in by the user. 
         *The resulting image is then returned.
         */

        for (int x = x1; x < x2; x++) {
            for (int y = y1; y < y2; y++) {
                Crop.setRGB(
                        xPixel(x - x1, y - y1, srcImg.getWidth(), srcImg.getHeight()),
                        yPixel(x - x1, y - y1, srcImg.getWidth(), srcImg.getHeight()),
                        srcImg.getRGB(x, y));
            }
        }

        return Crop;
    }

};

}