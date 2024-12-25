#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>
#include <map>
#include <vector>
#include <mmsystem.h>
#include <Windows.h>

float camX = 0.0f, camY = 2.0f, camZ = 5.0f;  // Vi tri camera
float camYaw = 0.0f, camPitch = 0.0f;         // Góoc yaw và pitch
float velocity = 0.03f;                          // Toc do di chuyen
float rotationSpeed = 0.005f;                 // T?c d? xoay camera
bool isLeftMousePressed = false;              // Tr?ng thái nh?n chu?t trái
int lastMouseX, lastMouseY;                   // V? trí chu?t tru?c dó

std::map<unsigned char, bool> keyStates;      // Trang thai phim thuong
std::map<int, bool> specialKeyStates;         // Trang thai phim dac biet
float starAngle = 0.0f; // Bien luu goc quay cua ngoi sao

struct Snowflake {
    float x, y, z;       // Vi tri
    float size;          // Kich thuoc
    float speed;         // Toc do tuyet roi
};

std::vector<Snowflake> snowflakes;  // Danh sach cac hat tuyet

void generateSnowflakes(int numSnowflakes) {
    for (int i = 0; i < numSnowflakes; ++i) {
        Snowflake snowflake;
        snowflake.x = (rand() % 101) - 50.0f;  // Vi tri ngau nhien tren truc x
        snowflake.y = (rand() % 5000) / 100.0f + 5.0f;  // Vi tri ngau nhien tren truc y
        snowflake.z = (rand() % 101) - 50.0f;  // Vi tri ngau nhien tren truc z
        snowflake.size = (rand() % 3 + 1) / 100.0f;    // Kich thuoc ngau nhien
        snowflake.speed = (rand() % 5 + 1) / 1000.0f;  // Toc do roi ngau nhien
        snowflakes.push_back(snowflake);
    }
}



void updateSnowflakes() {
    for (int i = 0; i < snowflakes.size(); ++i) {
        snowflakes[i].y -= snowflakes[i].speed;  // Roi xuong theo truc y
        if (snowflakes[i].y < -5.0f) {  // Neu tuyet da roi xuong duoi, dua nó lên lai tren cao
            snowflakes[i].y = (rand() % 10000) / 100.0f + 5.0f;;
            snowflakes[i].x = (rand() % 4000 - 100) / 100.0f;
            snowflakes[i].z = (rand() % 4000 - 100) / 100.0f;
        }
    }
}



void drawSnowflakes() {
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);  // white

    for (int i = 0; i < snowflakes.size(); ++i) {
        float x = snowflakes[i].x;
        float y = snowflakes[i].y;
        float z = snowflakes[i].z;
        float size = snowflakes[i].size;

        // Ve cac canh cua tuyet
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + size, y + size, z);

        glVertex3f(x, y, z);
        glVertex3f(x - size, y + size, z);

        glVertex3f(x, y, z);
        glVertex3f(x + size, y - size, z);

        glVertex3f(x, y, z);
        glVertex3f(x - size, y - size, z);
        glEnd();
    }
}





void drawLand() {
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_QUADS);

    // Mat tren
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);

    // Mat duoi
   	glColor3f(0.447f, 0.447f, 0.447f); // Gray
    glVertex3f(-50.0f, -5.0f, -50.0f);
    glVertex3f(50.0f, -5.0f, -50.0f);
    glVertex3f(50.0f, -5.0f, 50.0f);
    glVertex3f(-50.0f, -5.0f, 50.0f);

    // Mat truoc
    glColor3f(0.6f, 0.4f, 0.2f); // Brown
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, -5.0f, -50.0f);
    glVertex3f(-50.0f, -5.0f, -50.0f);

    // Mat sau
    glColor3f(0.6f, 0.4f, 0.2f); // Brown
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(50.0f, -5.0f, 50.0f);
    glVertex3f(-30.0f, -5.0f, 50.0f);

    // Mat trai
    glColor3f(0.6f, 0.4f, 0.2f); // Brown
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, -5.0f, 50.0f);
    glVertex3f(-50.0f, -5.0f, -50.0f);

    // Mat phai
    glColor3f(0.6f, 0.4f, 0.2f); // Brown
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(50.0f, -5.0f, 50.0f);
    glVertex3f(50.0f, -5.0f, -50.0f);

    glEnd();
}

