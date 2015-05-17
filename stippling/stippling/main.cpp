
#include "main.h"

Voronoi vor;
Stippler stippler;

void displayFunc() {
    /*
    std::vector<Stipple> stipples = stippler.getStipples();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat radius = 200.0f, height = 25.0f;
    GLint stacks = 10, slices = 40;

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(stipples[0].getX(), stipples[0].getY(), 0);
    glutSolidCone(radius, height, slices, stacks);
    glPopMatrix();
    

    glPushMatrix();
    glColor3f(0,10, 0);
    glTranslatef(stipples[1].getX(), stipples[1].getY(), 0);
    glutSolidCone(radius, height, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(stipples[2].getX(), stipples[2].getY(), 0);
    glutSolidCone(radius, height, slices, stacks);
    glPopMatrix();
    */
    vor.redistributeStipples(stippler);
    glFlush();
    //Sleep(1000);
}

int main(int argc, char** argv) {
    stippler = Stippler(600, 400, 1000);
    std::ofstream svg("output.svg");
    svg << stippler;
    svg.close();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(stippler.getWidth(), stippler.getHeight());
    glutInitWindowPosition(30, 30);
    int glutWindow = glutCreateWindow("voronoi");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glOrtho(0, 200, 200, 0, -100, 100);

    glutDisplayFunc(displayFunc);
    glutIdleFunc(displayFunc);
    glutMainLoop();
    //system("pause");
    return 0;
}
