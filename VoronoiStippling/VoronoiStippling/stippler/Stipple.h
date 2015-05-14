

#pragma once
#include <iostream>


// a simple stipple wrapper
// basically just the position, colour and radius of the point
// @author Taras Mychaskiw
class Stipple
{
public:
    Stipple(double x, double y, double radius, double grey);
    Stipple(double x, double y, double radius, double red, double green, double blue);

    double getX() const;
    double getY() const;
    double getRadius() const;

    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    double getGrey() const;

    void setX(double x);
    void setY(double y);
    void setRadius(double radius);

    void setRed(double red);
    void setGreen(double green);
    void setBlue(double blue);
    void setGrey(double grey);

    friend std::ostream& operator<<(std::ostream& out, const Stipple& stipple);

private:
    double x, y;                //x and y positions in the image
    double radius;              //radius of the stipple
    double red, green, blue;    //colour of the stipple
};
