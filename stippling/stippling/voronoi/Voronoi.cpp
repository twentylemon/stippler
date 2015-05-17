

#include "Voronoi.h"

const int VISIBILITY_FACTOR = 1000;

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
GLdouble Voronoi::getRadius() const {
    return radius;
}

// returns the height of the cones to draw
GLdouble Voronoi::getHeight() const {
    return height;
}

// returns the number of slices in the cone; around the z axis
GLint Voronoi::getSlices() const {
    return slices;
}

// returns the number of stacks in the cone; up the z axis
GLint Voronoi::getStacks() const {
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
std::array<GLubyte, 3> Voronoi::idx2Color(int idx) const {
    idx *= VISIBILITY_FACTOR;
    std::array<GLubyte, 3> ary = {{ idx & 0x0000FF, (idx & 0x00FF00) >> 8, (idx & 0xFF0000) >> 16 }};
    return ary;
}

// translates the color into an index
int Voronoi::color2Idx(const std::array<GLubyte, 3>& color) const {
    int idx = color[0] | (color[1] << 8) | (color[2] << 16);
    return idx / VISIBILITY_FACTOR;
}

// translates the color into an index
int Voronoi::color2Idx(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) const {
    return ( red | (green << 8) | (blue << 16) ) / VISIBILITY_FACTOR;
}


// initializes the opengl enviroment for calculating the voronoi diagram
void Voronoi::glInit(const Stippler& stippler) const {
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLoadIdentity();
    glOrtho(0, stippler.getWidth(), stippler.getHeight(), 0, -5*stippler.getHeight(), 5*stippler.getHeight());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// calculates the voronoi diagram given the points in the stippler
// the size of the plane is the same as the size of the stippler image
void Voronoi::calculateDiagram(const Stippler& stippler) {
    glInit(stippler);
    int i = 0;
    std::for_each(stippler.getStipples().begin(), stippler.getStipples().end(), [&i, this](const Stipple& stipple){
        glPushMatrix();
        glColor3ubv(idx2Color(i++).data());
        glTranslatef(stipple.getX(), stipple.getY(), 0.0f);
        glutSolidCone(getRadius(), getHeight(), getSlices(), getStacks());
        glPopMatrix();
    });
    glFlush();

    pixels.resize(4 * stippler.getWidth() * stippler.getHeight());
    glReadPixels(0, 0, stippler.getWidth(), stippler.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    // pixels are returned in row order, from lowest to highest row, from left to right in each row

    // diagram holds the stipple id of each pixel in the image
    diagram.resize(stippler.getWidth() * stippler.getHeight());
    for (unsigned i = 0; i < pixels.size(); i+=4) {
        diagram[i/4] = color2Idx(pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]);
    }
}

// moves the stipples in the stippler to the center of tone of each voronoi region
// automatically makes a call to `calculateDiagram(stippler)`
void Voronoi::redistributeStipples(Stippler& stippler) {
    calculateDiagram(stippler);
    stippler.resetVoronoi();
    for (unsigned i = 0; i < diagram.size(); i++) {
        int x = i % stippler.getWidth();
        int y = stippler.getHeight() - i / stippler.getWidth(); // subtract due to opengl (0,0) in bottom left
        Stipple& stipple = stippler[diagram[i]];
        stipple.setMass(stipple.getMass() + 1.0f);
        stipple.setXMoment(stipple.getXMoment() + x);
        stipple.setYMoment(stipple.getYMoment() + y);
    }
    for (Stipple& stipple : stippler.getStipples()) {
        stipple.setX(stipple.getXMoment() / stipple.getMass());
        stipple.setY(stipple.getYMoment() / stipple.getMass());
    }
}
