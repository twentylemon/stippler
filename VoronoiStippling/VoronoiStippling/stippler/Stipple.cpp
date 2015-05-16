

#include "Stipple.h"

// main greyscale constructor
// x: the x position of the stipple
// y: the y position of the stipple
// radius: the size of the stipple
// grey: the greyscale colour of the stipple, in [0..1] where 1 is white
Stipple::Stipple(float x, float y, float radius, float grey) :
    x(x),
    y(y),
    radius(radius),
    red(grey), green(grey), blue(grey)
{
}


// colour constructor
// x: the x position of the stipple
// y: the y position of the stipple
// radius: the size of the stipple
// r,g,b: red/green/blue colours of the stipple, each in [0..1]
Stipple::Stipple(float x, float y, float radius, float red, float green, float blue) :
    x(x),
    y(y),
    radius(radius),
    red(red), green(green), blue(blue)
{
}

// returns the x position of the stipple
float Stipple::getX() const {
    return x;
}

// returns the y position of the stipple
float Stipple::getY() const {
    return y;
}

// returns the radius of the stipple
float Stipple::getRadius() const {
    return radius;
}

// returns the red channel value of the stipple
float Stipple::getRed() const {
    return red;
}

// returns the green channel value of the stipple
float Stipple::getGreen() const {
    return green;
}

// returns the blue channel value of the stipple
float Stipple::getBlue() const {
    return blue;
}

// returns the greyscale colour of this stipple
float Stipple::getGrey() const {
    return (getRed() + getGreen() + getBlue()) / 3.0;
}

// sets the new x position of this stipple
void Stipple::setX(float x) {
    this->x = x;
}

// sets the new y position of this stipple
void Stipple::setY(float y) {
    this->y = y;
}

// sets the new radius of this stipple
void Stipple::setRadius(float radius) {
    this->radius = radius;
}

// sets the new red channel value of this stipple
void Stipple::setRed(float red) {
    this->red = red;
}

// sets the new green channel value of this stipple
void Stipple::setGreen(float green) {
    this->green = green;
}

// sets the new blue channel value of this stipple
void Stipple::setBlue(float blue) {
    this->blue = blue;
}

// sets the new greyscale colour of this stipple
void Stipple::setGrey(float grey) {
    setRed(grey);
    setGreen(grey);
    setBlue(grey);
}

// overriding `<<` for stipples, will be used to write svg files
std::ostream& operator<<(std::ostream& out, const Stipple& stipple) {
    out << "<circle"
        << " cx=\"" << stipple.getX() << "\""
        << " cy=\"" << stipple.getY() << "\""
        << " r=\"" << stipple.getRadius() << "\""
        << " fill=\"rgb(" << (unsigned int)(255 * stipple.getRed()) << "," << (unsigned int)(255 * stipple.getGreen()) << "," << (unsigned int)(255 * stipple.getBlue()) << ")\""
        << "/>";
    return out;
}
