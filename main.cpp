#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <ctime>
float santaX = -1.0f; // T?a d? ban d?u c?a ông già Noel
int goc = 0;
float posX = 0.0f, posY = 0.0f, posZ = 0.0f; // T?a d? ban d?u
float moveSpeed = 0.1f;         
struct Snowflake {
    float x, y, size, speed;
};

struct Star {
    float x, y, size, brightness;
    bool increasing;
    float flickerSpeed;
};

struct Smoke {
    float x, y, size, alpha;
};

std::vector<Smoke> smokeParticles;
int maxSmokeParticles = 50;

void generateSmoke() {
    if (smokeParticles.size() < maxSmokeParticles) {
        Smoke smoke;
        smoke.x = 0.705f;  
        smoke.y = 0.15f;
        smoke.size = (rand() % 5 + 1) / 100.0f;
        smoke.alpha = 1.0f; 
        smokeParticles.push_back(smoke);
    }
}

std::vector<Snowflake> snowflakes;
std::vector<Star> stars;
int numSnowflakes = 100;
int numStars = 150;

float windowColorR = 0.2, windowColorG = 0.6, windowColorB = 0.8; bool increasingBrightness = true;

bool showMessage = false;
float messageX = -1.0;

float starBrightness = 1.0;
bool starIncreasing = false;

bool showAurora = false;
int auroraDuration = 20000; 
int auroraStartTime = 0;

int messageDuration = 20000;
int messageStartTime = 0;

int colorChangeDuration = 500; 
int colorChangeStartTime = 0;
int currentColorIndex = 0;
float colors[7][3] = {
    {1.0, 0.0, 0.0}, 
    {1.0, 0.5, 0.0}, 
    {1.0, 1.0, 0.0}, 
    {0.0, 1.0, 0.0}, 
    {0.0, 0.0, 1.0}, 
    {0.5, 0.0, 1.0}, 
    {1.0, 0.0, 1.0}  
};

int lightColorChangeDuration = 500; 
int lightColorChangeStartTime = 0;
int currentLightColorIndex = 0;
float lightColors[7][3] = {
    {1.0, 0.0, 0.0}, // ??
    {1.0, 0.5, 0.0}, // Cam
    {1.0, 1.0, 0.0}, // V ng
    {0.0, 1.0, 0.0}, // Xanh l 
    {0.0, 0.0, 1.0}, // Xanh d??ng
    {0.5, 0.0, 1.0}, // T m
    {1.0, 0.0, 1.0}  // H?ng
};

void generateSnowflakes() {
    snowflakes.clear();
    for (int i = 0; i < numSnowflakes; ++i) {
        Snowflake snowflake;
        snowflake.x = (rand() % 200 - 100) / 100.0f;
        snowflake.y = (rand() % 200) / 100.0f + 1.0f;
        snowflake.size = (rand() % 5 + 2) / 100.0f;
        snowflake.speed = (rand() % 5 + 1) / 500.0f;
        snowflakes.push_back(snowflake);
    }
}

void generateStars() {
    stars.clear();
    for (int i = 0; i < numStars; ++i) {
        Star star;
        do {
            star.x = (rand() % 200 - 100) / 100.0f;
            star.y = (rand() % 200) / 100.0f + 0.2f;
        } while (star.y > 0.6 && star.y < 0.8);
        star.size = (rand() % 3 + 2) / 100.0f;
        star.brightness = (rand() % 7 + 3) / 10.0f;
        star.flickerSpeed = (rand() % 5 + 1) / 1000.0f;
        star.increasing = true;
        stars.push_back(star);
    }
}

void updateSmoke() {
    for (int i = 0; i < smokeParticles.size(); ++i) {
        Smoke& smoke = smokeParticles[i];
        smoke.y += 0.01f; 
        smoke.size += 0.001f; 
        smoke.alpha -= 0.01f; 
        if (smoke.alpha <= 0) {
            smokeParticles.erase(smokeParticles.begin() + i);
            --i;
        }
    }
    generateSmoke(); 
}

void updateSnowflakes() {
    for (int i = 0; i < snowflakes.size(); ++i) {
        Snowflake& snowflake = snowflakes[i];
        snowflake.y -= snowflake.speed;
        if (snowflake.y < -1.0f) {
            snowflake.y = 1.0f;
            snowflake.x = (rand() % 200 - 100) / 100.0f;
        }
    }
}