void setupLighting() {
    // Dinh nghia anh sang moi truong
    GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Anh sang moi truong
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Anh sang khuech tan
    GLfloat specularLight[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Anh sang phan xa
    GLfloat lightPosition[] = {0.0f, 00.0f, 40.0f, 1.0f}; // Vi tri nguon sang

    // Cai dat anh sang cho GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Bat anh sang
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Bat tinh nang chinh sua tu dong anh sang
    glEnable(GL_COLOR_MATERIAL);

    // Thiet lap thuoc tinh phan chieu anh sang cua vat lieu
    GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat mat_shininess[] = {25.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
//	setupLighting();
	generateSnowflakes(15000);
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
    // Di chuyen bang cac phim thuong
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
    if (keyStates[' ']) camY +=velocity;  // Len cao khi nhan Space
    if (keyStates['c']) camY -=velocity;  // Xuong thap khi nhan c
	
    glutPostRedisplay();
}

// Ham ve nguoi tuyet
void drawSnowman() {
	
    // Thuoc tinh
    float bodyRadius = 2.0f;
    float middleRadius = 1.5f;
    float headRadius = 1.0f;
    
    glColor3f( 0.9f, 0.9f, 0.9f); // Grey
    // Ve than nguoi tuyet
    glPushMatrix();
    glTranslatef(-8.0f, 1.5f, -5.0f); // Dich chuyen vi tri phan than
    glutSolidSphere(bodyRadius, 50, 50); // Ve phan than
    glPopMatrix();

    // Ve phan giua nguoi tuyet
    glPushMatrix();
    glTranslatef(-8.0f, 4.0f, -5.0f); // Dich chuyen vi tri phan giua
    glutSolidSphere(middleRadius, 50, 50); // Ve phan giua
    glPopMatrix();

    // Ve dau nguoi tuyet
    glPushMatrix();
    glTranslatef(-8.0f, 6.0f, -5.0f); // Dich chuyen vi tri phan dau
    glutSolidSphere(headRadius, 50, 50); // Ve phan dau
    glPopMatrix();

    // Ve mat
    glPushMatrix();
    glTranslatef(-8.0f, 6.0f, -4.0f); // Dich chuyen mat trai
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glutSolidSphere(0.1f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.4f, 6.0f, -4.2f); // Dich chuyen mat trai
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glutSolidSphere(0.1f, 10, 10);
    glPopMatrix();

    // Ve mui
    glPushMatrix();
    glTranslatef(-7.7f, 6.0f, -4.0f); // Dich chuyen mui
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Xoay mui
    glutSolidCone(0.2f, 0.5f, 10, 10); // Ve mui hinh non
    glPopMatrix();

    // Ve cuc ao
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glPushMatrix();
    glTranslatef(-7.35f, 4.0f, -3.7f); // Dich chuyen cuc ao dau tien
    glutSolidSphere(0.1f, 10, 10); // Cuc dau tien
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.35f, 3.3f, -3.75f); // Dich chuyen cuc ao thu hai
    glutSolidSphere(0.1f, 10, 10); // Cuc thu hai
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.35f, 2.6f, -3.5f); // Dich chuyen cuc ao thu ba
    glutSolidSphere(0.1f, 10, 10); // Cuc thu ba
    glPopMatrix();

    // Ve tay phai
    glPushMatrix();
    glTranslatef(-9.5f, 4.5f, -5.0f); // Dich chuyen
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Xoay tay
    glColor3f(0.5f, 0.25f, 0.0f); // Brown
    glutSolidCone(0.5f, 5.0f, 10, 10); // Ve tay
    glPopMatrix();

    // Ve tay trai
    glPushMatrix();
    glTranslatef(-7.0f, 4.5f, -5.0f); // Dich chuyen
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Xoay tay
    glColor3f(0.5f, 0.25f, 0.0f); // Brown
    glutSolidCone(0.5f, 5.0f, 10, 10); // Ve tay
    glPopMatrix();
}



// Hàm ve hinh tru (than cay)
void drawCylinder() {
    glColor3f(0.6f, 0.3f, 0.1f); // Brown
    GLUquadric *quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(-5.0f, 17.0f, -1.0f);

    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 3.0f, 3.0f, 10.0f, 50, 10);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// Ham ve hinh non (canh cay)
