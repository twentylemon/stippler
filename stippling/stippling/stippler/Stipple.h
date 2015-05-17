

#pragma once
#include <iostream>


// a simple stipple wrapper
// basically just the position, colour and radius of the point
// @author Taras Mychaskiw
class Stipple
{
public:
    Stipple(float x, float y, float radius, float grey);
    Stipple(float x, float y, float radius, float red, float green, float blue);

    float getX() const;
    float getY() const;
    float getRadius() const;

    float getRed() const;
    float getGreen() const;
    float getBlue() const;
    float getGrey() const;

    float getMass() const;
    float getXMoment() const;
    float getYMoment() const;

    void setX(float x);
    void setY(float y);
    void setRadius(float radius);

    void setRed(float red);
    void setGreen(float green);
    void setBlue(float blue);
    void setGrey(float grey);

    void setMass(float mass);
    void setXMoment(float xMoment);
    void setYMoment(float yMoment);

    void resetVoronoi();

    friend std::ostream& operator<<(std::ostream& out, const Stipple& stipple);

private:
    float x, y;                 // x and y positions in the image
    float radius;               // radius of the stipple
    float red, green, blue;     // colour of the stipple
    float mass, xMom, yMom;     // moments of the voronoi region
};
