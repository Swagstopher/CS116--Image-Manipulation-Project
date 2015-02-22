#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Exceptions.h"
#include "RGBPixel.h"

namespace IManip {

// forward declaration of load function for use in constructor
class RGBImage;
std::ifstream& operator>>(std::ifstream& ifs, RGBImage& destImg);

// Here are the constants used to write to the file header for a bitmap image.
// The full bitmap header specification can be found here:
// http://www.fastgraph.com/help/bmp_header_format.html
const int FILE_START_INDEX = 0; /// start index of the file
const short BMP_IDENTIFIER = 0x4d42; /// file type identifer, constant 0x4d42
const int FILE_SIZE_INDEX = 2; /// index where the image filesize is stored
const int DATA_START_INDEX_INDEX = 10; /// index where DATA_START_INDEX is stored
const int DATA_START_INDEX = 54; /// index where the image data starts is always 54
const int HEADER_SIZE_INDEX = 14; /// index where HEADER_SIZE is found
const int HEADER_SIZE = 40; /// size of header metadata (stuff after HEADER_SIZE_INDEX)
const int WIDTH_INDEX = 18; /// index where image width is found
const int HEIGHT_INDEX = 22; /// index where image height is found
const int PLANES_INDEX = 26; /// index where "number of planes" is found
const short PLANES = 1; /// number of planes is always 1
const int BIT_DEPTH_INDEX = 28; /// index where pixel bit depth is found
const int PIXEL_SIZE = 3; /// size in bytes of a pixel
const short BIT_DEPTH = PIXEL_SIZE*BYTE_BIT; /// all of our images are 24 bit (3 byte) pixels
const int IMAGE_SIZE_INDEX = 34; /// index where image size (not including header) is found

/* fstream can only read in or write out one char at a time. That limitation
 * makes reading and writing anything larger than a byte a bit of a pain to do.
 * These functions allow for easier read/write to a specific position.
 *
 * They're a bit of black magic. If you want to know how they work, track me
 * down and I'll explain it to you. Otherwise, don't feel pressured to understand
 * why they work and trust that they do.
 */
int getInt(std::ifstream& ifs, int offset) {
    ifs.seekg(offset);

    int result = 0;

    for(int i = 0; i < sizeof(int); i++)
    {
        // bit shift each character by the number of bytes * byte bitwidth
        result += ifs.get()<<(i * BYTE_BIT);
    }

    return result;
}
void putInt(std::ofstream& ofs, int offset, int val) {
    ofs.seekp(offset);

    for(int i = 0; i < sizeof(int); i++)
    {
        ofs.put(val>>(i*BYTE_BIT) % (BYTE_MAX + 1));
    }
}
short getShort(std::ifstream& ifs, int offset) {
    ifs.seekg(offset);

    short result = 0;

    for(int i = 0; i < sizeof(short); i++)
    {
        // bit shift each character by the number of bytes * byte bitwidth
        result += ifs.get()<<(i * BYTE_BIT);
    }

    return result;
}
void putShort(std::ofstream& ofs, int offset, short val) {
    ofs.seekp(offset);

    for(int i = 0; i < sizeof(short); i++)
    {
        ofs.put(val>>(i*BYTE_BIT) % (BYTE_MAX + 1));
    }
}

/**
 * Gets the number of padding bytes added to every scanline for an image with
 * the given width.
 * A scanline is a line of pixels across the x direction of an image. With the
 * bitmap format, the number of bytes in a scanline must be a multiple of 4.
 * If, after the pixels in a scanline have been written, the bytes in the
 * scanline are not divisible by 4, padding bytes must be added.
 * @param width the width of the scanline to begin with in bytes.
 * @return the number of padding bytes needed for a scanline of the given width
 */
int getScanlinePadding(int width) {
    int scanlineSize = width * PIXEL_SIZE;
    if(scanlineSize % 4 != 0)
    {
        return 4 - scanlineSize % 4;
    }
    else
    {
        return 0;
    }
}

/**
 * This class is the internal representation of a 24-bit bitmap image.
 * The memory to store the image data is heap allocated.
 * Individual pixels may be accessed or modified with coordinates.
 * The size of the image is immutable once created. Create a new RGBImage
 * to "change" the size.
 */
class RGBImage {
private:
    /** "2d array" of pixel data with dimension width*height */
    RGBPixel* image;
    /** the image width in pixels */
    int width; 
    /** the image height in pixels */
    int height;
    /**
     * Checks if the given coordinates are within the bounds of the image, and
     * throws an exception if they are not.
     * @param x the x coordinate to check.
     * @param y the y coordinate to check.
     * @throws IndexOutOfBoundsException if the point is outside of the bounds
     */
    void assertBounds(int x, int y) const {
        if(x < 0
        || x >= width
        || y < 0
        || y >= height)
        {
            std::stringstream stream;
            // with the default constructor, bounds can be 0. This is to allow
            // easier initialization before assignment or reading in.
            if(width == 0 || height == 0)
            {
                stream << "Image not properly intiialized: bounds of 0. widt:h"
                       << width << " height: " << height;
            }
            else
            {
                stream << "Bounds error: (" << x << "," << y
                       << "), width: " << width << " height: " << height;
            }
            throw IndexOutOfBoundsException(stream.str());
        }
    }
    /**
     * Initializes the data members of this RGBImage to the given width and
     * height. The width and height must be non-negative quantities.
     * @param width the width in pixels, must be non-negative
     * @param height the height in pixels, must be non-negative
     */
    void initializeWith(int width, int height) {
        // ensure that the dimensions are greater than zero
        if(width < 0 || height < 0)
        {
            std::stringstream stream;
            stream << "Dimensions must be greater than zero. Width: "
                   << width << " Height: " << height << "\n";
            throw IllegalArgumentException(stream.str());
        }
        this->width = width;
        this->height = height;

        // heap allocated "2d array" to store image data
        this->image = new RGBPixel[width*height];
    }
    /**
     * Initializes the data members of this RGBImage to those of the source image
     * Note that this assumes that the image* has either not yet been allocated,
     * or has been properly deleted.
     * The assignment operator (operator==) should be used for general assignment.
     * @param srcImg the image whose data will be copied.
     */
    void initializeTo(const RGBImage& srcImg) {
        // copy dimensions of the source image
        this->width = srcImg.width;
        this->height = srcImg.height;

        // copy pixel data of source image
        this->image = new RGBPixel[width*height];
        for(int i = 0; i < width*height; i++)
        {
            this->image[i] = srcImg.image[i];
        }
    }
public:
    /**
     * RGBImage constructor takes width and height, heap allocates image memory.
     * @param width the width of the image in pixels
     * @param height the height of the image in pixels
     * @throws IllegalArgumentException if either dimension is negative
     */
    RGBImage(int width, int height) {
        initializeWith(width, height);
    }
    /**
     * RGBImage constructor takes a filename and loads the bitmap image in that file.
     * @param filename the name of the bitmap file to load for the image.
     * @throws FileException if the file does not exist, is not a bitmap, or is corrupt.
     */
    RGBImage(std::string filename) {
        std::ifstream ifs;
        ifs.open(filename.c_str(), std::ios::in | std::ios:: binary);

        // if the file cannot be read
        if(!ifs.good())
        {
            ifs.close();
            throw FileException(filename, "File cannot be read or does not exist");
        }
        // make sure that the file opened is of a valid bitmap
        if(getShort(ifs, FILE_START_INDEX) != BMP_IDENTIFIER)
        {
            ifs.close();
            throw FileException(filename, "File is not a bitmap");
        }

        // load data about the file from the stream;
        int file_size = getInt(ifs, FILE_SIZE_INDEX);
        int data_start = getInt(ifs, DATA_START_INDEX_INDEX);
        int data_width = getInt(ifs, WIDTH_INDEX);
        int data_height = getInt(ifs, HEIGHT_INDEX);

        // validate the file header by checking the padding bytes
        if(file_size != data_start + (data_width * PIXEL_SIZE + getScanlinePadding(data_width)) * data_height)
        {
            ifs.close();
            throw FileException(filename, "File is not a valid bitmap.");
        }

        // initialize the image
        initializeWith(data_width, data_height);

        ifs.seekg(data_start);
        ifs >> *this;

        ifs.close();
    }
    /**
     * Copy constructor for RGBImage. Makes a deep copy of the old image.
     * @param srcImg the image whose data will be copied.
     */
    RGBImage(const RGBImage& srcImg) {
        initializeTo(srcImg);
    }
    /**
     * Default constructor takes no arguments and initializes an image with no
     * pixels. Convenience constructor for immediate assignment or read in.
     */
    RGBImage() : width(0), height(0) { }
    /**
     * RGBImage destructor to release heap allocated memory
     */
    ~RGBImage() {
        // if the image data had been heap allocated
        if(image)
        {
            // delete that heap allocated data
            delete image;
        }
    }
    /**
     * Assignment operator makes a deep copy of the source image.
     * @param srcImg the image whose data will be copied.
     * @return a reference to this.
     */
    RGBImage& operator=(const RGBImage& srcImg) {
        // We only have to do the assignment if the objects are unequal
        // If they're already equal, then we can skip the assignment.
        if(*this != srcImg)
        {
            // Call the destructor to clean up any old heap allocated image data
            this->~RGBImage();
            // initialize the now cleared data members to those of the source image
            initializeTo(srcImg);
        }
        return *this;
    }
    /**
     * Overloading of operator== compares the dimensions and then the individual
     * pixel values of the two images. If the dimensions and pixels are equal,
     * then the images are equal.
     * @param img the image that this will be compared to.
     * @return true if the images are equivalent.
     */
    bool operator==(const RGBImage& img) {
        // if the dimensions aren't equal, then the images definitely aren't
        if( !(width == img.width && height == img.height) )
        {
            return false;
        }

        // If the pointers are equal, then the data definitely is, so we don't
        // need to check explicitly.
        if(image != img.image)
        {
            // iterate through every pixel in the images
            for(int i = 0; i < width*height; i++)
            {
                // if any of the pixels in the images aren't equal, the images aren't
                if(image[i] != img.image[i])
                {
                    return false;
                }
            }
        }

        // finally, if the dimensions of the images are equal, and the
        // individual pixels of the images are equal, then the images are equal
        return true;
    }
    /**
     * Overloading of operator!= returns the inverse of operator==.
     * @param img the image that this will be compared to.
     * @return true if the images are not equivalent.
     */
    bool operator!=(const RGBImage& img) {
        return !operator==(img);
    }

