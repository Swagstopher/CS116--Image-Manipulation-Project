#pragma once
#include "ImageFilter.h"

namespace IManip {

/**
 * ColorAmplifier amplifies the colors of an image pixel by pixel. The scales used
 * in amplification are specified in the ColorAmplifier constructor. It inherits
 * from ImageTransformer and overrides the transform(const RGBImage&) function.
 */
class ColorAmplifier : public ImageFilter {
private:
    // amplification ratios for the pixel values, must be greater than 0
    /** the amplification ratio for red */
    double redRatio; 
    /** the amplification ratio for green */
    double greenRatio; 
    /** the amplification ratio for blue */
    double blueRatio; 
    
    /**
     * Amplifies the given byte color value by the given ratio. The given ratio
     * may not be a negative number. Any amplification that would return a value
     * greater than BYTE_MAX returns BYTE_MAX instead.
     * @param value the initial value to be amplified
     * @param ratio the ratio that the value will be amplified by
     * @return the amplified version of the value
     */
    static byte amplify(byte value, double ratio) {
        int amplifiedValue = value*ratio;
        return amplifiedValue < BYTE_MAX ? amplifiedValue : BYTE_MAX;
    }
    /**
     * Amplifies the colors of a given pixel by the ratios for this ColorAmplifier.
     * @param srcPix the source pixel whose values will be amplified.
     * @return an amplified version of the pixel.
     */
    RGBPixel amplifyPixel(const RGBPixel& srcPix) {
        RGBPixel pix;
        pix.r = amplify(srcPix.r, redRatio);
        pix.g = amplify(srcPix.g, greenRatio);
        pix.b = amplify(srcPix.b, blueRatio);
        return pix;
    }
public:
    /**
     * Constructs a ColorAmplifier that will amplify the colors of an image
     * with the given color ratios.
     * @param redRatio the amplification ratio for red, must not be less than 0
     * @param greenRatio the amplification ratio for green, must not be less than 0
     * @param blueRatio the amplification ratio for blue, must not be less than 0
     * @throws IllegalArgumentException if any of the ratios are less than 0
     */
    ColorAmplifier(double redRatio, double greenRatio, double blueRatio) 
        : redRatio(redRatio), greenRatio(greenRatio), blueRatio(blueRatio) {
        if(redRatio < 0 || greenRatio < 0 || blueRatio < 0)
        {
            throw IllegalArgumentException("Color amplification ratios must be greater than 0.");
        }
    }
        
    /**
     * ColorAmplifier's transform(const RGBImage&) function returns a copy of 
     * the image passed to it with the colors amplified. The colors are amplified
     * by the ratios specified in the ColorAmplifier constructor.
     * @param srcImg the base image used in the transformation
     * @return an amplified version of the image, not the original image.
     */
    virtual RGBImage filter(const RGBImage& srcImage) {
        RGBImage amplifiedImage(srcImage.getWidth(), srcImage.getHeight());
        
        // for every pixel in the source image
        for(int x = 0; x < srcImage.getWidth(); x++)
        {
            for(int y = 0; y < srcImage.getHeight(); y++)
            {
                // put an amplified copy of the source pixel into the amplified image
                amplifiedImage.setRGB(x, y, amplifyPixel(srcImage.getRGB(x, y)));
            }
        }
        // finally return the amplified copy
        return amplifiedImage;
    }
};

}
