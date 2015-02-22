#pragma once

#include "ImageSeparator.h"
#include "ImageCropper.h"

namespace IManip {

/**
 * ImageSlicer slices a source image into a specified number of quadrants, 
 * and then returns the subimages as a vector. It takes two parameters in its
 * constructor, the number of rows to slice the image into, and the number
 * of columns to slice it into. 
 */
class ImageSlicer : public ImageSeparator {
private:
    /** the number of rows that this slicer slices images into */
    int rows;
    /** the number of columns that this slicer slices images into */
    int columns;

public:
    /**
     * Creates an ImageSlicer that slices images into the specified number of rows and columns.
     * @param rows the number of rows to slice the source image into.
     * @param columns the number of columns to slice the source image into.
     */
    ImageSlicer(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
    }

    /**
     * Slices the source image into the specified number of rows and columns.
     * @param srcImg the image to slice.
     * @return a vector of images containing the subimages.
     */
    virtual std::vector<RGBImage> separate(const RGBImage& srcImg) {
        std::vector<RGBImage> slicedImages(rows*columns);
        
        int rowHeight = srcImg.getHeight() / rows;
        int columnWidth = srcImg.getWidth() / columns;
        
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                ImageCropper cropper(r*columnWidth, c*rowHeight, 
                             r*columnWidth + columnWidth, c*rowHeight + rowHeight);
                slicedImages[r*rows + c] = cropper.filter(srcImg);
            }
        }
        
        return slicedImages;
    }

};

}
