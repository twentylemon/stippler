

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

// class to do weighted voronoi stippling
class Stippler
{
public:
    Stippler(int width, int height, int numStipples);

    int getWidth() const;
    int getHeight() const;
    const std::vector<Stipple>& getStipples() const;

    void setWidth(int width);
    void setHeight(int height);

    std::pair<double,double> lloydsMethod();

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
