#pragma once
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include "Exceptions.h"
#include "Coloramplifier.h"
#include "ColorInverter.h"
#include "ColorSplitter.h"
#include "ImageReflector.h"
#include "ImageRotator.h"
#include "ImageScaler.h"
#include "ImageSeparator.h"
#include "ImageSlicer.h"

namespace IManip {

/** A help message displaying the builtin filters */
const std::string AVAILIBLE_FILTERS =
    "Known filters:\n"
    "ColorAmplifier:\tca <double> <double> <double>\n"
    "ColorInverter:\tci\n"
    "ColorSplitter:\tcs\n"
    "ImageCropper:\tic <int> <int> <int> <int>\n"
    "ImageReflector:\tiref\n"
    "ImageRotator:\tir <int>\n"
    "ImageScaler:\tis <int>\n"
    "ImageSlicer:\tisl <int> <int>\n";

/**
 * Checks that the required number of arguments are remaining in the command line arguments.
 * @param argCount the number of arguments to check for
 * @param message the error message to display if the check fails
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @throws IllegalArgumentException with information about the error if there
 *         are not enough arguments remaining
 */
void assertArgCount(int argCount, std::string message, int index, int argc, const char** argv) {
    int argsRemaining = argc - index;
    if(argsRemaining < argCount)
    {
        std::stringstream stream;
        
        stream << message << "\nArgs Found (" << argsRemaining << "): ";
        for(int i = 0; i < argsRemaining; i++)
        {
            stream << argv[index + i] << " ";
        }
        
        throw IllegalArgumentException(stream.str());
    }
}

/**
 * Constructs a ColorAmplifier based on the remaining command line arguments.
 * It will use three arguments, double double double.
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ColorAmplifier
 */
ColorAmplifier createColorAmplifier(int& index, int argc, const char** argv) {
    assertArgCount(3, "ColorAmplifier requires <double> <double> <double>", index, argc, argv);
    return ColorAmplifier(atof(argv[index++]), atof(argv[index++]), atof(argv[index++]));
}
/**
 * Constructs a ColorInverter based on the remaining command line arguments.
 * It will use no arguments, but takes them for symmetry.
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ColorInverter
 */
ColorInverter createColorInverter(int& index, int argc, const char** argv) {
    return ColorInverter();
}
/**
 * Constructs a ColorSplitter based on the remaining command line arguments.
 * It will use no arguments, but takes them for symmetry.
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ColorSplitter
 */
ColorSplitter createColorSplitter(int& index, int argc, const char** argv) {
    return ColorSplitter();
}
/**
 * Constructs an ImageCropper based on the remaining command line arguments.
 * It will use 4 arguments, int int int int
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ImageCropper
 */
ImageCropper createImageCropper(int& index, int argc, const char** argv) {
    assertArgCount(4, "ImageSlicer requires <int> <int> <int> <int>", index, argc, argv);
    return ImageCropper(atoi(argv[index++]),atoi(argv[index++]),atoi(argv[index++]),atoi(argv[index++]));
}
/**
 * Constructs an ImageReflector based on the remaining command line arguments.
 * It will use no arguments, but takes them for symmetry.
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ImageReflector
 */
ImageReflector createImageReflector(int& index, int argc, const char** argv) {
    return ImageReflector();
}
/**
 * Constructs an ImageRotator based on the remaining command line arguments.
 * It will use 1 argument, int
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ImageRotator
 */
ImageRotator createImageRotator(int& index, int argc, const char** argv) {
    assertArgCount(1, "ImageRotator requires <int>", index, argc, argv);
    return ImageRotator(atoi(argv[index++]));
}
/**
 * Constructs an ImageScaler based on the remaining command line arguments.
 * It will use 1 argument, int
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ImageScaler
 */
ImageScaler createImageScaler(int& index, int argc, const char** argv) {
    assertArgCount(1, "ImageScaler requires <int>", index, argc, argv);
    return ImageScaler(atoi(argv[index++]));
}
/**
 * Constructs an ImageSlicer based on the remaining command line arguments.
 * It will use 2 arguments, int int
 * @param index the index of the next argument to be used
 * @param argc the total number of arguments
 * @param argv the array of string literal arguments
 * @return the constructed ImageSlicer
 */
ImageSlicer createImageSlicer(int& index, int argc, const char** argv) {
    assertArgCount(2, "ImageSlicer requires <int> <int>", index, argc, argv);
    return ImageSlicer(atoi(argv[index++]), atoi(argv[index++]));
}

/**
 * Parses a set of string literal arguments and runs the resulting set of
 * Image Manipulations.
 * @param argc the total number of arguments 
 * @param argv the array of string literal arguments
 */
void parseAndRun(int argc, const char** argv) {
    if(argc < 2)
    {
        throw IllegalArgumentException("Format is: <input_filename> <output_filename> [filters...]");
    }
    std::string inputFilename = argv[0];
    std::string outputFilename = argv[1];
    
    int index = 2;
    std::vector<RGBImage> images;
    images.push_back(RGBImage(inputFilename)); // add the seed input image
    
    // run through the commands
    while(index < argc) {
        std::string command = argv[index++];
        if(command == "ca")
        {
            ColorAmplifier colorAmplifier = createColorAmplifier(index, argc, argv);
            images = colorAmplifier.applyOverVector(images);
        }
        else if(command == "ci")
        {
            ColorInverter colorInverter = createColorInverter(index, argc, argv);
            images = colorInverter.applyOverVector(images);
        }
        else if(command == "cs")
        {
            ColorSplitter colorSplitter = createColorSplitter(index, argc, argv);
            images = colorSplitter.applyOverVector(images);
        }
        else if(command == "ic")
        {
            ImageCropper imageCropper = createImageCropper(index, argc, argv);
            images = imageCropper.applyOverVector(images);
        }
        else if(command == "ir")
        {
            ImageRotator imageRotator = createImageRotator(index, argc, argv);
            images = imageRotator.applyOverVector(images);
        }
        else if(command == "iref")
        {
            ImageReflector imageReflector = createImageReflector(index, argc, argv);
            images = imageReflector.applyOverVector(images);
        }
        else if(command == "is")
        {
            ImageScaler imageScaler = createImageScaler(index, argc, argv);
            images = imageScaler.applyOverVector(images);
        }
        else if(command == "isl")
        {
            ImageSlicer imageSlicer = createImageSlicer(index, argc, argv);
            images = imageSlicer.applyOverVector(images);
        }
        else
        {
            std::stringstream stream;
            stream << "Unknown filter name: \"" << command << "\"\n" << AVAILIBLE_FILTERS << std::endl;
            throw IllegalArgumentException(stream.str());
        }
    }
    
    // Save the images
    if(images.size() == 1)
    {
        saveImage(outputFilename, images[0]);
    }
    else
    {
        saveImages(outputFilename, images);
    }
}

}
