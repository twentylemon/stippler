
#include "main.h"

Stippler stippler;
Stippler cyan, magenta, yellow, black;
FinalizeParams params;

// recovers the list of cities from the tsp file
std::map<int, std::pair<float,float>> getMap(std::string tspFile) {
    std::ifstream tsp(tspFile);
    std::string line;
    while (std::getline(tsp, line)) {
        if (line == "NODE_COORD_SECTION") {
            break;
        }
    }
    int idx;
    float x, y;
    std::map<int, std::pair<float,float>> points;
    while (tsp >> idx >> x >> y) {
        points[idx-1] = std::make_pair(x, (float)stippler.getHeight() - y);
    }
    tsp.close();
    return points;
}

// displays the points and writes the points to the given svg file
void writePoints(std::string solnFile, std::map<int, std::pair<float,float>> points, std::ofstream& out, int red, int green, int blue, int alpha = 255) {
    std::ifstream soln(solnFile);
    glColor3ub(red, green, blue);
    glBegin(GL_LINE_LOOP);
    int idx = 0, prev = -1;
    while (soln >> idx) {
        glVertex2f(points[idx].first, points[idx].second);
        if (prev != -1) {
            out << "<line x1=\"" << std::to_string(points[prev].first) << "\" y1=\"" << std::to_string(points[prev].second) << "\""
                << " x2=\"" << std::to_string(points[idx].first) << "\" y2=\"" << std::to_string(points[idx].second) << "\""
                << " style=\"stroke:rgba(" << red << "," << green << "," << blue << "," << alpha << ");stroke-width:2\"/>" << std::endl;
        }
        prev = idx;
    }
    glEnd();
    soln.close();
}

// handles tsp art for black only, soln file is soln.cyc
void displayTSP() {
    std::ofstream svg("../../tsp.svg");
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << stippler.getWidth() << "\" height=\"" << stippler.getHeight() << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1.5f);
    writePoints("../../soln.cyc", getMap("../../output.tsp"), svg, 0, 0, 0);
    svg << "</svg>" << std::endl;
    svg.close();
    glEnd();
    glFlush();
}

// handles color tsp art, soln files are [cyan|magenta|yellow|black].cyc
void displayColorTSP() {
    std::ofstream svg("../../tsp.svg");
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << stippler.getWidth() << "\" height=\"" << stippler.getHeight() << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1.5f);
    writePoints("../../yellow.cyc", getMap("../../yellow.tsp"), svg, 255, 255, 0, 255);
    writePoints("../../magenta.cyc", getMap("../../magenta.tsp"), svg, 255, 0, 255, 64);
    writePoints("../../cyan.cyc", getMap("../../cyan.tsp"), svg, 0, 255, 255, 64);
    writePoints("../../black.cyc", getMap("../../black.tsp"), svg, 0, 0, 0, 128);
    svg << "</svg>" << std::endl;
    svg.close();
    glEnd();
    glFlush();
    std::cout << "displayed" << std::endl;
}


// writes the svg and tsp files
void save(Stippler& stippler, std::string file, float threshold = 0.15f) {
    std::ofstream svg("../../" + file + ".svg");
    svg << stippler;
    svg.close();
    std::ofstream tsp("../../" + file + ".tsp");
    tsp << stippler.toTSP(threshold);
    tsp.close();
}

// keyboard func for stippling
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        //break;
    case 's':
        stippler.finalize(params);
        save(stippler, "output");
        break;
    }
}

// display function for stippling; each iteration works on a voronoi step
void displayFunc() {
    std::pair<float,float> moves = stippler.lloydsMethod();
    std::cout << "avg = " << moves.first << "\tmax = " << moves.second << std::endl;
    glFlush();
    if (moves.first < 0.0001f || moves.second < 0.001f) {
        keyboardFunc('q', 0, 0);
    }
}


// keyboard func for stippling
void keyboardColorFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        //break;
    case 's':
        cyan.finalize(params);
        magenta.finalize(params);
        yellow.finalize(params);
        black.finalize(params);
        save(cyan, "cyan", 0.1f);
        save(magenta, "magenta", 0.15f);
        save(yellow, "yellow", 0.05f);
        save(black, "black", 0.3f);
        break;
    }
}

