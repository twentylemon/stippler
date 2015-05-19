
#include "main.h"

int glutWindow;
Stippler stippler;

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        //break;
    case 's':
        stippler.finalize(Stippler::FinalizeParams(
            Stippler::RADIUS_MODE_LINEAR,
            2.0f
        ));
        std::ofstream svg("../../output.svg");
        svg << stippler;
        svg.close();
        break;
    }
}

void displayFunc() {
    std::pair<float,float> moves = stippler.lloydsMethod();
    std::cout << "avg = " << moves.first << "\tmax = " << moves.second << std::endl;
    glFlush();
    if (moves.first < 0.001f || moves.second < 0.001f) {
        keyboardFunc('q', 0, 0);
    }
}

int main(int argc, char** argv) {
    //stippler = Stippler("../../images/blue_stuff.png", 25000);
    //stippler = Stippler("../../images/earth.tiff", 100000);
    //stippler = Stippler("../../images/stripes.jpg", 10000);
    //stippler = Stippler("../../images/zoidberg.tiff", 25000);
    //stippler = Stippler("../../images/toto.tiff", 15000);
    //stippler = Stippler("../../images/smiley.tiff", 35000);
    //stippler = Stippler("../../images/chaika01.jpg", 20000);
    //stippler = Stippler("../../images/chaika02.jpg", 35000);
    //stippler = Stippler("../../images/chaika04.JPG", 35000);
    //stippler = Stippler("../../images/horse.JPG", 20000);
    //stippler = Stippler("../../images/girl.JPG", 35000);
    //stippler = Stippler("../../images/sleepy_pup.JPG", 35000);
    //stippler = Stippler("../../images/erin01.JPG", 35000);
    //stippler = Stippler("../../images/chaika03.JPG", 35000);
    //stippler = Stippler("../../images/odyssey.jpg", 50000);
    //stippler = Stippler("../../images/moop.jpg", 50000);
    //stippler = Stippler("../../images/erin02.JPG", 50000);
    //stippler = Stippler("../../images/frog.JPG", 35000);
    //stippler = Stippler("../../images/chaika05.JPG", 35000);
    //stippler = Stippler("../../images/horse2.JPG", 50000);
    //stippler = Stippler("../../images/boats.JPG", 60000);
    //stippler = Stippler("../../images/pacman.jpg", 60000);
    stippler = Stippler("../../images/moop02.JPG", 35000);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(stippler.getWidth(), stippler.getHeight());
    glutInitWindowPosition(30, 30);
    glutWindow = glutCreateWindow("voronoi");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glEnable(GL_DEPTH_TEST);
    glOrtho(0, stippler.getWidth(), 0, stippler.getHeight(), 0, 1);

    glutDisplayFunc(displayFunc);
    glutIdleFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMainLoop();

    return 0;
}
