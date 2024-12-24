#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>
#include <map>

float camX = 0.0f, camY = 0.0f, camZ = 5.0f;  // Vi tri camera
float camYaw = 0.0f, camPitch = 0.0f;         // G�oc yaw v� pitch
float velocity = 0.005f;                          // Toc do di chuyen
float rotationSpeed = 0.005f;                 // T?c d? xoay camera
bool isLeftMousePressed = false;              // Tr?ng th�i nh?n chu?t tr�i
int lastMouseX, lastMouseY;                   // V? tr� chu?t tru?c d�

std::map<unsigned char, bool> keyStates;      // Tr?ng th�i ph�m thu?ng
std::map<int, bool> specialKeyStates;         // Tr?ng th�i ph�m d?c bi?t
void setupLighting()
{
    GLfloat ambientLight[] = {1.0f, 0.0f, 0.0f, 1.0f}; // �nh s�ng m�i truong (ambient)
   	GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f}; // �nh s�ng khuech t�n (diffuse)
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};// �nh s�ng phan chieu (specular)
    GLfloat lightPosition[] = {1.0f, 1.0f, 1.0f, 0.0f}; // Vi tri nguon sang

    // Thi?t l?p �nh s�ng m�i tru?ng v� khu?ch t�n
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    // B?t ngu?n s�ng
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Thi?t l?p v?t li?u: �?c di?m ph?n chi?u specular c?a v?t li?u
    GLfloat mat_specular[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat mat_shininess[] = {50.0f};


    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
	setupLighting();
    glEnable(GL_DEPTH_TEST);
}

void handleKeyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
    if (key == 27) exit(0);  // ESC d? tho�t
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
    // Di chuy?n b?ng c�c ph�m thu?ng
    if (keyStates['w']) {
        camX +=velocity * sin(camYaw);
        camZ -=velocity * cos(camYaw);
    }
    if (keyStates['s']) {
        camX -=velocity * sin(camYaw);
        camZ +=velocity * cos(camYaw);
    }
    if (keyStates['a']) {
        camX -=velocity * cos(camYaw);
        camZ -=velocity * sin(camYaw);
    }
    if (keyStates['d']) {
        camX +=velocity * cos(camYaw);
        camZ +=velocity * sin(camYaw);
    }
    if (keyStates[' ']) camY +=velocity;  // L�n cao khi nh?n Space
    if (keyStates['c']) camY -=velocity;  // Xu?ng th?p khi nh?n Shift
	
    glutPostRedisplay();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Thi?t l?p g�c nh�n camera
    gluLookAt(camX, camY, camZ, 
              camX + sin(camYaw), camY - sin(camPitch), camZ - cos(camYaw), 
              0.0, 1.0, 0.0);

    // V? d?i tu?ng
    glColor3f(1.0, 1.0, 1.0);
	glColor3f(0.2f, 0.8f, 0.2f); // M�u xanh l�
    glBegin(GL_QUADS);           // B?t d?u v? h�nh t? gi�c
        glVertex3f(-10.0f, -1.0f, -10.0f); // �?nh th? nh?t
        glVertex3f( 10.0f, -1.0f, -10.0f); // �?nh th? hai
        glVertex3f( 10.0f, -1.0f,  10.0f); // �?nh th? ba
        glVertex3f(-10.0f, -1.0f,  10.0f); // �?nh th? tu
    glEnd();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// X? l� s? ki?n chu?t
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

// X? l� s? ki?n di chuy?n chu?t
void motion(int x, int y) {
    if (isLeftMousePressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        camYaw += deltaX * rotationSpeed;
        camPitch -= deltaY * rotationSpeed;

        // Gi?i h?n pitch d? tr�nh l?t camera
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
