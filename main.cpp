#include <GL/glut.h>
#include <math.h>
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glOrtho(-1, 1, -1, 1, -1, 1);
}

void myDisplay() {

}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Giang sinh vui ve");
    glutFullScreen();
    glutDisplayFunc(myDisplay);
    initGL();
    glutMainLoop();
    return 0;
}