// display function for color tsp stippling; each iteration works on a voronoi step
void displayColorFunc() {
    std::pair<float,float> cMoves = cyan.lloydsMethod();
    glFlush();
    std::pair<float,float> mMoves = magenta.lloydsMethod();
    glFlush();
    std::pair<float,float> yMoves = yellow.lloydsMethod();
    glFlush();
    std::pair<float,float> kMoves = black.lloydsMethod();
    glFlush();
    std::cout << "cyan:    avg = " << cMoves.first << "\tmax = " << cMoves.second << std::endl;
    std::cout << "magenta: avg = " << mMoves.first << "\tmax = " << mMoves.second << std::endl;
    std::cout << "yellow:  avg = " << yMoves.first << "\tmax = " << yMoves.second << std::endl;
    std::cout << "black:   avg = " << kMoves.first << "\tmax = " << kMoves.second << std::endl << std::endl;
    float avg = std::max(std::max(std::max(cMoves.first, mMoves.first), yMoves.first), kMoves.first);
    float max = std::max(std::max(std::max(cMoves.second, mMoves.second), yMoves.second), kMoves.second);
    if (avg < 0.0001f || max < 0.001f) {
        keyboardColorFunc('q', 0, 0);
    }
}

int main(int argc, char** argv) {
    Random::initSeed();
    //stippler = Stippler("../../images/blue_stuff.png", 25000);
    //stippler = Stippler("../../images/earth.tiff", 25000);
    //stippler = Stippler("../../images/stripes.jpg", 10000);
    //stippler = Stippler("../../images/zoidberg.tiff", 25000);
    //stippler = Stippler("../../images/toto.tiff", 15000);
    //stippler = Stippler("../../images/smiley.tiff", 35000);
    //stippler = Stippler("../../images/chaika01.jpg", 20000);
    //stippler = Stippler("../../images/chaika02.jpg", 35000);
    //stippler = Stippler("../../images/chaika04.JPG", 25000);
    //stippler = Stippler("../../images/horse.JPG", 20000);
    //stippler = Stippler("../../images/girl.JPG", 35000);
    //stippler = Stippler("../../images/sleepy_pup.JPG", 18000);
    //stippler = Stippler("../../images/erin01.JPG", 35000);
    //stippler = Stippler("../../images/chaika03.JPG", 35000);
    //stippler = Stippler("../../images/odyssey.jpg", 20000);
    //stippler = Stippler("../../images/moop.jpg", 50000);
    //stippler = Stippler("../../images/erin02.JPG", 50000);
    //stippler = Stippler("../../images/frog.JPG", 35000);
    //stippler = Stippler("../../images/chaika05.JPG", 35000);
    //stippler = Stippler("../../images/horse2.JPG", 50000);
    //stippler = Stippler("../../images/boats.JPG", 20000);
    //stippler = Stippler("../../images/pacman.jpg", 3000);
    stippler = Stippler("../../images/pengiun.jpg", 10000);
    //stippler = Stippler("../../images/zebra.jpg", 20000);
    //stippler = Stippler("../../images/moop02.JPG", 55000);

    bool filter = false;

    bool outputTSP = true;

    bool cmyk = true;

    params = FinalizeParams(
        FinalizeParams::RADIUS_FUNC_LINEAR,
        0.85f,
        FinalizeParams::RADIUS_MODE_MASS,
        true
    );

    if (filter) {
        stippler.getImage().reset();
        stippler.getImage().applyCosineFilter();
        //stippler.getImage().applyHopeFilter();
        stippler.initStipples(stippler.getNumStipples());
    }

    if (cmyk) {
        cyan = magenta = yellow = black = stippler;
        cyan.getImage().toCyanOnly();
        magenta.getImage().toMagentaOnly();
        yellow.getImage().toYellowOnly();
        cyan.initStipples(stippler.getNumStipples());
        magenta.initStipples(stippler.getNumStipples());
        yellow.initStipples(stippler.getNumStipples());
        // black can use the standard stippler
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(stippler.getWidth(), stippler.getHeight());
    glutInitWindowPosition(30, 30);
    glutCreateWindow("voronoi");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glEnable(GL_DEPTH_TEST);
    glOrtho(0, stippler.getWidth(), stippler.getHeight(), 0, 0, 1);

    if (outputTSP) {
        if (cmyk) {
            glutDisplayFunc(displayColorTSP);
        }
        else {
            glutDisplayFunc(displayTSP);
        }
    }
    else {
        if (cmyk) {
            glutDisplayFunc(displayColorFunc);
            glutIdleFunc(displayColorFunc);
            glutKeyboardFunc(keyboardColorFunc);
        }
        else {
            glutDisplayFunc(displayFunc);
            glutIdleFunc(displayFunc);
            glutKeyboardFunc(keyboardFunc);
        }
    }
    glutMainLoop();
    return 0;
}