void drawCone(float baseRadius, float height, float red, float green, float blue) {
    glColor3f(red, green, blue); // Thiet lap mau
    glPushMatrix();
    glutSolidCone(baseRadius, height, 50, 50);
    glPopMatrix();
}

// Ham ve ngoi sao tren dinh cay
void drawStar(float size) {
    glPushMatrix();
    glTranslatef(0.0, 0.0, 40.0);
    glRotatef(starAngle, 0.0f, 0.0f, 1.0f); // Xoay ngoi sao theo goc

    glBegin(GL_TRIANGLE_FAN); // Bat dau ve ngoi sao
    glColor3f(1.0, 1.0, 0.0); // Yellow
    
    // Tam giac giua ngoi sao
    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= 10; i++) {
        float angle = i * M_PI / 5.0; // 5 cánh = 10 diem
        float radius = (i % 2 == 0) ? size : size / 2.5;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }

    glEnd();
    glPopMatrix();

    starAngle += 0.1f; // Cap nhat goc quay
    if (starAngle > 360.0f) {
        starAngle -= 360.0f; // Ðam bao goc quay luon trong pham vi 0-360 do
    }
}



void drawTree() {
    glPushMatrix();

    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    // Ve than cay
    drawCylinder();

    glTranslatef(-5.0f, 17.0f, 8.99f);

    // Ve lop la thu nhat
    glPushMatrix();
    drawCone(10.0f, 10.0f, 0.0f, 0.5f, 0.0f);
    glPopMatrix();

    // Ve lop la thu hai
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.5f);
    drawCone(8.0f, 20.0f, 0.0f, 0.7f, 0.0f);
    glPopMatrix();

    // Ve lop la thu ba
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 4.5f);
    drawCone(6.0f, 30.0f, 0.0f, 0.9f, 0.0f);
    glPopMatrix();

    // Ve lop la thu tu
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 6.0f);
    drawCone(4.0f, 40.0f, 0.0f, 1.0f, 0.0f);
    glPopMatrix();

    // Ve ngoi sao tren dinh
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 7.5f); // Dich len tren cung
    drawStar(10.0);
    glPopMatrix();

    glPopMatrix();
}



