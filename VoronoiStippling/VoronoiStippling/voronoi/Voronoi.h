

#pragma once
#include <GL/glut.h>

#include <stippler/Stippler.h>
class Stippler;

class Voronoi
{
public:
    Voronoi();


    void calculateDiagram(const Stippler& stippler);
};
