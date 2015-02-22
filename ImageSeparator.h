#pragma once
#include <vector>
#include "RGBImage.h"

namespace IManip {

/**
 * ImageSeparator is the base abstract class for all of the classes that can
 * separate (split up) images. Any user defined ImageSeparator should inherit 
 * from ImageSeparator and override the separate(const RGBImage&) function.
 */
class ImageSeparator {
public:
    /** 
     * Separates the source image into component images of some kind.
     * How the images are separated depends on the ImageSeparater implementation.
     * @param srcImg the source image to split.
     * @return a vector of images containing the split images.
     */
    virtual std::vector<RGBImage> separate(const RGBImage& srcImg) = 0;
    
    /**
     * Applies a specific separator to all of the Images in a vector.
     * @param srcImgs the vector of images to be separated.
     * @return A new vector containing all of the separated images.
     */
    std::vector<RGBImage> applyOverVector(const std::vector<RGBImage> srcImgs) {
        std::vector<RGBImage> separatedImages;
        for(int i = 0; i < srcImgs.size(); i++)
        {
            std::vector<RGBImage> cachedImages = separate(srcImgs[i]);
            for(int j = 0; j < cachedImages.size(); j++)
            {
                separatedImages.push_back(cachedImages[j]);
            }
        }
        return separatedImages;
    }
};




}
