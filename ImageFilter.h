#pragma once
#include <vector>
#include "RGBImage.h"

namespace IManip {

/**
 * ImageTransfomer is the base abstract class for all of the classes that can
 * transform (filter/manipulate/modify/whatever) images. Any user defined
 * ImageTransformer should inherit from ImageTransformer and override the
 * filter(const RGBImage&) function.
 */
class ImageFilter {
public:
    /**
     * This virtual function should be overridden by any class inheriting from
     * ImageTransformer. This allows for polymorphic image transformation.
     * @param srcImg the base image used in the transformation
     * @return a filtered version of the image, not the original image.
     */
    virtual RGBImage filter(const RGBImage& srcImg) = 0;
    
    /**
     * Applies a specific filter to all of the Images in a vector.
     * @param srcImgs the vector of images to be transformed.
     * @return A new vector containing the transformed images.
     */
    std::vector<RGBImage> applyOverVector(const std::vector<RGBImage> srcImgs) {
        std::vector<RGBImage> transformedImages(srcImgs.size());
        for(int i = 0; i < srcImgs.size(); i++)
        {
            transformedImages[i] = filter(srcImgs[i]);
        }
        return transformedImages;
    }
};

/**
 * Applies all of the filters in the vector to the source image and returns the
 * final transformed image.
 * @param filters the vector of filters to be used.
 * @param srcImg the initial source image.
 * @return the final transformed image, which is the source image with all of 
 * the filters applied to it.
 */
RGBImage applyFilters(const std::vector<ImageFilter*>& filters, const RGBImage& srcImg) {
    RGBImage transformedImage(srcImg);
    for(int i = 0; i < filters.size(); i++)
    {
        transformedImage = filters[i]->filter(transformedImage);
    }
    return transformedImage;
}

}
