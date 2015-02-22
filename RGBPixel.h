#pragma once
#include <istream>
#include <climits>
#include <ostream>

#define BYTE_MAX UCHAR_MAX
#define BYTE_BIT CHAR_BIT

namespace IManip {
    
/**
 * c++ doesn't have a native "byte" type, but "unsigned char" is a pain to type
 * properly, so I defined a byte type which is equivalent to unsigned char
 */ 
typedef unsigned char byte;

/**
 * This struct is the internal representation of a 24 bit bitmap pixel.
 * Note that each data member is 1 byte in size. Each of the values can be 
 * read and modified manually.
 */
struct RGBPixel {
byte r; /// red color values of the pixel
byte g; /// green color values of the pixel
byte b; /// blue color values of the pixel
/**
 * Standard constructor to facilitate initialization.
 * @param r the byte value of the red in the pixel
 * @param g the byte value of the green in the pixel
 * @param b the byte value of the blue in the pixel
 */
RGBPixel(byte r, byte g, byte b) : r(r), g(g), b(b) {}
/**
 * Default constructor to allow generic initialization. This would especially 
 * be used before reading in to the pixel with a file stream
 */
RGBPixel() : r(0), g(0), b(0) {}

/**
 * Overloading of operator== compares the byte values for the two pixels.
 * If the byte values are equal, then the pixels are considered equal.
 * @param pix the pixel that this is compared to.
 * @return true if the pixels are equivalent.
 */
bool operator==(const RGBPixel& pix) {
    return  this->r == pix.r
         && this->g == pix.g
         && this->b == pix.b;
}
/**
 * Overloading of operator!= returns the inverse of operator==.
 * @param pix the pixel that this is compared to.
 * @return true if the pixels are inequivalent.
 */
bool operator!=(const RGBPixel& pix) {
    return !operator==(pix);
}
};

/**
 * Overloading of operator>> loads the pixel's internal data from a file stream.
 * The bitmap specification has the bytes laid out b,g,r.
 * @param ifs the file input stream that the pixel data will be read from.
 * @param pix the pixel that the pixel data will be written to.
 * @return a reference to the file input stream to be used for chaining.
 */
std::ifstream& operator>>(std::ifstream& ifs, RGBPixel& pix) {
    pix.b = ifs.get();
    pix.g = ifs.get();
    pix.r = ifs.get();
    return ifs;
}
/**
 * Overloading of operator<< writes the pixel's internal data to a file stream.
 * The bitmap specification has the bytes laid out b,g,r.
 * @param ofs the file output stream that the pixel data will be written to.
 * @param srcPix the pixel pixel that the data will be read from.
 * @return a reference to the file output stream to be used for chaining.
 */
std::ofstream& operator<<(std::ofstream& ofs, const RGBPixel& srcPix) {
    ofs.put(srcPix.b);
    ofs.put(srcPix.g);
    ofs.put(srcPix.r);
    return ofs;
} 
/**
 * Overloading of operator<< prints the pixel's internal data to an output stream.
 * The format is as follows: "r: [r] g: [g] b: [b]"
 * @param os the output stream to be printed to.
 * @param srcPix the pixel that will be printed to the output stream.
 * @return 
 */
std::ostream& operator<<(std::ostream& os, const RGBPixel& srcPix) {
    return os << "r: " << (int)srcPix.r << " g: " << (int)srcPix.g << " b: " << (int)srcPix.b;  
}

}