    /**
     * Gets the width of the image in pixels.
     * @return the width of the image in pixels.
     */
    int getWidth() const {
        return width;
    }
    /**
     * Gets the height of the image in pixels.
     * @return the height of the image in pixels.
     */
    int getHeight() const {
        return height;
    }
    /**
     * Gets the pixel at the given coordinates in the image.
     * @param x the x coordinate of the pixel to be retrieved
     * @param y the y coordinate of the pixel to be retrieved
     * @return the pixel at the given coordinates
     * @throws IndexOutOfBoundsException if the coordinates are out of the image's bounds
     */
    RGBPixel getRGB(int x, int y) const {
        assertBounds(x, y);
        return image[x*height + y];
    }
    /**
     * Puts the pixel at the given coordinates in the image.
     * @param x the x coordinate of the pixel to be set
     * @param y the y coordinate of the pixel to be set
     * @param pixel the pixel to be set.
     * @throws IndexOutOfBoundsException if the coordinates are out of the image's bounds
     */
    void setRGB(int x, int y, RGBPixel pixel) {
        assertBounds(x, y);
        image[x*height + y] = pixel;
    }
    /**
     * Gets a copy of a subsection of this image.
     * @param xOffset the top left x coordinate to begin the read from
     * @param yOffset the top left y coordinate to begin the read from
     * @param width the width of the SubImage
     * @param height the height of the SubImage
     * @throws IndexOutOfBoundsException if the SubImage dimensions overflow those of the SrcImage
     * @return a copy of the given SubImage
     */
    RGBImage subImage(int xOffset, int yOffset, int width, int height) const {
        if( (xOffset + width  > this->width)
         || (yOffset + height > this->height) )
        {
            std::stringstream stream;
            stream << "SubImage dimensions out of bounds:\nSubImage: x: " << xOffset 
                   << " y: " << yOffset << " width: " << width << " height: " 
                   << height << "\nSrcImage: width: " << this->width 
                   << " height: " << this->height;
            throw IndexOutOfBoundsException(stream.str());
        }
        
        RGBImage subImage(width, height);
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                subImage.setRGB(x, y, getRGB(x + xOffset, y + yOffset));
            }
        }
        return subImage;
    }
};
/**
 * Overloading of operator>> reads image pixel data into the RGBImage from
 * a file stream. This assumes that the RGBImage has been properly initialized
 * based on the header in the file stream.
 * @param ifs the file input stream where the image data will be read.
 * @param destImg the image where the image data will be written.
 * @return a reference to the file input stream
 */
