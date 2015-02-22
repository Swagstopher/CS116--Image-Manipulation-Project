#pragma once
#include "ImageFilter.h"
#include "Exceptions.h"

namespace IManip {
    
/**
 * ImageScaler scales an image up in size by a positive integer specified in
 * the constructor of the ImageScaler. It inherits from ImageTransformer, and
 * overrides the transform(const RGBImage&) function.
 */
class ImageScaler : public ImageFilter {
private:
    int scale; /// The scale that every image passed in is scaled up by
public:
    /**
     * Creates an ImageScaler with the given scale. The ImageScaler scales up
     * images passed to it by its scale.
     * @param scale the scale used in filtering, must be a positive integer.
     */
    ImageScaler(int scale) : scale(scale)  {
        if(scale < 1)
        {
            throw IllegalArgumentException("ImageScaler scale cannot be less than 1");
        }
    }
    
    /**
     * ImageScaler's transform(const RGBImage&) function returns a scaled up
     * copy of the image passed to it by reference. The factor by which the
     * image is scaled up is determined by ImageScaler's scale member variable,
     * which is initialized at construction.
     * @param srcImg the base image used in the transformation
     * @return a scaled up copy of the image.
     */
    virtual RGBImage filter(const RGBImage& srcImg) {
        // allocating a new image that is of the appropriate size, determined
        // by the source image's dimensions and the scale.
        RGBImage scaledImage(srcImg.getWidth()*scale, srcImg.getHeight()*scale);
        
        // for every pixel in the source image
        for(int x = 0; x < srcImg.getWidth(); x++)
        {
            for(int y = 0; y < srcImg.getHeight(); y++)
            {
                // load the pixel from the source image
                RGBPixel pix = srcImg.getRGB(x, y);
                // then store that pixel the appropriate number of times
                // in the new image
                
                int scaled_x = x*scale;
                int scaled_y = y*scale;
                
                for(int xs = 0; xs < scale; xs++)
                {
                    for (int ys = 0; ys < scale; ys++)
                    {
                        scaledImage.setRGB(scaled_x + xs, scaled_y + ys, pix);
                    }
                }

            }
        }

        return scaledImage;
    }
};

}
