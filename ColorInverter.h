#pragma once
#include "ImageFilter.h"
#include <iostream>

namespace IManip {

/**
 * ColorInverter inverts the colors of an image pixel by pixel. It inherits
 * from ImageTransformer and overrides the transform(const RGBImage&) function.
 * Notably, it does not currently have any member variables and does not have
 * a constructor.
 */
class ColorInverter : public ImageFilter {
private:
    /**
     * This private function returns an inverted version of the pixel passed
     * into it.
     * @param srcPix the source pixel whose inverse will be returned.
     * @return a pixel that represents the inverse of the source pixel
     */
    RGBPixel invertPixel(const RGBPixel& srcPix) {
        RGBPixel pix;
        pix.r = 255 - srcPix.r;
        pix.g = 255 - srcPix.g;
        pix.b = 255 - srcPix.b;
        return pix;
    }
public:
    /**
     * ColorInverter's transform(const RGBImage&) function returns an inverted
     * copy of the image passed to it by reference. The pixels are inverted by
     * the private invertPixel(const RGBPixel&) function.
     * @param srcImg the base image used in the transformation
     * @return an inverted version of the image, not the original image.
     */
    virtual RGBImage filter(const RGBImage& srcImg) {
        // make a new image to write to
        RGBImage invertedImage(srcImg.getWidth(), srcImg.getHeight());
        
        // for every pixel in the image
        for(int x = 0; x < srcImg.getWidth(); x++)
        {
            for(int y = 0; y < srcImg.getHeight(); y++)
            {
                // write an inverted copy of the source pixel to the new image
                invertedImage.setRGB(x, y, invertPixel(srcImg.getRGB(x, y)));
            }
        }
        
        return invertedImage;
    }
};

}
