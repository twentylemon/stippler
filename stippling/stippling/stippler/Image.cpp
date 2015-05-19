/**
 * Image.cpp
 * Implementation of an image
 *
 * @author Taras Mychaskiw
 * @version 1.0
 * @since 2013-09-10
**/
#include "Image.h"

double Image::MASK_AVERAGE[9] = { 1,1,1, 1,1,1, 1,1,1 };
double Image::MASK_SOBEL_HORIZONTAL[9] = { 1,2,1, 0,0,0, -1,-2,-1 };
double Image::MASK_SOBEL_VERTICAL[9] = { 1,0,-1, 2,0,-2, 1,0,-1 };
double Image::MASK_NEGATIVE_CROSS[9] = { 0,-1,0, -1,5,-1, 0,-1,0 };
double Image::MASK_IDENTITY[9] = { 1,0,0, 0,1,0, 0,0,1 };
double Image::MASK_SPIN[9] = { 9,8,7, 2,1,6, 3,4,5 };
double Image::MASK_CROSS[9] = { 0,1,0, 1,1,1, 0,1,0 };

Image::Image() {}

// default constructor, creates a black image with the width/height given
Image::Image(int width, int height) : width(width), height(height), numPixels(width*height) {
    data.resize(numPixels);
    buffer.resize(numPixels);
    orig.resize(numPixels);
    for (int i = 0; i < numPixels; i++) {
        data[i] = Pixel(0, 0, 0);
        orig[i] = Pixel(0, 0, 0);
    }
}

// copies the image
Image::Image(const Image& image) : width(image.getWidth()), height(image.getHeight()), numPixels(image.getNumPixels()) {
    data.resize(numPixels);
    buffer.resize(numPixels);
    orig.resize(numPixels);
    for (int i = 0; i < numPixels; i++) {
        orig[i] = data[i] = buffer[i] = image.orig[i];
    }
}

/**
 * Class constructor. Reads the image from the file name specified.
 *
 * @param file the file name containing the image this object should represent
**/
Image::Image(std::string file) {
    std::cerr << "Reading " << file << "...";
    //FIBITMAP* image = FreeImage_Load(FIF_TIFF, file.c_str());
    FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(file.c_str()), file.c_str());
    if (image) {
        RGBQUAD pixel;
        width = FreeImage_GetWidth(image);
        height = FreeImage_GetHeight(image);
        numPixels = width * height;
        data.resize(numPixels);
        buffer.resize(numPixels);
        orig.resize(numPixels);
        int pix = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                FreeImage_GetPixelColor(image, x, y, &pixel);
                data[pix] = Pixel(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue);
                orig[pix] = Pixel(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue);
                pix++;
            }
        }
        FreeImage_Unload(image);
    }
    else {
    }
    std::cerr << "done!" << std::endl;
}


// saves the image to the file specified
void Image::save(std::string file) {
    std::cerr << "Saving to " << file << "...";
    RGBQUAD pixel;
    FIBITMAP* image = FreeImage_Allocate(width, height, 24);
    if (!image) {
        perror("FreeImage_Allocate failure in Image::save()");
        return;
    }
    int pix = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel.rgbRed = data[pix].red;
            pixel.rgbGreen = data[pix].green;
            pixel.rgbBlue = data[pix].blue;
            pix++;
            FreeImage_SetPixelColor(image, x, y, &pixel);
        }
    }
    
    //if (!FreeImage_Save(FIF_TIFF, image, file.c_str()))
    if (!FreeImage_Save(FreeImage_GetFileType(file.c_str()), image, file.c_str()))
        perror("FreeImage_Save failed in Image::save()");
    FreeImage_Unload(image);
    std::cerr << "done!" << std::endl;
}

// copies the buffer into the display
void Image::copyBuffer() {
    std::copy(buffer.begin(), buffer.end(), data.begin());
}


// returns the width of the image
int Image::getWidth() const {
    return width;
}

// returns the height of the image
int Image::getHeight() const {
    return height;
}

// returns the number of pixels in the image
int Image::getNumPixels() const {
    return numPixels;
}

// converts (x,y) coord into a pixel index
int Image::getPixelIndex(int x, int y) const {
    return y*getWidth() + x;
}