void drawMountains() {
    glColor3f(0.5, 0.35, 0.05); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.6, 0.0);
    glVertex2f(-0.1, 0.0);
    glVertex2f(-0.35, 0.5);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.4, 0.4);
    glVertex2f(-0.35, 0.5);
    glVertex2f(-0.3, 0.4);
    glEnd();

    // Ng?n n i ph? b n tr i
    glColor3f(0.45, 0.3, 0.05); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9, 0.0);
    glVertex2f(-0.5, 0.0);
    glVertex2f(-0.7, 0.4);
    glEnd();

    glColor3f(0.95, 0.95, 0.95); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.75, 0.3);
    glVertex2f(-0.7, 0.4);
    glVertex2f(-0.65, 0.3);
    glEnd();

    glColor3f(0.45, 0.3, 0.05);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.3, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(-0.1, 0.3);
    glEnd();

    glColor3f(0.95, 0.95, 0.95);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15, 0.25);
    glVertex2f(-0.1, 0.3);
    glVertex2f(-0.05, 0.25);
    glEnd();
}

void updateStars() {
    for (int i = 0; i < stars.size(); ++i) {
        Star& star = stars[i];
        if (star.increasing) {
            star.brightness += star.flickerSpeed;
            if (star.brightness >= 1.0f) {
                star.increasing = false;
            }
        } else {
            star.brightness -= star.flickerSpeed;
            if (star.brightness <= 0.3f) {
                star.increasing = true;
            }
        }
    }
}

void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        showMessage = true;
        messageX = -1.0;
        messageStartTime = glutGet(GLUT_ELAPSED_TIME); 
        showAurora = true; 
    }
}

void drawSmoke() {
    for (int i = 0; i < smokeParticles.size(); ++i) {
        const Smoke& smoke = smokeParticles[i];
        glColor4f(0.5, 0.5, 0.5, smoke.alpha);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++) {
            float angle = j * M_PI / 180.0;
            glVertex2f(smoke.x + smoke.size * cos(angle), smoke.y + smoke.size * sin(angle));
        }
        glEnd();
    }
}

void drawStars() {
    for (int i = 0; i < stars.size(); ++i) {
        const Star& star = stars[i];
        glColor3f(star.brightness, star.brightness, 0.0);
        glPointSize(star.size * 100);
        glBegin(GL_POINTS);
        glVertex2f(star.x, star.y);
        glEnd();
    }
}

void drawSnowflakes() {
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(4.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < snowflakes.size(); ++i) {
        const Snowflake& snowflake = snowflakes[i];
        glVertex2f(snowflake.x, snowflake.y);
    }
    glEnd();
}

