

#pragma once


// a simple stipple wrapper
// basically just the position, colour and radius of the point
// @author Taras Mychaskiw
class Stipple
{
public:
    Stipple(double x, double y, double radius, double grey);
    Stipple(double x, double y, double radius, double red, double green, double blue);

    double getX();
    double getY();
    double getRadius();

    double getRed();
    double getGreen();
    double getBlue();
    double getGrey();

private:
    double x, y;                //x and y positions in the image
    double radius;              //radius of the stipple
    double red, green, blue;    //colour of the stipple
};
