#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>
#include <map>

float camX = 0.0f, camY = 0.0f, camZ = 5.0f;  // V? trí camera
float camYaw = 0.0f, camPitch = 0.0f;         // Góc yaw và pitch
float step = 0.005f;                          // T?c d? di chuy?n
float rotationSpeed = 0.005f;                 // T?c d? xoay camera
bool isLeftMousePressed = false;              // Tr?ng thái nh?n chu?t trái
int lastMouseX, lastMouseY;                   // V? trí chu?t tru?c dó

std::map<unsigned char, bool> keyStates;      // Tr?ng thái phím thu?ng
std::map<int, bool> specialKeyStates;         // Tr?ng thái phím d?c bi?t

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
}

void handleKeyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
    if (key == 27) exit(0);  // ESC d? thoát
}

void handleKeyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void handleSpecialKeyboardDown(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void handleSpecialKeyboardUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

void update() {
    // Di chuy?n b?ng các phím thu?ng
    if (keyStates['w']) {
        camX += step * sin(camYaw);
        camZ -= step * cos(camYaw);
    }
    if (keyStates['s']) {
        camX -= step * sin(camYaw);
        camZ += step * cos(camYaw);
    }
    if (keyStates['a']) {
        camX -= step * cos(camYaw);
        camZ -= step * sin(camYaw);
    }
    if (keyStates['d']) {
        camX += step * cos(camYaw);
        camZ += step * sin(camYaw);
    }
    if (keyStates[' ']) camY += step;  // Lên cao khi nh?n Space
    if (keyStates['c']) camY -= step;  // Xu?ng th?p khi nh?n Shift
	
    glutPostRedisplay();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Thi?t l?p góc nhìn camera
    gluLookAt(camX, camY, camZ, 
              camX + sin(camYaw), camY - sin(camPitch), camZ - cos(camYaw), 
              0.0, 1.0, 0.0);

    // V? d?i tu?ng
    glColor3f(1.0, 1.0, 1.0);
    glutWireTeapot(1.0);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// X? lý s? ki?n chu?t
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isLeftMousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isLeftMousePressed = false;
        }
    }
}

// X? lý s? ki?n di chuy?n chu?t
void motion(int x, int y) {
    if (isLeftMousePressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        camYaw += deltaX * rotationSpeed;
        camPitch -= deltaY * rotationSpeed;

        // Gi?i h?n pitch d? tránh l?t camera
        if (camPitch > M_PI / 2.0f) camPitch = M_PI / 2.0f;
        if (camPitch < -M_PI / 2.0f) camPitch = -M_PI / 2.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Camera Movement and Rotation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutKeyboardFunc(handleKeyboardDown);
    glutKeyboardUpFunc(handleKeyboardUp);
    glutSpecialFunc(handleSpecialKeyboardDown);
    glutSpecialUpFunc(handleSpecialKeyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
