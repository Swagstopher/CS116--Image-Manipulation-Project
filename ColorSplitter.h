#pragma once
#include "ImageSeparator.h"
#include "ColorAmplifier.h"

namespace IManip {

/**
 * ColorSplitter splits an image into into its component subimages by 
 * masking a given image with red, green, and blue filters. It returns 
 * three images, one for each color. It takes no arguments in its constructor.
 */
class ColorSplitter : public ImageSeparator {
private:
    /** Filter to separate out the red coloration of an image */
    ColorAmplifier redFilter;
    /** Filter to separate out the green coloration of an image */
    ColorAmplifier greenFilter;
    /** Filter to separate out the blue coloration of an image */
    ColorAmplifier blueFilter;
public:
    /**
     * Constructs a ColorSplitter that will split a given image into its red,
     * green, and blue componenents. Does not take any arguments.
     */
    ColorSplitter() : redFilter(1,0,0), greenFilter(0,1,0), blueFilter(0,0,1) { }
    
    /**
     * Separates the source image into red, green, and blue component images.
     * The vector returned will contain three images, one for each color.
     * Each image is created by masking the source image, transferring only
     * the values of a certain color to the destination pixel.
     * @param srcImg the image to be separated into component images.
     * @return a vector containing the three component images: red, green, blue
     */
    virtual std::vector<RGBImage> separate(const RGBImage& srcImg) {
        std::vector<RGBImage> images;
        
        images.push_back(redFilter.filter(srcImg));
        images.push_back(greenFilter.filter(srcImg));
        images.push_back(blueFilter.filter(srcImg));
        
        return images;
    }
};

}
