

#pragma once
#include <vector>

#include <GL/glut.h>

#include <stippler/Stippler.h>
class Stippler;

class Voronoi
{
public:
    Voronoi();
    Voronoi(double radius, double height, int slices, int stacks);

    void calculateDiagram(const Stippler& stippler);
    int idx2Color(int idx);

    GLdouble getRadius();
    GLdouble getHeight();
    GLint getSlices();
    GLint getStacks();

    void setRadius(GLdouble radius);
    void setHeight(GLdouble height);
    void setSlices(GLint slices);
    void setStacks(GLint stacks);

private:
    void glInit(const Stippler& stippler);

    GLdouble radius, height;
    GLint slices, stacks;
};
