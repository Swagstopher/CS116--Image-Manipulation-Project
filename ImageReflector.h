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
class ImageReflector : public ImageFilter {
private:
	int getReflectedX(int x, int y, int width, int height) {
		return (width-1-x);
	}
	int getReflectedY(int x, int y, int width, int height) {
		return (y);
	}
public:
	ImageReflector() {}

    virtual RGBImage filter(const RGBImage& srcImg) {
		RGBImage reflectedImage(srcImg.getWidth(), srcImg.getHeight());

		for(int x = 0; x < srcImg.getWidth(); x++)
		{
			for(int y = 0; y < srcImg.getHeight(); y++)
			{
				// write a reflected copy of the source pixel to the new image
				reflectedImage.setRGB(
					getReflectedX(x, y, srcImg.getWidth(), srcImg.getHeight()),
					getReflectedY(x, y, srcImg.getWidth(), srcImg.getHeight()),
					srcImg.getRGB(x, y));
			}
		}

		return reflectedImage;
    }

};

}
