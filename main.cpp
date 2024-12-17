#include <GL/glut.h>
#include <math.h>
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glOrtho(-1, 1, -1, 1, -1, 1);
}
void veSao()
{
	glColor3f(255.0,255.0,0.0);
    glViewport(200,600,200,200);
    glBegin(GL_POLYGON);
    GLfloat step = 3.14 / 5.0;
	register int i;
	GLfloat angle, r;
	for (i = 0; i < 10; i++)
	{
	r = (i % 2 == 0 ? 0.2 : 0.5);
	angle = i*step;
	glVertex3f(r*cos(angle), r*sin(angle), 0.0);
	}
    glEnd();
}
void veSao90()
{
	glColor3f(1.0f, 0.0f, 0.0f);
    glViewport(600,400,500,500);
    glBegin(GL_POLYGON);
    GLfloat step = 2.0 * M_PI / 190.0;
	register int i;
	GLfloat angle, r;
	for (i = 0; i < 190; i++)
	{
	r = (i % 2 == 0 ? 0.2 : 0.5);
	angle = i*step;
	glVertex3f(r*cos(angle), r*sin(angle), 0.0);
	}
    glEnd();
}

void drawHouse() {
    glViewport(650, 200, 200, 200);

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.2f, 0.0f);
    glVertex2f(1.2f, 0.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4f, -1.0f);
    glVertex2f(0.4f, -1.0f);
    glVertex2f(0.4f, -0.3f);
    glVertex2f(-0.4f, -0.3f);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(-0.8f, -0.2f);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POLYGON);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.8f, -0.5f);
    glVertex2f(0.8f, -0.2f);
    glVertex2f(0.5f, -0.2f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.7f, 0.5f);
    glVertex2f(0.7f, 1.2f);
    glVertex2f(0.5f, 1.2f);
    glEnd();
}
void drawCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int numSegments = 100;
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        GLfloat angle = 2.0f * M_PI * i / numSegments;
        GLfloat dx = radius * cos(angle);
        GLfloat dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawCloud() {
    glColor3f(0.0f, 0.0f, 1.0f);

    glViewport(300, 450, 200, 200);
    drawCircle(-0.3f, 0.1f, 0.2f);
    drawCircle(0.0f, 0.2f, 0.25f);
    drawCircle(0.3f, 0.1f, 0.2f);
    drawCircle(-0.1f, -0.1f, 0.15f);
    drawCircle(0.2f, -0.1f, 0.15f);
}

void drawPeople() {
    glColor3f(1.0f, 0.8f, 0.6f);
    glViewport(400, 150, 250, 250);
    drawCircle(0.0f, 0.4f, 0.1f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, 0.3f);
    glVertex2f(0.1f, 0.3f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(-0.1f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, 0.3f);
    glVertex2f(-0.2f, 0.2f);
    glVertex2f(-0.15f, 0.15f);
    glVertex2f(-0.1f, 0.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.1f, 0.3f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(0.15f, 0.15f);
    glVertex2f(0.1f, 0.2f);
    glEnd();

    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(-0.05f, 0.0f);
    glVertex2f(-0.05f, -0.3f);
    glVertex2f(-0.1f, -0.3f);
    glEnd();
	
	
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(0.1f, -0.3f);
    glVertex2f(0.05f, -0.3f);
    glEnd();
}
void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Ve ngu giac
    glColor3f(1.0f, 1.0f, 1.0f);
    glViewport(200, 200, 200, 200);

    glBegin(GL_POLYGON);
    GLfloat step = 2.0 * M_PI / 5.0;
    GLfloat startAngle = M_PI / 2.0;
    GLfloat r = 0.5f;

    for (int i = 0; i < 5; i++) {
        GLfloat angle = startAngle + i * step;
        glVertex2f(r * cos(angle), r * sin(angle));
    }
    glEnd();
	
	
	// Ve luc giac
	glColor3f(1.0f, 1.0f, 1.0f);
    glViewport(1000, 200, 200, 200);

    glBegin(GL_POLYGON);
    GLfloat step2 = 2.0 * M_PI / 6.0;
    GLfloat startAngle2 = M_PI / 3.0;
    GLfloat r2 = 0.5f;

    for (int i = 0; i < 6; i++) {
        GLfloat angle = startAngle2 + i * step2;
        glVertex2f(r2 * cos(angle), r2 * sin(angle));
    }
    glEnd();
    
    drawHouse();
    
    veSao();
    veSao90();
    drawCloud();
    drawPeople();
    glFlush();
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


