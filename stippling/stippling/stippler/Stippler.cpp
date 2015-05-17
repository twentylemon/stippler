

#include "Stippler.h"

Stippler::Stippler() :
    width(0),
    height(0)
{
}

// initializes the stippler
Stippler::Stippler(int width, int height, int numStipples) : 
    width(width),
    height(height)
{
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

// sets the new image width
void Stippler::setWidth(int width) {
    this->width = width;
}

// sets the new image height
void Stippler::setHeight(int height) {
    this->height = height;
}

// initializes the list of stipples, randomly placing them in the plane for now
void Stippler::initStipples(int numStipples) {
    for (int i = 0; i < numStipples; i++) {
        xValues.push_back(getWidth() * Random::nextFloat());
        yValues.push_back(getHeight() * Random::nextFloat());
        stipples.push_back(Stipple(xValues[i], yValues[i], 1.0, 0.0));
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
// returns the <total,max> movement of the stipples as a pair
std::pair<float,float> Stippler::lloydsMethod() {
    VoronoiDiagramGenerator gen;
    gen.generateVoronoi(xValues.data(), yValues.data(), xValues.size(), 0.0f, getWidth()-1.0f, 0.0f, getHeight()-1.0f);

    edgeMap.clear();
    Edge edge;
    Point p1, p2;
    while (gen.getNext(edge.first.first, edge.first.second, edge.second.first, edge.second.second, p1.first, p1.second, p2.first, p2.second)) {
        if (edge.first == edge.second) {
            continue;
        }
        if (edgeMap.find(p1) == edgeMap.end()) {
            edgeMap[p1] = std::vector<Edge>();
        }
        if (edgeMap.find(p2) == edgeMap.end()) {
            edgeMap[p2] = std::vector<Edge>();
        }
        edgeMap[p1].push_back(edge);
        edgeMap[p2].push_back(edge);
    }

    std::atomic<float> max, avg;
    concurrency::parallel_for_each(edgeMap.begin(), edgeMap.end(), [this](std::pair<Point,std::vector<Edge>> item){
        // calculate centroid
    });
    return std::make_pair(0.0f, 0.0f);
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