std::ifstream& operator>>(std::ifstream& ifs, RGBImage& destImg) {
    // note that bmp format has the origin at the bottom left
    // while RGBImage has the origin at the top left
    int padding = getScanlinePadding(destImg.getWidth());
    for(int y = destImg.getHeight() - 1; y >= 0; y--)
    {
        for(int x = 0; x < destImg.getWidth(); x++)
        {
            RGBPixel pix;
            ifs >> pix;
            destImg.setRGB(x, y, pix);
        }
        // after every scanline, ignore the padding
        ifs.ignore(padding);
    }
    return ifs;
}
/**
 * Overloading of operator<< writes image pixel data of an RGBImage to a
 * file stream. This assumes that the appropriate header has already been
 * written to the stream. The header should be written with with writeHeader()
 * @param ofs the file output stream where the image data will be written.
 * @param srcImg the image where the image data will be read from.
 * @return a reference to the file output stream
 */
std::ofstream& operator<<(std::ofstream& ofs, const RGBImage& srcImg) {
    // note that bmp format has the origin at the bottom left
    // while RGBImage has the origin at the top left
    int padding = getScanlinePadding(srcImg.getWidth());
    for(int y = srcImg.getHeight() - 1; y >= 0; y--)
    {
        for(int x = 0; x < srcImg.getWidth(); x++)
        {
            ofs << srcImg.getRGB(x,y);
        }
        // After every scanline, write padding if necessary
        for(int i = 0; i < padding; i++)
        {
            ofs.put(0);
        }
    }

    return ofs;
}
/**
 * Overloading of operator<< prints image pixel data of an RGBImage to an
 * output stream. The function loops through all of the pixels in the image
 * and prints them with the following format:
 * "pix: ([x coordinate],[y coordinate]): <pixel data>\n"
 * @param os the output stream where the image data will be written to.
 * @param srcImg the image where the image data will be read from.
 * @return a reference to the output stream.
 */