// returns the pixel at index i
Image::Pixel& Image::getPixel(int i) {
    return data[i];
}

// returns the pixel at index i
const Image::Pixel& Image::getPixel(int i) const {
    return data[i];
}

// returns the pixel at (x,y)
Image::Pixel& Image::getPixel(int x, int y) {
    return data[getPixelIndex(x, y)];
}

// returns the pixel at (x,y)
const Image::Pixel& Image::getPixel(int x, int y) const {
    return data[getPixelIndex(x, y)];
}


// sets the pixel at index i, the old pixel is returned
Image::Pixel Image::setPixel(int i, const Pixel& pixel) {
    Pixel old = data[i];
    data[i] = pixel;
    return old;
}

// sets the pixel at index i, the old pixel is returned
Image::Pixel Image::setPixel(int x, int y, const Pixel& pixel) {
    return setPixel(getPixelIndex(x, y), pixel);
}

// sets the pixel at index i, the old pixel is returned
Image::Pixel Image::setPixel(int i, GLubyte red, GLubyte green, GLubyte blue) {
    return setPixel(i, Pixel(red, green, blue));
}

// sets the pixel at index i, the old pixel is returned
Image::Pixel Image::setPixel(int x, int y, GLubyte red, GLubyte green, GLubyte blue) {
    return setPixel(getPixelIndex(x, y), red, green, blue);
}

// sets the specified box to the color given, returns the number of pixels in the box
// breaks if min > max
int Image::setPixelRange(int xmin, int xmax, int ymin, int ymax, GLubyte red, GLubyte green, GLubyte blue) {
    return setPixelRange(xmin, xmax, ymin, ymax, Pixel(red, green, blue));
}

// sets the specified box to the color given, returns the number of pixels in the box
// breaks if min > max
int Image::setPixelRange(int xmin, int xmax, int ymin, int ymax, const Pixel& pixel) {
    int box = 0, pix;
    for (int y = std::max(ymin, 0); y < std::min(height, ymax); y++) {
        for (int x = std::max(xmin, 0); x < std::min(width, xmax); x++) {
            box++;
            pix = getPixelIndex(x, y);
            buffer[pix] = pixel;
        }
    }
    copyBuffer();
    return box;
}

// resets this image to the way it was when it was first loaded
void Image::reset() {
    std::cerr << "Resetting image...";
    std::copy(orig.begin(), orig.end(), data.begin());
    std::cerr << "done!" << std::endl;
}

