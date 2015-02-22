#pragma once
#include <iostream>
#include "ImageFilter.h"
#include "Exceptions.h"

namespace IManip {

/** ImageRotator rotates the image counter clock-wise by 90 degrees.
 * It inherits from ImageTransformer,
 * and overrides the transform(const RGBImage&) function.
 */
class ImageRotator : public ImageFilter {
private:
    /** rotate determines the amount of times ImageRotator rotates the image */
    int rotate;

    int get_rotated_x(int x, int y, int width, int height) {
        switch (rotate) /// this switch statement takes the input of rotate and checks cases
        {
            case 0: return x;
            case 1: return height - y - 1;
            case 2: return width - x - 1;
            case 3: return y;
            default: throw Exception("We should never get here, rotations");
        }
    }

    int get_rotated_y(int x, int y, int width, int height) {
        switch (rotate) /// this switch statement takes the input of rotate and checks cases
        {
            case 0: return y;
            case 1: return x;
            case 2: return height - y - 1;
            case 3: return width - x - 1;
            default: throw Exception("We should never get here, rotations");
        }
    }

    /**
     * The following function determines the rotated image's width.
     * Using modulus, if the rotate input is even, then the width stays the same,
     * otherwise the width is the original image's height.
     */
    int get_rotated_width(int width, int height) {
        if (rotate % 2 == 0) {
            return width; /// if rotate is even, width remains the same
        } else {
            return height; /// if rotate is odd, the height becomes the width
        }
    }

    /**
     * The following function determines the rotated image's height.
     * Using modulus, if the rotate input is odd, then the height stays the same,
     * otherwise the height is the original image's width.
     */
    int get_rotated_height(int width, int height) {
        if (rotate % 2 == 0) {
            return height; // if rotate is even, height remains the same
        } else {
            return width; // if rotate is odd, the width becomes the height
        }
    }
public:

    /**
     * The following function accounts for any given values that are outside
     * the range of 0 to 3. Modulus will give the remainder of the number to
     * determine the number of 90 degree rotations.
     */
    ImageRotator(int rotate) {
        int rotation_modulus = rotate % 4;
        if (rotation_modulus < 0) 
        {
            this->rotate = rotation_modulus + 4;
        } 
        else 
        {
            this->rotate = rotation_modulus;
        }
    }

    RGBImage filter(const RGBImage& srcImg) {
        /// allocating a new image that is of the appropriate size, determined
        /// by the source image's dimensions.
        RGBImage rotatedImage(
                get_rotated_width(srcImg.getWidth(), srcImg.getHeight()),
                get_rotated_height(srcImg.getWidth(), srcImg.getHeight()));

        /// takes in every pixel of the source image
        for (int x = 0; x < srcImg.getWidth(); x++) 
        {
            for (int y = 0; y < srcImg.getHeight(); y++) 
            {
                /**
                 * The pixels are re-coordinated to the new points determined by
                 * the get_rotated functions.
                 */
                rotatedImage.setRGB(
                        get_rotated_x(x, y, srcImg.getWidth(), srcImg.getHeight()),
                        get_rotated_y(x, y, srcImg.getWidth(), srcImg.getHeight()),
                        srcImg.getRGB(x, y));



            }
        }

        return rotatedImage; /// returns the rotated image
    }

};

}
