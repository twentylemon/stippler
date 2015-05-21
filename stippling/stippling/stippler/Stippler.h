

#pragma once

#include <ppl.h>

#include <map>
#include <atomic>
#include <vector>
#include <utility>
#include <iostream>

#include "Image.h"
#include "Stipple.h"
#include "../util/Random.h"

#include "../voronoi/Voronoi.h"
class Voronoi;

#include "FinalizeParams.h"

// class to do weighted voronoi stippling
class Stippler
{
public:
    Stippler();
    Stippler(int width, int height, unsigned int numStipples);
    Stippler(std::string file, unsigned int numStipples);

    int getWidth() const;
    int getHeight() const;
    int getNumStipples() const;
    std::vector<Stipple>& getStipples();
    const std::vector<Stipple>& getStipples() const;
    Image& getImage();
    const Image& getImage() const;

    Stipple& operator[](int index);
    const Stipple& operator[](int index) const;

    void setWidth(int width);
    void setHeight(int height);
    
    float getImageTone(int i) const;
    float getImageTone(int x, int y) const;

    void resetVoronoi();
    std::pair<float,float> lloydsMethod();

    void initStipples(unsigned int numStipples);
    void finalize(const FinalizeParams& params);

    std::string toTSP(float threshold = 0.15f) const;

    friend std::ostream& operator<<(std::ostream& out, const Stippler& stippler);

private:
    typedef std::pair<float,float> Point;
    typedef std::pair<Point,Point> Edge;

    int width, height;
    std::vector<Stipple> stipples;

    Image original, ntsc;
};