void drawTree() {

    glColor3f(0.1, 0.6, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9, -0.1);
    glVertex2f(-0.5, -0.1);
    glVertex2f(-0.7, 0.3);

    glVertex2f(-1.0, -0.3);
    glVertex2f(-0.4, -0.3);
    glVertex2f(-0.7, 0.1);

    glVertex2f(-1.1, -0.5);
    glVertex2f(-0.3, -0.5);
    glVertex2f(-0.7, -0.1);
    glEnd();

    glColor3f(0.6, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(-0.75, -0.5);
    glVertex2f(-0.65, -0.5);
    glVertex2f(-0.65, -0.8);
    glVertex2f(-0.75, -0.8);
    glEnd();

    glColor3f(1.0, 0.0, 0.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.75 + 0.03 * cos(angle), 0.1 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(0.0, 1.0, 0.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.85 + 0.03 * cos(angle), -0.2 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(0.0, 0.0, 1.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.55 + 0.03 * cos(angle), -0.3 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 1.0, 0.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.65 + 0.03 * cos(angle), 0.0 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.7 + 0.03 * cos(angle), -0.1 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.6 + 0.03 * cos(angle), -0.4 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(0.0, 1.0, 1.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.8 + 0.03 * cos(angle), -0.35 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 0.0, 1.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(-0.5 + 0.03 * cos(angle), -0.25 + 0.03 * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 0.0, 0.0); 
	glBegin(GL_QUADS);
	glVertex2f(-0.86, -0.56); 
	glVertex2f(-0.76, -0.56); 
	glVertex2f(-0.76, -0.66); 
	glVertex2f(-0.86, -0.66);
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_LINES);
	
	glVertex2f(-0.86, -0.61);
	glVertex2f(-0.76, -0.61);

	glVertex2f(-0.81, -0.56);
	glVertex2f(-0.81, -0.66);
	glEnd();

	glColor3f(0.0, 1.0, 0.0); 
	glBegin(GL_QUADS);
	glVertex2f(-0.64, -0.64); 
	glVertex2f(-0.54, -0.64); 
	glVertex2f(-0.54, -0.74); 
	glVertex2f(-0.64, -0.74); 
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_LINES);
	
	glVertex2f(-0.64, -0.69);
	glVertex2f(-0.54, -0.69);
	
	glVertex2f(-0.59, -0.64);
	glVertex2f(-0.59, -0.74);
	glEnd();

	glColor3f(0.0, 0.0, 1.0); 
	glBegin(GL_QUADS);
	glVertex2f(-0.48, -0.56); 
	glVertex2f(-0.38, -0.56);
	glVertex2f(-0.38, -0.66);
	glVertex2f(-0.48, -0.66);
	glEnd();

	glColor3f(0.9, 0.9, 0.9); 
	glBegin(GL_LINES);
	
	glVertex2f(-0.48, -0.61);
	glVertex2f(-0.38, -0.61);
	
	glVertex2f(-0.43, -0.56);
	glVertex2f(-0.43, -0.66);
	glEnd();
}

void drawMoon() {
    glColor3f(1.0, 1.0, 0.8);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(0.8 + 0.1 * cos(angle), 0.7 + 0.1 * sin(angle));
    }
    glEnd();
}

void drawStreetLamp(float x, float y, float height) {

    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_QUADS);
    glVertex2f(x - 0.01, y);
    glVertex2f(x + 0.01, y);
    glVertex2f(x + 0.01, y + height);
    glVertex2f(x - 0.01, y + height);
    glEnd();

    glColor3f(1.0, 1.0, 0.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(x + 0.05 * cos(angle), y + height + 0.05 * sin(angle));
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_QUADS);
    glVertex2f(x - 0.06, y + height + 0.03);
    glVertex2f(x + 0.06, y + height + 0.03);
    glVertex2f(x + 0.06, y + height + 0.05);
    glVertex2f(x - 0.06, y + height + 0.05);
    glEnd();
}

void drawHouse() {
    
    glColor3f(0.9, 0.8, 0.7);
    glBegin(GL_QUADS);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.8, -0.6);
    glVertex2f(0.8, -0.2);
    glVertex2f(0.4, -0.2);
    glEnd();

    glColor3f(0.5, 0.2, 0.1); 
    glBegin(GL_TRIANGLES);
    glVertex2f(0.35, -0.2);
    glVertex2f(0.85, -0.2);
    glVertex2f(0.6, 0.2);
    glEnd();

    glColor3f(0.4, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0.55, -0.6);
    glVertex2f(0.65, -0.6);
    glVertex2f(0.65, -0.4);
    glVertex2f(0.55, -0.4);
    glEnd();

    glColor3f(windowColorR, windowColorG, windowColorB);
    glBegin(GL_QUADS);
    glVertex2f(0.43, -0.45);
    glVertex2f(0.51, -0.45);
    glVertex2f(0.51, -0.35);
    glVertex2f(0.43, -0.35);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(0.69, -0.45);
    glVertex2f(0.77, -0.45);
    glVertex2f(0.77, -0.35);
    glVertex2f(0.69, -0.35);
    glEnd();

    // ?ng kh i
    glColor3f(0.6, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(0.68, -0.05);
    glVertex2f(0.73, -0.05);
    glVertex2f(0.73, 0.15);
    glVertex2f(0.68, 0.15);
    glEnd();
    drawStreetLamp(0.9, -0.6, 0.5);
}

void drawPenguin(float x, float y, float width, float height) {
    
    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(x + width * cos(angle), y + height * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex2f(x + (width / 2) * cos(angle), y + (height / 2) * sin(angle));
    }
    glEnd();

    glColor3f(1.0, 1.0, 1.0); 
    glPointSize(3.0);
    glBegin(GL_POINTS);
    glVertex2f(x - width / 4, y + height / 3);
    glVertex2f(x + width / 4, y + height / 3);
    glEnd();

    glColor3f(0.0, 0.0, 0.0); 
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glVertex2f(x - width / 4, y + height / 3);
    glVertex2f(x + width / 4, y + height / 3);
    glEnd();

    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + height / 4);
    glVertex2f(x - width / 8, y + height / 6);
    glVertex2f(x + width / 8, y + height / 6);
    glEnd();

    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - width / 3, y - height / 2);
    glVertex2f(x - width / 6, y - height / 2);
    glVertex2f(x - width / 4, y - height / 3);

    glVertex2f(x + width / 3, y - height / 2);
    glVertex2f(x + width / 6, y - height / 2);
    glVertex2f(x + width / 4, y - height / 3);
    glEnd();
}