std::ostream& operator<<(std::ostream& os, const RGBImage& srcImg) {
    for(int y = 0; y < srcImg.getHeight(); y++)
    {
        for(int x = 0; x < srcImg.getWidth(); x++)
        {
            os << "pix: (" << x << ',' << y << "): " << srcImg.getRGB(x, y) << '\n';
        }
    }
    return os;
}

/**
 * Writes all of the necessary header information about the given RGBImage to
 * the given file stream.
 * @param ofs the file output stream that the header will be written to.
 * @param srcImg the source image that the header will be determined using.
 */
void writeHeader(std::ofstream& ofs, const RGBImage& srcImg) {
    // constant values for bitmap header
    putShort(ofs, FILE_START_INDEX, BMP_IDENTIFIER); // BMP header identifier, constant 0x4d42
    putInt(ofs, DATA_START_INDEX_INDEX, DATA_START_INDEX); // position where the read starts, constant 54
    putInt(ofs, HEADER_SIZE_INDEX, HEADER_SIZE); // size of header, constant 40
    putShort(ofs, PLANES_INDEX, PLANES); // number of "planes" in image, constant 1
    putShort(ofs, BIT_DEPTH_INDEX, BIT_DEPTH); // bit-depth of a pixel, constant 24

    // values of bitmap header dependent upon the bitmap
    putInt(ofs, WIDTH_INDEX, srcImg.getWidth()); // width of bitmap in pixels
    putInt(ofs, HEIGHT_INDEX, srcImg.getHeight()); // height of bitmap in pixels

    int scanline = srcImg.getWidth()*PIXEL_SIZE;
    int imageDataSize = (scanline + getScanlinePadding(srcImg.getWidth())) * srcImg.getHeight();
    putInt(ofs, IMAGE_SIZE_INDEX, imageDataSize);
    putInt(ofs, FILE_SIZE_INDEX, imageDataSize + DATA_START_INDEX);
}
/**
 * Saves the given image to a file at the given filename.
 * @param filename the name of the file that the image will be saved to.
 * @param srcImg the image to be saved.
 */
void saveImage(std::string filename, const RGBImage& srcImg) {
        std::ofstream ofs;
        ofs.open(filename.c_str(), std::ios::out | std::ios::binary);

        writeHeader(ofs, srcImg);
        ofs.seekp(DATA_START_INDEX);
        ofs << srcImg;

        ofs.close();
    }
/**
 * Saves the given images to files of the given filename plus a number.
 * @param filename the base filename that the image will be saved to.
 * @param srcImgs the images to be saved
 */
void saveImages(std::string filename, const std::vector<RGBImage>& srcImgs) {
    for(int i = 0; i < srcImgs.size(); i++)
    {
        std::stringstream stream;
        stream << filename.substr(0, filename.size() - 4) << i 
               << filename.substr(filename.size() - 4);
        saveImage(stream.str(), srcImgs[i]);
    }
}
}
