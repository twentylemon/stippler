

#include "Voronoi.h"

// initializes the class with some default cone parameters
Voronoi::Voronoi() : 
    radius(200.0),
    height(25.0),
    slices(40),
    stacks(10)
{
}

// initializes the class with the given cone parameters
Voronoi::Voronoi(double radius, double height, int slices, int stacks) : 
    radius(radius),
    height(height),
    slices(slices),
    stacks(stacks)
{
}

// returns the radius of the cones to draw
GLdouble Voronoi::getRadius() {
    return radius;
}

// returns the height of the cones to draw
GLdouble Voronoi::getHeight() {
    return height;
}

// returns the number of slices in the cone; around the z axis
GLint Voronoi::getSlices() {
    return slices;
}

// returns the number of stacks in the cone; up the z axis
GLint Voronoi::getStacks() {
    return stacks;
}

// sets the new radius of the cone
void Voronoi::setRadius(GLdouble radius) {
    this->radius = radius;
}

// sets the new height of the cone
void Voronoi::setHeight(GLdouble height) {
    this->height = height;
}

// sets the new number of slices of the cone; around the z axis
void Voronoi::setSlices(GLint slices) {
    this->slices = slices;
}

// sets the new number of stacks of the cone; up the z axis
void Voronoi::setStacks(GLint stacks) {
    this->stacks = stacks;
}

// translates the index into a color
// this can probably get removed altogether; just use the index counter as the color
int Voronoi::idx2Color(int idx) {
    if (idx == 0) {
        return 0xFF0000;
    }
    else if (idx == 1) {
        return 0x00FF00;
    }
    return 0x0000FF;
}


// initializes the opengl enviroment for calculating the voronoi diagram
void Voronoi::glInit(const Stippler& stippler) {
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLoadIdentity();
    glOrtho(0, stippler.getWidth(), stippler.getHeight(), 0, -100, 100);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// calculates the voronoi diagram given the points in the stippler
// the size of the plane is the same as the size of the stippler image
void Voronoi::calculateDiagram(const Stippler& stippler) {
    glInit(stippler);
    int i = 0;
    std::for_each(stippler.getStipples().begin(), stippler.getStipples().end(), [&i, this](const Stipple& stipple){
        glPushMatrix();
        int color = idx2Color(i++);
        std::cout << (color & 0x0000FF) << "," << ((color & 0x00FF00) >> 8) << "," << ((color & 0xFF0000) >> 16) << std::endl;
        glColor3ub(color & 0x0000FF, (color & 0x00FF00) >> 8, (color & 0xFF0000) >> 16);
        glTranslatef(stipple.getX(), stipple.getY(), 0.0f);
        glutSolidCone(getRadius(), getHeight(), getSlices(), getStacks());
        glPopMatrix();
    });
}