void drawLightsOnTree(float x, float y, float size) {
    float lights[9][2] = {
        {x, y - size / 5}, 
        {x - size / 6, y - size / 3}, 
        {x + size / 6, y - size / 2.5}, 
        {x - size / 6, y - size / 1.8}, 
        {x + size / 6, y - size / 1.6}, 
        {x - size / 8, y - size * 0.75}, 
        {x + size / 8, y - size * 0.85}, 
        {x - size / 8, y - size * 0.95}, 
        {x + size / 8, y - size * 1.05}
    };

    for (int i = 0; i < 9; ++i) {
        glColor3f(lightColors[currentLightColorIndex][0], lightColors[currentLightColorIndex][1], lightColors[currentLightColorIndex][2]);
        glPointSize(5.0);
        glBegin(GL_POINTS);
        glVertex2f(lights[i][0], lights[i][1]);
        glEnd();
    }
}

void drawSmallTree(float x, float y, float size) {
    glColor3f(0.1, 0.6, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - size / 2, y - size);
    glVertex2f(x + size / 2, y - size);
    glEnd();

    glColor3f(0.6, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 8, y - size);
    glVertex2f(x + size / 8, y - size);
    glVertex2f(x + size / 8, y - size - size / 4);
    glVertex2f(x - size / 8, y - size - size / 4);
    glEnd();

    drawLightsOnTree(x, y, size);
}

void drawPinkGiftBox(float x, float y, float size) {
   
    glColor3f(1.0, 0.5, 1.0); 
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();

    glColor3f(0.9, 0.9, 0.9); 
    glBegin(GL_LINES);
    glVertex2f(x - size / 2, y);
    glVertex2f(x + size / 2, y);
    glVertex2f(x, y - size / 2);
    glVertex2f(x, y + size / 2);
    glEnd();
}