// changes this image to grey scale
void Image::toGreyScale() {
    std::cerr << "Greyscaling image...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        int grey = (pixel.red + pixel.green + pixel.blue) / 3;
        return Pixel(grey, grey, grey);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// changes this image to NTSC grey scale
void Image::toNTSC() {
    std::cerr << "NTSC greyscaling image...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        int grey = (int)(0.299*pixel.red + 0.587*pixel.green + 0.114*pixel.blue);
        return Pixel(grey, grey, grey);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// changes this image to a intensity tone version
void Image::toTone() {
    std::cerr << "Intensity toning image...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        int grey = (int)(0.2126*pixel.red + 0.7152*pixel.green + 0.0722*pixel.blue);
        return Pixel(grey, grey, grey);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// changes this image to monochrome black and white
void Image::toMonochrome() {
    std::cerr << "Monochroming image...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        if (pixel.red + pixel.green + pixel.blue > 3*255/2) {
            return Pixel(255, 255, 255);
        }
        return Pixel(0, 0, 0);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// inverts every pixel color in this image
void Image::toInverted() {
    std::cerr << "Inverting image...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(255 - pixel.red, 255 - pixel.green, 255 - pixel.blue);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// displays only the red channel of this image
void Image::toRedOnly() {
    std::cerr << "Displaying red channel only...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(pixel.red, 0, 0);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// displays only the green channel of this image
void Image::toGreenOnly() {
    std::cerr << "Displaying green channel only...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(0, pixel.green, 0);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// displays only the blue channel of this image
void Image::toBlueOnly() {
    std::cerr << "Displaying blue channel only...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(0, 0, pixel.blue);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// applies the max filter to this image
// max filter: replace each channel of a pixel with the max value of that channel of itself + surrounding 8 pixels
void Image::applyMaxFilter() {
    std::cerr << "Applying max filter...";
    Pixel max;
    int pix, newpix;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pix = getPixelIndex(x, y);
            max = data[pix];
            //loop over the 9 pixels and get the max of each channel
            for (int p = std::max(0,x-1); p < std::min(width,x+2); p++) {
                for (int q = std::max(0,y-1); q < std::min(height,y+2); q++) {
                    newpix = getPixelIndex(p, q);
                    max.red = std::max(max.red, data[newpix].red);
                    max.green = std::max(max.green, data[newpix].green);
                    max.blue = std::max(max.blue, data[newpix].blue);
                }
            }
            buffer[pix] = max;
        }
    }
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// applies the min filter to this image
// min filter: replace each channel of a pixel with the min value of that channel of itself + surrounding 8 pixels
void Image::applyMinFilter() {
    std::cerr << "Applying min filter...";
    Pixel min;
    int pix, newpix;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pix = getPixelIndex(x, y);
            min = data[pix];
            //loop over the 9 pixels and get the max of each channel
            for (int p = std::max(0,x-1); p < std::min(width,x+2); p++) {
                for (int q = std::max(0,y-1); q < std::min(height,y+2); q++) {
                    newpix = getPixelIndex(p, q);
                    min.red = std::min(min.red, data[newpix].red);
                    min.green = std::min(min.green, data[newpix].green);
                    min.blue = std::min(min.blue, data[newpix].blue);
                }
            }
            buffer[pix] = min;
        }
    }
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// brightens the red channel across this image
// factor: how much to brighten the channel
void Image::applyRedIntensifyFilter(double factor) {
    std::cerr << "Applying red intensify filter, factor = " << factor << "...";
    std::transform(data.begin(), data.end(), buffer.begin(), [factor](const Pixel& pixel) {
        return Pixel(std::min(255, (int)(factor * pixel.red)), pixel.green, pixel.blue);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// brightens the green channel across this image
// factor: how much to brighten the channel
void Image::applyGreenIntensifyFilter(double factor) {
    std::cerr << "Applying green intensify filter, factor = " << factor << "...";
    std::transform(data.begin(), data.end(), buffer.begin(), [factor](const Pixel& pixel) {
        return Pixel(pixel.red, std::min(255, (int)(factor * pixel.green)), pixel.blue);
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// brightens the blue channel across this image
// factor: how much to brighten the channel
void Image::applyBlueIntensifyFilter(double factor) {
    std::cerr << "Applying blue intensify filter, factor = " << factor << "...";
    std::transform(data.begin(), data.end(), buffer.begin(), [factor](const Pixel& pixel) {
        return Pixel(pixel.red, pixel.green, std::min(255, (int)(factor * pixel.blue)));
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

/**
 applies the mask filter specified.
 mask filter: take a grid, center it on a pixel, multiply the values of the grid against each
 channel, sum up results, divide by sum of grid values, then assign value to the pixel we
 are currently working on. eg
       3 1 1    center the mask on pixel P (will have the value 2), go through each neighbour and
 mask: 0 2 0    multiply that red channel by the number in the grid, keeping sum. Divide sum by the
       1 1 3    sum of the values in mask (12 here), then assign normalized result to P.red, repeat for G/B
**/
void Image::applyMaskFilter(double mask[9]) {
    std::cerr << "Applying mask...";
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0)
            std::cerr << std::endl << "\t";
        std::cerr << mask[i] << " ";
    }
    double maskSum;
    int redSum, greenSum, blueSum;
    int pix, newpix, pos;
    int maskSize = 3;
    int offset = maskSize/2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pix = getPixelIndex(x, y);
            maskSum = redSum = greenSum = blueSum = 0;
            pos = 0;
            //loop through the neighbours
            for (int p = std::max(0,y-offset); p < std::min(height,y+offset+1); p++) {
                for (int q = std::max(0,x-offset); q < std::min(width,x+offset+1); q++) {
                    newpix = getPixelIndex(q, p);
                    maskSum += mask[pos];
                    redSum += (int)(mask[pos] * data[newpix].red);
                    greenSum += (int)(mask[pos] * data[newpix].green);
                    blueSum += (int)(mask[pos] * data[newpix].blue);
                    pos++;
                }
            }
            if (maskSum == 0)
                maskSum = 1;
            
            buffer[pix] = Pixel((GLubyte)std::min(255.0, std::max(0.0, redSum / maskSum)),
                (GLubyte)std::min(255.0, std::max(0.0, greenSum / maskSum)),
                (GLubyte)std::min(255.0, std::max(0.0, blueSum / maskSum)));
        }
    }
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

/**
 applies both mask filters. The resulant pixel intensity is sqrt(m1*m1 + m2*m2) where mi is the intensity
 of the ith filter.
 masks: 2d array of masks to apply
 num: the number of masks being applied
**/
void Image::applyManyMaskFilters(double masks[][9], int num) {
    std::cerr << "Applying " << num << " masks..." << std::endl;
    Pixel** buffers = new Pixel*[num];
    Pixel* pre = new Pixel[numPixels]; //the current state of the image
    memcpy(pre, data.data(), numPixels * sizeof Pixel);
    for (int i = 0; i < num; i++) {
        buffers[i] = new Pixel[numPixels];
        memcpy(data.data(), pre, numPixels * sizeof Pixel);        //reset the image
        std::cerr << "\t";
        applyMaskFilter(masks[i]);                          //apply the ith filter
        memcpy(buffers[i], data.data(), numPixels * sizeof Pixel); //save the result of the application of the filter
    }
    memcpy(data.data(), pre, numPixels * sizeof Pixel);    //may cause flashing... oh well

    int red, green, blue;
    for (int i = 0; i < numPixels; i++) {
        red = green = blue = 0;
        for (int j = 0; j < num; j++) {
            red += buffers[j][i].red * buffers[j][i].red;
            green += buffers[j][i].green * buffers[j][i].green;
            blue += buffers[j][i].blue * buffers[j][i].blue;
        }
        buffer[i] = Pixel((GLubyte)std::sqrt(red), (GLubyte)std::sqrt(green), (GLubyte)std::sqrt(blue));
    }
    copyBuffer();
    delete[] pre;
    delete[] buffers;
    std::cerr << "done!" << std::endl;
}

// applies the sine function to each pixel in the image. sin is normalized to be between 0 and 255,
// and the period of the sine wave in 256. So it goes through one cycle in the range 0-255.
void Image::applySineFilter() {
    std::cerr << "Applying sine filter...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(
            (GLubyte)(127.5 * (std::sin(2*3.1415926535897 / 256 * pixel.red) + 1)),
            (GLubyte)(127.5 * (std::sin(2*3.1415926535897 / 256 * pixel.green) + 1)),
            (GLubyte)(127.5 * (std::sin(2*3.1415926535897 / 256 * pixel.blue) + 1)));
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// applies the cosine function to each pixel in the image. cos is normalized to be between 0 and 255,
// and the period of the cosine wave in 256. So it goes through one cycle in the range 0-255.
void Image::applyCosineFilter() {
    std::cerr << "Applying cosine filter...";
    std::transform(data.begin(), data.end(), buffer.begin(), [](const Pixel& pixel) {
        return Pixel(
            (GLubyte)(127.5 * (std::cos(2*3.1415926535897 / 256 * pixel.red) + 1)),
            (GLubyte)(127.5 * (std::cos(2*3.1415926535897 / 256 * pixel.green) + 1)),
            (GLubyte)(127.5 * (std::cos(2*3.1415926535897 / 256 * pixel.blue) + 1)));
    });
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// applies a custom night vision filter
void Image::applyNightVisionFilter() {
    std::cerr << "Applying nightvision filter..." << std::endl;
    std::cerr << "\t";
    toGreyScale();
    std::cerr << "\t";
    applyGreenIntensifyFilter(1.75);
    std::cerr << "done!" << std::endl;
}

// my attempt at a hope filter. Hah.
// Hope filter, sort of produces the Obama "Hope" poster of this image. It's kind of like
// a quantization of the image, but instead of the closest color, it changes each pixel to
// to whichever highest threshold it passed. So say the thresholds are 0,85,170,255 and a
// pixel has a greyscale color of 169, the color will be same as applied to one that had 85.
void Image::applyHopeFilter(bool applyAverage) {
    std::cerr << "Applying Obama Hope filter...";
    Pixel space[4];
    space[0].red = space[0].green = space[0].blue = 255;
    space[1].red = 220; space[1].green = space[1].blue = 30;
    space[2].red = 100; space[2].green = space[2].blue = 150;
    space[3].red = 0; space[3].green = 50; space[3].blue = 75;

    //create the divisions
    int min = 255, max = 0, pix;
    for (int i = 0; i < numPixels; i++) {
        pix = (data[i].red + data[i].green + data[i].blue) / 3;
        if (pix < min)
            min = pix;
        if (pix > max)
            max = pix;
    }

    int thres[5];
    thres[0] = max;
    thres[1] = 3*max/4 + min;
    thres[2] = 2*max/4 + min;
    thres[3] = 1*max/4 + min;
    thres[4] = min;

    for (int i = 0; i < numPixels; i++) {
        pix = (data[i].red + data[i].green + data[i].blue) / 3;
        for (int j = 0; j < 4; j++) {
            if (pix <= thres[j] && pix >= thres[j+1])
                buffer[i] = space[j];
        }
    }
    copyBuffer();
    if (applyAverage) {
        std::cerr << std::endl << "\t";
        applyMaskFilter(Image::MASK_AVERAGE);
    }
    std::cerr << "done!" << std::endl;
}

// applies a paintbrush filter to this image
void Image::applyPaintbrushFilter() {
    std::cerr << "Applying paintbrush filter...";
    int pix, newpix, offset = 2;
    //for (int y = 2; y < height; y = std::min(y+5, height)) {
    //    for (int x = 2; x < width; x = std::min(x+5, width)) {
    for (int y = 0; y < height; y = std::min(y+1, height)) {
        for (int x = 0; x < width; x = std::min(x+1, width)) {
            pix = getPixelIndex(x, y);
            for (int p = std::max(0,x-offset); p < std::min(width,x+offset+1); p++) {
                for (int q = std::max(0,y-offset); q < std::min(height,y+offset+1); q++) {
                    newpix = getPixelIndex(p, q);
                    buffer[newpix].red = (data[pix].red + data[newpix].red) / 2;
                    buffer[newpix].green = (data[pix].green + data[newpix].green) / 2;
                    buffer[newpix].blue = (data[pix].blue + data[newpix].blue) / 2;
                }
            }
        }
    }
    copyBuffer();
    applyMaskFilter(Image::MASK_AVERAGE);
    std::cerr << "done! I guess..." << std::endl;
}

// performs quantization of this image
// quantize: create a color palette, then for each pixel, change that pixels color to the closest color in the palette.
void Image::quantize(std::vector<Pixel> palette) {
    int size = palette.size();
    std::cerr << "Quantizing image to " << size << " colors..." << std::endl;
    for (int i = 0; i < size; i++) {
        std::cerr << "\t(" << (int)palette[i].red << "," << (int)palette[i].green << "," << (int)palette[i].blue << ")" << std::endl;
    }
    Pixel best;
    double dist, bestDist;
    for (int i = 0; i < numPixels; i++) {
        //distance between white and black is ~441
        bestDist = 500 * 500;   //getDistance() returns distance squared, so square 500
        for (int j = 0; j < size; j++) {
            dist = getDistance(data[i], palette[j]);
            if (bestDist > dist) {
                bestDist = dist;
                best = palette[j];
            }
        }
        buffer[i] = best;
    }
    copyBuffer();
    std::cerr << "done!" << std::endl;
}

// draws this image
void Image::glDisplay() const {
    glDrawPixels(width, height, GL_RGB /* GL_RGBA */, GL_UNSIGNED_BYTE, (GLubyte*)data.data());
}

// returns the squared euclidean distance between the two Pixels.
double Image::getDistance(const Pixel& p1, const Pixel& p2) {
    int r = (p1.red - p2.red) * (p1.red - p2.red);
    int g = (p1.green - p2.green) * (p1.green - p2.green);
    int b = (p1.blue - p2.blue) * (p1.blue - p2.blue);
    //return sqrt(r + g + b);
    return r + g + b; //screw the sqrt, only used for comparisons anyways
}
