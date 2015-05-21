

#pragma once
#include <array>
#include <vector>

class Stippler;

#include <GL/glut.h>

class Voronoi
{
public:
    Voronoi();
    Voronoi(double radius, double height, int slices, int stacks);

    void calculateDiagram(const Stippler& stippler);
    void redistributeStipples(Stippler& stippler);

    std::array<GLubyte, 3> idx2Color(int idx) const;
    int color2Idx(const std::array<GLubyte, 3>& color) const;
    int color2Idx(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) const;

    GLdouble getRadius() const;
    GLdouble getHeight() const;
    GLint getSlices() const;
    GLint getStacks() const;

    void setRadius(GLdouble radius);
    void setHeight(GLdouble height);
    void setSlices(GLint slices);
    void setStacks(GLint stacks);
    
    float getMaxMovement() const;
    float getAvgMovement() const;
    float getMaxMass() const;
    float getAvgMass() const;

    const std::vector<int>& getDiagram() const;

private:
    void glInit(const Stippler& stippler) const;

    GLdouble radius, height;
    GLint slices, stacks;
    std::vector<GLubyte> pixels;
    std::vector<int> diagram;
    float maxMovement, avgMovement;
    float maxMass, avgMass;
};

// moving include stippler here fixes undefined class bugs
#include "../stippler/Stippler.h"