void drawWoodenSignWithText(float x, float y, float width, float height, const char* text) {
    
    glColor3f(0.7, 0.4, 0.1); 
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y - height / 2);
    glVertex2f(x + width / 2, y - height / 2);
    glVertex2f(x + width / 2, y + height / 2);
    glVertex2f(x - width / 2, y + height / 2);
    glEnd();

    glColor3f(0.3, 0.3, 0.3);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(x - width / 2 + 0.02, y + height / 2 - 0.02);
    glVertex2f(x + width / 2 - 0.02, y + height / 2 - 0.02);
    glVertex2f(x - width / 2 + 0.02, y - height / 2 + 0.02);
    glVertex2f(x + width / 2 - 0.02, y - height / 2 + 0.02);
    glEnd();

    glColor3f(0.5, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.01, y - height / 2);
    glVertex2f(x + 0.01, y - height / 2);
    glVertex2f(x + 0.01, y - height / 2 - 0.2);
    glVertex2f(x - 0.01, y - height / 2 - 0.2);
    glEnd();

    glColor3f(colors[currentColorIndex][0], colors[currentColorIndex][1], colors[currentColorIndex][2]);
    glRasterPos2f(x - width / 4, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawAurora() {
    glBegin(GL_POLYGON);
    for (float i = 0; i <= 1.0; i += 0.01) {
        glColor3f(0.0, 1.0 * i, 1.0 - i); 
        glVertex2f(-1.0 + 2.0 * i, 0.8 + 0.1 * sin(i * 20.0));
    }
    for (float i = 1.0; i >= 0; i -= 0.01) {
        glColor3f(0.0, 1.0 * i, 1.0 - i); 
        glVertex2f(-1.0 + 2.0 * i, 0.7 + 0.1 * sin(i * 20.0));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (float i = 0; i <= 1.0; i += 0.01) {
        glColor3f(1.0 * i, 0.0, 1.0 - i); 
        glVertex2f(-1.0 + 2.0 * i, 0.9 + 0.1 * sin(i * 20.0));
    }
    for (float i = 1.0; i >= 0; i -= 0.01) {
        glColor3f(1.0 * i, 0.0, 1.0 - i);
        glVertex2f(-1.0 + 2.0 * i, 0.8 + 0.1 * sin(i * 20.0));
    }
    glEnd();
}
void drawSnowman() {
    // Thân du?i (hình c?u l?n)
    glColor3f(0.9, 0.9, 0.8);
    glPushMatrix();
    glTranslatef(0.0, -0.65, 0.0);
    glutSolidSphere(0.15, 50, 50);
    glPopMatrix();

    // Thân trên (hình c?u nh?)
    glPushMatrix();
    glTranslatef(0.0, -0.38, 0.0);
    glutSolidSphere(0.12, 50, 50);
    glPopMatrix();

    // Ð?u (hình c?u nh? nh?t)
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.0);
    glutSolidSphere(0.08, 50, 50);
    glPopMatrix();

    // M?t
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-0.02, -0.15, 0.07);
    glutSolidSphere(0.01, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.02, -0.15, 0.07);
    glutSolidSphere(0.01, 50, 50);
    glPopMatrix();

    // Mui (hình nón)
    glColor3f(1.0, 0.5, 0.0);
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.07);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.02, 0.1, 50, 50);
    glPopMatrix();
}
//void drawSnowman() {
//    glColor3f(0.9, 0.9, 0.8);
//    glBegin(GL_POLYGON);
//    for (int i = 0; i < 360; i++) {
//        float angle = i * M_PI / 180.0;
//        glVertex2f(0.0 + 0.15 * cos(angle), -0.65 + 0.15 * sin(angle));
//    }
//    glEnd();
//
//    glBegin(GL_POLYGON);
//    for (int i = 0; i < 360; i++) {
//        float angle = i * M_PI / 180.0;
//        glVertex2f(0.0 + 0.12 * cos(angle), -0.38 + 0.12 * sin(angle));
//    }
//    glEnd();
//
//    glBegin(GL_POLYGON);
//    for (int i = 0; i < 360; i++) {
//        float angle = i * M_PI / 180.0;
//        glVertex2f(0.0 + 0.08 * cos(angle), -0.18 + 0.08 * sin(angle));
//    }
//    glEnd();
//
//    glColor3f(0.0, 0.0, 0.0);
//    glPointSize(5.0);
//    glBegin(GL_POINTS);
//    glVertex2f(-0.02, -0.15);
//    glVertex2f(0.02, -0.15);
//    glEnd();
//
//    glColor3f(1.0, 0.5, 0.0);
//    glBegin(GL_TRIANGLES);
//    glVertex2f(0.0, -0.18);
//    glVertex2f(0.05, -0.18);
//    glVertex2f(0.0, -0.2);
//    glEnd();
//
//    glColor3f(0.5, 0.3, 0.1);
//    glLineWidth(3.0);
//    glBegin(GL_LINES);
//    glVertex2f(-0.12, -0.38);
//    glVertex2f(-0.25, -0.45);
//    glEnd();
//
//    glBegin(GL_LINES);
//    glVertex2f(0.12, -0.38);
//    glVertex2f(0.25, -0.45);
//    glEnd();
//
//    drawPenguin(-0.3, -0.7, 0.08, 0.12);  
//    drawPenguin(0.3, -0.8, 0.08, 0.12);   
// 
//    drawSmallTree(-0.8, -0.72, 0.2);  
//    drawSmallTree(0.8, -0.67, 0.2);   
//
//    drawPinkGiftBox(-0.5, -0.8, 0.1);
//	drawPinkGiftBox(0.5, -0.8, 0.1);
//	
//	drawWoodenSignWithText(-0.2, 0.015, 0.3, 0.2, "Christmas Eve");
//}
void drawSantaAndReindeer() {
    glColor3f(0.0f, 0.0f, 0.0f); // Màu den cho bóng

    // V? xe kéo
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, -0.05f);
    glVertex2f(0.1f, -0.05f);
    glVertex2f(0.1f, 0.05f);
    glVertex2f(-0.1f, 0.05f);
    glEnd();

    // V? ông già Noel (hình tròn)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.1f); // Tâm hình tròn
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.03f * cos(angle), 0.1f + 0.03f * sin(angle));
    }
    glEnd();

    // V? tu?n l?c (2 hình tròn làm thân và d?u)
    glBegin(GL_TRIANGLE_FAN); // Thân
    glVertex2f(0.2f, 0.0f); 
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.05f * cos(angle) + 0.2f, 0.05f * sin(angle));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Ð?u
    glVertex2f(0.25f, 0.05f); 
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.03f * cos(angle) + 0.25f, 0.03f * sin(angle) + 0.05f);
    }
    glEnd();

    // V? s?ng tu?n l?c
    glBegin(GL_LINES);
    glVertex2f(0.26f, 0.08f); glVertex2f(0.28f, 0.1f);
    glVertex2f(0.26f, 0.08f); glVertex2f(0.24f, 0.1f);
    glEnd();
}
void update_santa(int value) {
    santaX += 0.01f; // Di chuy?n sang ph?i
    if (santaX > 1.5f) { // N?u ra kh?i màn hình, quay l?i t? d?u
        santaX = -1.5f;
    }
    glutPostRedisplay(); // Yêu c?u v? l?i
    glutTimerFunc(16, update_santa, 0); // C?p nh?t sau 16ms (~60fps)
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//    glColor3f(0.0,0.3,0.7);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glScalef(tlx, tly, tlz);
    glTranslatef(posX, posY, posZ);
    // V? b?u tr?i
    glColor3f(0.0, 0.0, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(-1.0, 0.0);
    glEnd();

    glColor3f(0.95, 0.95, 0.95); 
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(-1.0, 0.0);
    glEnd();

    drawMoon();

    if (showAurora) {
        drawAurora(); 
    }

    drawStars(); 
    drawMountains();
    drawTree();
    drawHouse();
    drawSnowman();
    drawSmoke();
    drawSnowflakes();
    drawWoodenSignWithText(-0.2, 0.015, 0.3, 0.2, "Christmas Eve"); 

    if (showMessage) {
        glColor3f(1.0, 1.0, 1.0);  
        drawText("Merry Christmas", messageX, 0.0);
    }
	// Di chuy?n ông già Noel
    glPushMatrix();
    glTranslatef(santaX, 0.5f, 0.0f); // Di chuy?n ngang qua b?u tr?i
    drawSantaAndReindeer();
    glPopMatrix();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void update(int value) {
    updateSnowflakes();
    updateStars();
    updateSmoke();

    if (showAurora) {
        if (auroraStartTime == 0) {
            auroraStartTime = glutGet(GLUT_ELAPSED_TIME);
        }
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (currentTime - auroraStartTime > auroraDuration) {
            showAurora = false;
            auroraStartTime = 0;
        }
    }

    if (increasingBrightness) {
        windowColorR += 0.01;
        windowColorG += 0.01;
        windowColorB += 0.01;
        if (windowColorR >= 1.0) {
            increasingBrightness = false;
        }
    } else {
        windowColorR -= 0.01;
        windowColorG -= 0.01;
        windowColorB -= 0.01;
        if (windowColorR <= 0.2) {
            increasingBrightness = true;
        }
    }

    if (showMessage) {
        messageX += 0.005;
        if (messageX > 1.0) {
            messageX = -1.0;
        }
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (currentTime - messageStartTime > messageDuration) {
            showMessage = false; 
        }
    }

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lightColorChangeStartTime > lightColorChangeDuration) {
        currentLightColorIndex = (currentLightColorIndex + 1) % 7;
        lightColorChangeStartTime = currentTime;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': // Di chuy?n sang trái
        case 'A':
            posX -= moveSpeed;
            glutPostRedisplay();
            break;
        case 'd': // Di chuy?n sang ph?i
        case 'D':
            posX += moveSpeed;
            glutPostRedisplay();
            break;
        case 'w': // Di chuy?n lên
        case 'W':
            posY += moveSpeed;
            glutPostRedisplay();
            break;
        case 's': // Di chuy?n xu?ng
        case 'S':
            posY -= moveSpeed;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    srand(time(0));
    generateSnowflakes();
    generateStars();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Winter Scene");
	glutFullScreen();
    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update_santa, 0);
    glutKeyboardFunc(handleKeypress); 
    glutTimerFunc(25, update, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
