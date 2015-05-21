


#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include <GL/glut.h>
#include <FreeImage.h>

// holds an image... more on this later!
class Image
{
public:
    Image();
    Image(int width, int height);
    Image(std::string file);
    Image(const Image& image);

    class Pixel {
    public:
        Pixel() : red(0), green(0), blue(0) { }
        Pixel(GLubyte red, GLubyte green, GLubyte blue) : red(red), green(green), blue(blue) { }
        GLubyte red;
        GLubyte green;
        GLubyte blue;
    };

    int getWidth() const;
    int getHeight() const;
    int getNumPixels() const;
    int getPixelIndex(int x, int y) const;
    Pixel& getPixel(int i);
    const Pixel& getPixel(int i) const;
    Pixel& getPixel(int x, int y);
    const Pixel& getPixel(int, int) const;

    void save(std::string file);

    Pixel setPixel(int i, const Pixel& pixel);
    Pixel setPixel(int x, int y, const Pixel& pixel);
    Pixel setPixel(int i, GLubyte, GLubyte, GLubyte);
    Pixel setPixel(int x, int y, GLubyte, GLubyte, GLubyte);
    int setPixelRange(int xmin, int xmax, int ymin, int ymax, const Pixel& pixel);
    int setPixelRange(int xmin, int xmax, int ymin, int ymax, GLubyte red, GLubyte green, GLubyte blue);

    //display modifiers
    void reset();
    void toGreyScale();
    void toNTSC();
    void toTone();
    void toMonochrome();
    void toInverted();
    void toRedOnly();
    void toGreenOnly();
    void toBlueOnly();

    void toCyanOnly();
    void toYellowOnly();
    void toMagentaOnly();

    //filters
    void applyMaxFilter();
    void applyMinFilter();
    void applyRedIntensifyFilter(double factor = 1.05);
    void applyGreenIntensifyFilter(double factor = 1.05);
    void applyBlueIntensifyFilter(double factor = 1.05);
    void applyMaskFilter(double[9]);
    void applyManyMaskFilters(double[][9], int);
    void applySineFilter();
    void applyCosineFilter();
    void applyNightVisionFilter();
    void applyHopeFilter(bool applyAverage = true);
    void applyPaintbrushFilter(); //not good at all

    //quantize
    void quantize(std::vector<Pixel> palette);

    void glDisplay() const;

    //default mask filers
    static double MASK_AVERAGE[9];
    static double MASK_SOBEL_HORIZONTAL[9];
    static double MASK_SOBEL_VERTICAL[9];
    static double MASK_NEGATIVE_CROSS[9];
    static double MASK_IDENTITY[9];
    static double MASK_SPIN[9];
    static double MASK_CROSS[9];

private:
    double getDistance(const Pixel& p1, const Pixel& p2);
    void copyBuffer();

    int width;
    int height;
    int numPixels;
    std::vector<Pixel> data;    //display buffer
    std::vector<Pixel> buffer;  //working buffer
    std::vector<Pixel> orig;    //original copy of the image
};
