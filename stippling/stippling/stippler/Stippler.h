

#pragma once

#include <ppl.h>

#include <map>
#include <atomic>
#include <vector>
#include <utility>
#include <iostream>

#include "Stipple.h"
#include "VoronoiDiagramGenerator.h"
#include <util/Random.h>

#include <voronoi/Voronoi.h>
class Voronoi;

// class to do weighted voronoi stippling
class Stippler
{
public:
    Stippler();
    Stippler(int width, int height, int numStipples);

    int getWidth() const;
    int getHeight() const;
    std::vector<Stipple>& getStipples();
    const std::vector<Stipple>& getStipples() const;

    Stipple& operator[](int index);
    const Stipple& operator[](int index) const;

    void setWidth(int width);
    void setHeight(int height);

    void resetVoronoi();
    std::pair<float,float> lloydsMethod();

    friend std::ostream& operator<<(std::ostream& out, const Stippler& stippler);

private:
    typedef std::pair<float,float> Point;
    typedef std::pair<Point,Point> Edge;

    void initStipples(int numStipples);

    int width, height;
    std::vector<Stipple> stipples;
    std::vector<float> xValues, yValues;
    std::map<Point, std::vector<Edge>> edgeMap;
};