#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include "Test.h"
#include "Coloramplifier.h"
#include "ColorInverter.h"
#include "ColorSplitter.h"
#include "ImageReflector.h"
#include "ImageRotator.h"
#include "ImageScaler.h"
#include "ImageSeparator.h"
#include "ImageSlicer.h"

namespace IManip {

/**
 * Checks if the remaining data in two file streams is identical
 * @param ifs1 the first file stream to check
 * @param ifs2 the file stream to compare it to
 * @return true if the remaining data is identical, false if one stream terminates before the other
 */
bool equalContents(std::ifstream& ifs1, std::ifstream& ifs2) {
    bool filesEqual = true;
    while(!ifs1.eof() || !ifs2.eof())
    {
        if(ifs1.get() != ifs2.get())
        {
            filesEqual = false;
            break;
        }
    }
    // if either of the files isn't at its end
    if(!ifs1.eof() && !ifs2.eof())
    {
        filesEqual = false;
    }
    return filesEqual;
}
    
/**
 * Unit Tests ensure correctness of the library's different usages.
 */
class ImageTester : public TestSuite::Test {
public:
    /**
     * Runs tests on the different usages of the library.
     * Checks:<ul>
     * <li>Library file input and output</li>
     * <li>Image equality and inequality</li>
     * <li>Built-in ImageFilters</li>
     * <li>Built-in ImageSeparators</li>
     * </ul>
     */
    void run() {
        // test that the program does IO properly
        saveImage("images/test/apple_copy.bmp", RGBImage("images/apple.bmp"));
        std::ifstream ifs1("images/apple.bmp", std::ios::in | std::ios::binary);
        std::ifstream ifs2("images/test/apple_copy.bmp", std::ios::in | std::ios::binary);
        // seek to data, header need not be reliable
        ifs1.seekg(DATA_START_INDEX); 
        ifs2.seekg(DATA_START_INDEX); 
        test_(equalContents(ifs1, ifs2));
        // close the filestreams
        ifs1.close();
        ifs2.close();
        // delete the copied image
        remove("images/test/apple_copy.bmp");
        
        // test various filters
        // load a test image which will not be modified
        RGBImage testImage("images/test.bmp");
        
        // test that an image is equal to itself
        test_(testImage == testImage);
        
        // test that an image is not equal to another different image
        test_(testImage != RGBImage("images/apple.bmp"));
        
        // test the ColorInverter
        ColorInverter inverter;
        test_(RGBImage("images/test/test_inverted.bmp") == inverter.filter(testImage));
        
        // test the ImageRotator
        ImageRotator rotator(1);
        test_(RGBImage("images/test/test_rotated_1.bmp") == rotator.filter(testImage));
        
        // test the ImageScaler
        ImageScaler scaler(2);
        test_(RGBImage("images/test/test_scaled_2.bmp") == scaler.filter(testImage));
        
        // test the ImageReflector
        ImageReflector reflector;
        test_(RGBImage("images/test/test_reflected.bmp") == reflector.filter(testImage));
        
        // test the ImageCropper
        ImageCropper cropper(50, 50, 250, 250);
        test_(RGBImage("images/test/test_crop_50_50_250_250.bmp") == cropper.filter(testImage));
        
        // test the ColorAmplifier
        ColorAmplifier amplifier(0.75, 0.5, 0.3);
        test_(RGBImage("images/test/test_amped_0-75_0-5_0-3.bmp") == amplifier.filter(testImage));
        
        // test the ImageSlicer
        ImageSlicer slicer(3, 3);
        test_(RGBImage("images/test/test_sliced_3x3_4.bmp") == slicer.separate(testImage)[4]);
        
        // test the ColorSplitter
        ColorSplitter splitter;
        test_(RGBImage("images/test/test_color_split_1.bmp") == splitter.separate(testImage)[1]);
    }
};

};
