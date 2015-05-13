

#include "Stipple.h"

// main greyscale constructor
// x: the x position of the stipple
// y: the y position of the stipple
// radius: the size of the stipple
// grey: the greyscale colour of the stipple, in [0..1] where 1 is white
Stipple::Stipple(double x, double y, double radius, double grey) :
    x(x),
    y(y),
    radius(radius),
    red(grey), green(grey), blue(grey) {
}


// colour constructor
// x: the x position of the stipple
// y: the y position of the stipple
// radius: the size of the stipple
// r,g,b: red/green/blue colours of the stipple, each in [0..1]
Stipple::Stipple(double x, double y, double radius, double red, double green, double blue) :
    x(x),
    y(y),
    radius(radius),
    red(red), green(green), blue(blue) {
}

// returns the x position of the stipple
double Stipple::getX() {
    return x;
}

// returns the y position of the stipple
double Stipple::getY() {
    return y;
}

// returns the radius of the stipple
double Stipple::getRadius() {
    return radius;
}

// returns the red channel value of the stipple
double Stipple::getRed() {
    return red;
}

// returns the green channel value of the stipple
double Stipple::getGreen() {
    return green;
}

// returns the blue channel value of the stipple
double Stipple::getBlue() {
    return blue;
}

// returns the greyscale colour of this stipple
double Stipple::getGrey() {
    return (getRed() + getGreen() + getBlue()) / 3.0;
}

