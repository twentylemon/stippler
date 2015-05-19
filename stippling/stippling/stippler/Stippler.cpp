

#include "Stippler.h"
static Voronoi voronoi;

Stippler::Stippler() :
    width(0),
    height(0),
    image(Image(0, 0))
{
}

// initializes the stippler
Stippler::Stippler(int width, int height, int numStipples) : 
    width(width),
    height(height),
    image(Image(width, height))
{
    initStipples(numStipples);
}

// initializes the stippler using the file as the image to stipple
Stippler::Stippler(std::string file, int numStipples) {
    image = Image(file);
    setWidth(image.getWidth());
    setHeight(image.getHeight());
    initStipples(numStipples);
}

// returns the width of the image for this stippler
int Stippler::getWidth() const {
    return width;
}

// returns the height of the image for this stippler
int Stippler::getHeight() const {
    return height;
}

// returns the number of stipples
int Stippler::getNumStipples() const {
    return stipples.size();
}

// returns the list of stipples
std::vector<Stipple>& Stippler::getStipples() {
    return stipples;
}

// returns the list of stipples
const std::vector<Stipple>& Stippler::getStipples() const {
    return stipples;
}

// returns the stipple with the given index
Stipple& Stippler::operator[](int index) {
    return stipples[index];
}

// returns the stipple with the given index
const Stipple& Stippler::operator[](int index) const {
    return stipples[index];
}

// returns the image
const Image& Stippler::getImage() const {
    return image;
}

// sets the new image width
void Stippler::setWidth(int width) {
    this->width = width;
}

// sets the new image height
void Stippler::setHeight(int height) {
    this->height = height;
}

// returns how much tone there is in the source image at the specified pixel coord
float Stippler::getImageTone(int i) const {
    Image::Pixel pixel = image.getPixel(i);
    return 1.0f - (0.2126f*pixel.red + 0.7152f*pixel.green + 0.0722f*pixel.blue) / 255.0f;
}

// returns how much tone there is in the source image at the specified pixel coord
float Stippler::getImageTone(int x, int y) const {
    Image::Pixel pixel = image.getPixel(x, y);
    return 1.0f - (0.2126f*pixel.red + 0.7152f*pixel.green + 0.0722f*pixel.blue) / 255.0f;
}


// initializes the list of stipples, randomly placing them in the plane for now
void Stippler::initStipples(int numStipples) {
    while (stipples.size() < (unsigned)numStipples) {
        float x = getWidth() * Random::nextFloat();
        float y = getHeight() * Random::nextFloat();
        if (Random::nextInt(256) <= getImageTone((int)x, (int)y)) {
            stipples.push_back(Stipple(x, y, 1.0, 0.0));
        }
    }
}


// resets all the stipples to prep for a new interation of lloyds method
void Stippler::resetVoronoi() {
    for (Stipple& stipple : getStipples()) {
        stipple.resetVoronoi();
    }
}


// performs one iteration of lloyds method on the stipples
// calculates the voronoi diagram, moves stipples to centroids
// returns the <avg,max> movement of the stipples as a pair
std::pair<float,float> Stippler::lloydsMethod() {
    voronoi.redistributeStipples(*this);
    return std::make_pair(voronoi.getAvgMovement(), voronoi.getMaxMovement());
}


// onces all the stipples are in their spots, call this method to resize the stipples
// depending on the source image
void Stippler::finalize(const FinalizeParams& params) {
    std::for_each(stipples.begin(), stipples.end(), [&params,this](Stipple& stipple){
        if (params.radiusMode == RADIUS_MODE_LINEAR) {
            stipple.setRadius(params.radiusScale * stipple.getMass() / voronoi.getMaxMass());
        }
        else if (params.radiusMode == RADIUS_MODE_TRIG) {
            stipple.setRadius(params.radiusScale * 0.5f*(1.0f-std::cos(3.1415926535897f*stipple.getMass()/voronoi.getMaxMass())));
        }
    });
}


// overriding `<<` for the Stippler, writes the stipplers as an svg into the stream
std::ostream& operator<<(std::ostream& out, const Stippler& stippler) {
    out << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << stippler.getWidth() << "\" height=\"" << stippler.getHeight() << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
    for (Stipple stipple : stippler.getStipples()) {
        out << stipple << std::endl;
    }
    out << "</svg>" << std::endl;
    return out;
}