void drawHouse() {
	glPushMatrix(); // Luu trang thai hien tai
    glTranslatef(3.0f, 0.0f, 0.0f);
	glScalef(3.0, 3.0, 3.0);
    // Than nha (hinh hop)
    glColor3f(0.8f, 0.2f, 0.2f); // Mau do
    glBegin(GL_QUADS);
    // Mat truoc
    glVertex3f(-2.0f, 0.0f, 2.0f);
    glVertex3f(2.0f, 0.0f, 2.0f);
    glVertex3f(2.0f, 3.0f, 2.0f);
    glVertex3f(-2.0f, 3.0f, 2.0f);
    // Mat sau
    glVertex3f(-2.0f, 0.0f, -2.0f);
    glVertex3f(2.0f, 0.0f, -2.0f);
    glVertex3f(2.0f, 3.0f, -2.0f);
    glVertex3f(-2.0f, 3.0f, -2.0f);
    // Mat trai
    glVertex3f(-2.0f, 0.0f, -2.0f);
    glVertex3f(-2.0f, 0.0f, 2.0f);
    glVertex3f(-2.0f, 3.0f, 2.0f);
    glVertex3f(-2.0f, 3.0f, -2.0f);
    // Mat phai
    glVertex3f(2.0f, 0.0f, -2.0f);
    glVertex3f(2.0f, 0.0f, 2.0f);
    glVertex3f(2.0f, 3.0f, 2.0f);
    glVertex3f(2.0f, 3.0f, -2.0f);
    // Mat duoi
    glVertex3f(-2.0f, 0.0f, -2.0f);
    glVertex3f(2.0f, 0.0f, -2.0f);
    glVertex3f(2.0f, 0.0f, 2.0f);
    glVertex3f(-2.0f, 0.0f, 2.0f);
    glEnd();

    // Mai nha (hinh chop)
    glColor3f(0.6f, 0.4f, 0.2f); // Màu nâu
    glBegin(GL_TRIANGLES);
    // Mai phia truoc
    glVertex3f(-2.0f, 3.0f, 2.0f);
    glVertex3f(2.0f, 3.0f, 2.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    // Mai phia sau
    glVertex3f(-2.0f, 3.0f, -2.0f);
    glVertex3f(2.0f, 3.0f, -2.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    // Mai ben trai
    glVertex3f(-2.0f, 3.0f, -2.0f);
    glVertex3f(-2.0f, 3.0f, 2.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    // Mai ben phai
    glVertex3f(2.0f, 3.0f, -2.0f);
    glVertex3f(2.0f, 3.0f, 2.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    
    // Ve cua
	glColor3f(0.5f, 0.2f, 0.0f); // Màu nâu
	glBegin(GL_QUADS);
	glVertex3f(-2.01f, 0.0f, 0.5f);
	glVertex3f(-2.01f, 0.0f, -0.5f);
	glVertex3f(-2.01f, 1.5f, -0.5f);
	glVertex3f(-2.01f, 1.5f, 0.5f);
    glEnd();
    
    glPopMatrix();
}

// Ham ve hop qua
void drawBox(float scale, float posX, float posY, float posZ, float r, float g, float b) {
    // Hop qua chinh (hinh lap phuong)
    glPushMatrix();
    glTranslatef(posX, posY, posZ); // Setup vi tri cho hop qua
    glScalef(scale, scale, scale);
    glColor3f(r, g, b); // Color
    glutSolidCube(1.0f);
    glPopMatrix();

    // Nap hop qua
    glPushMatrix();
    glTranslatef(posX, posY + scale * 0.6f, posZ);
    glScalef(scale * 1.1f, scale * 0.2f, scale * 1.1f);
    glColor3f(r, r, b); // Random color
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dai ruy bang ngang
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale * 0.2f, scale * 1.2f, scale);
    glColor3f(b, r, g); // Random color
    glutSolidCube(1.2f);
    glPopMatrix();

    // Dai ruy bang ngang
    glPushMatrix();
    glTranslatef(posX, posY, posZ); // Gi? ? v? trí h?p
    glScalef(scale, scale * 1.2f, scale * 0.2f);
    glColor3f(b, r, g); // Random color
    glutSolidCube(1.2f);
    glPopMatrix();

    // No tren hop qua
    glPushMatrix();
    glTranslatef(posX, posY + scale * 0.7f, posZ);
    glScalef(scale * 0.4f, scale * 0.2f, scale * 0.4f);
    glColor3f(g, b, r); // Random color
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Thiet lap goc nhin
    gluLookAt(camX, camY, camZ, camX + sin(camYaw), camY - sin(camPitch), camZ - cos(camYaw), 0.0, 1.0, 0.0);

    // Ve doi tuong
    glColor3f(1.0, 1.0, 1.0);
	drawLand();
	drawHouse();
	drawTree();
	updateSnowflakes(); 
	drawSnowflakes();
    drawBox(10.0f, -20.0f, 0.0f, -20.0f, 1.0f, 0.0f, 0.0f);
	drawBox(4.0f, -10.0f, 0.0f, -20.0f, 0.5f, 1.0f, 0.0f);
	drawBox(1.2f, -1.0f, 0.0f, -16.0f, 0.4f, 0.5f, 1.0f);
	drawBox(7.5f, -15.0f, 0.0f, -10.0f, 0.8f, 0.2f, 0.5f);
	drawBox(12.0f, -25.0f, 0.0f, -30.0f, 0.9f, 0.6f, 0.1f);
	drawBox(5.0f, -10.0f, 0.0f, -5.0f, 0.2f, 0.7f, 0.8f);
	drawBox(8.0f, -18.0f, 0.0f, -22.0f, 0.4f, 0.6f, 0.9f);
	drawBox(6.0f, -12.0f, 0.0f, -8.0f, 0.1f, 0.9f, 0.4f);
	drawBox(11.0f, -20.0f, 0.0f, -15.0f, 0.3f, 0.5f, 1.0f);
	drawBox(9.5f, -14.0f, 0.0f, -25.0f, 0.7f, 0.3f, 0.2f);
	drawBox(3.0f, -6.0f, 0.0f, -18.0f, 1.0f, 0.0f, 0.0f);
	drawBox(2.5f, -5.0f, 0.0f, -12.0f, 0.6f, 0.4f, 0.9f);
	drawBox(6.0f, -10.0f, 0.0f, 40.0f, 0.4f, 0.1f, 0.9f);
	drawBox(3.7f, -5.0f, 0.0f, 35.0f, 0.1f, 0.2f, 0.9f);
	drawBox(1.0f, -1.0f, 0.0f, 41.0f, 0.5f, 0.8f, 0.6f);
	drawBox(4.0f, 3.0f, 0.0f, 41.0f, 0.7f, 0.8f, 0.6f);

	
	
	drawBox(6.0f, 20.0f, 0.0f, -10.0f, 0.4f, 0.7f, 0.9f);
	drawBox(8.0f, -35.0f, 0.0f, 5.0f, 0.5f, 0.4f, 0.3f);
	drawBox(3.5f, 15.0f, 0.0f, 20.0f, 0.2f, 0.6f, 0.8f);
	drawBox(5.0f, -45.0f, 0.0f, -5.0f, 0.3f, 0.1f, 0.7f);
	drawBox(4.5f, 30.0f, 0.0f, 10.0f, 0.6f, 0.9f, 0.5f);
	drawBox(7.0f, -20.0f, 0.0f, 15.0f, 0.8f, 0.5f, 0.4f);
	drawBox(9.0f, 12.0f, 0.0f, 25.0f, 0.7f, 0.2f, 0.1f);
	drawBox(2.0f, -40.0f, 0.0f, -10.0f, 0.3f, 0.8f, 0.6f);
	drawBox(10.0f, -10.0f, 0.0f, -30.0f, 0.6f, 0.9f, 0.2f);
	drawBox(1.5f, 5.0f, 0.0f, 18.0f, 0.9f, 0.1f, 0.5f);
	drawBox(6.5f, 25.0f, 0.0f, -15.0f, 0.5f, 0.6f, 0.7f);
	drawBox(11.0f, -30.0f, 0.0f, 3.0f, 0.8f, 0.3f, 0.4f);
	drawBox(30.0f, 25.0f, 0.0f, -35.0f, 0.7f, 0.8f, 0.9f);
	drawBox(4.8f, -50.0f, 0.0f, 12.0f, 0.5f, 0.4f, 0.6f);
	drawBox(12.0f, 2.0f, 0.0f, -20.0f, 0.6f, 0.7f, 0.3f);
	drawBox(3.0f, -28.0f, 0.0f, 9.0f, 0.7f, 0.8f, 0.2f);
	drawBox(9.5f, 18.0f, 0.0f, 21.0f, 0.9f, 0.1f, 0.5f);
	drawBox(4.0f, 7.0f, 0.0f, -12.0f, 0.3f, 0.4f, 0.8f);
	drawBox(6.0f, -10.0f, 0.0f, -18.0f, 0.5f, 0.2f, 0.6f);
	drawBox(5.5f, 35.0f, 0.0f, 6.0f, 0.6f, 0.9f, 0.3f);
	drawBox(8.0f, -15.0f, 0.0f, 30.0f, 0.8f, 0.3f, 0.2f);
	drawBox(2.5f, -25.0f, 0.0f, 22.0f, 0.4f, 0.7f, 0.5f);
	drawBox(7.3f, 0.0f, 0.0f, -50.0f, 0.1f, 0.9f, 0.7f);
	drawBox(10.5f, 9.0f, 0.0f, 40.0f, 0.2f, 0.5f, 0.3f);
	drawBox(4.0f, -7.0f, 0.0f, 12.0f, 0.9f, 0.4f, 0.8f);
	drawBox(3.8f, 22.0f, 0.0f, 13.0f, 0.5f, 0.6f, 0.9f);
	drawBox(6.5f, 19.0f, 0.0f, 29.0f, 0.7f, 0.4f, 0.1f);
	drawBox(8.0f, -10.0f, 0.0f, -25.0f, 0.8f, 0.6f, 0.4f);
	drawBox(7.5f, 28.0f, 0.0f, 19.0f, 0.5f, 0.3f, 0.8f);
	drawBox(6.2f, -12.0f, 0.0f, -6.0f, 0.6f, 0.9f, 0.2f);
	drawBox(10.0f, -33.0f, 0.0f, -11.0f, 0.4f, 0.3f, 0.5f);
	drawBox(9.0f, 20.0f, 0.0f, 5.0f, 0.6f, 0.9f, 0.8f);

	
	drawSnowman();
    glutSwapBuffers();
    
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 10000.0);
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
	
	PlaySound(TEXT("noel.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Noel Scene");
	glutFullScreen();
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
