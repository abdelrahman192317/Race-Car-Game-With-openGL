#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <gl/stb_image.h>
#include <GL/glut.h>


unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;

float ratio;
bool fullscreen = true;
int i = 0;

float FPS = 1;
int gv = 0, score = 0, hight_score = 0;
bool pause = false;

float plus_z = 13;
float eyex = 0, eyey = 0, eyez = 50 + 13, upz = -1;

float xMin=-10, xMax=10, yMin=-5.5, yMax=5.5, zMin=-100, zMax=50;

float z = 0, z1 = 0, z2 = zMax + abs(zMin);

float lrIndex = 0;
float car = 0, lrIndex0 = 0;
float car1, lrIndex1, car2, lrIndex2, car3, lrIndex3;
float incre1 = 0;
float incre = -500;

char s[50];

//Funcations
void spe_key(int, int, int);
void processKeys(unsigned char, int, int);
void timer(int);
void init();
void reshape(int, int);

void start();
void road();
void sky();
void renderBitmapString(float, float, float, void*, const char*); //display text on the screen
void tree(float, float, int);
void trees(int);
void road_backbround();
void score_board();
void cars(int);
void road_midle();
void Race();
void collision();
void flag();

void load(int);
void check(unsigned char*);


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500,650);
    glutInitWindowPosition(200,20);
    glutCreateWindow("Racing Car");
	glutFullScreen();

    init();
    glutDisplayFunc(Race);
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(spe_key);

    glutMainLoop();

    return 0;
}


void init() {
    glClearColor(0.3, 0.3, 0.3, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}
void reshape(int w, int h) {
    if (h == 0)h = 1;
    ratio = w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ratio, 1, z2 + .1);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(100/(FPS * 80), timer, 0);
    if (gv == 1 && !pause) {
        if (FPS < 1)FPS += 0.01;
        score += FPS;

        car1 += FPS;
        if (car1 > z2) {
            car1 = 0;
            lrIndex1 = ((rand() % 3 + 1) * 3) + xMin + 4;
        }

        car2 += FPS;
        if (car2 > z2) {
            car2 = 0;
            lrIndex2 = ((rand() % 3 + 1) * 3) + xMin + 4;
        }

        car3 += FPS;
        if (car3 > z2) {
            car3 = 0;
            lrIndex3 = ((rand() % 3 + 1) * 3) + xMin + 4;
        }

        incre1 += FPS / 2;
        if (incre1 > z2) incre1 = 0;

        incre += FPS;
    }
}
void spe_key(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            fullscreen = !fullscreen;
            if (fullscreen) {
                glutFullScreen();
            }
            else {
                glutPositionWindow(100, 80);
                glutReshapeWindow(1000,800);
            }
            break;
        case GLUT_KEY_DOWN:
            eyey--;
            break;
        case GLUT_KEY_UP:
            eyey++;
            break;
        case GLUT_KEY_LEFT:
            eyex--;
            break;
        case GLUT_KEY_RIGHT:
            eyex++;
            break;
        default:
            break;
    }
}
void processKeys(unsigned char key, int x, int y) {
    switch (key)
    {
    case 13: // Enter To Start 
        start();
        break;
    case 32: // pause
        if (gv == 1) pause = !pause;
        break;
    case 'W':
    case 'w': // forward 
        if (gv == 1 && !pause && FPS < 1) FPS += 0.05;
        break;
    case 'S':
    case 's': // back
        if (gv == 1 && !pause && FPS > 0.1) FPS -= 0.1;
        break;
    case 'A':
    case 'a': // left
        if (lrIndex > -3 && gv == 1 && !pause)lrIndex -= 3;
        break;
    case 'D':
    case 'd': // right
        if (lrIndex < 3 && gv == 1 && !pause)lrIndex += 3;
        break;
    case 'I':
    case 'i': // zoom in
        eyez--;
        break;
    case 'O':
    case 'o': // zoom out
        eyez++;
        break;
    case 'R':
    case 'r': // reset
        eyex = 0; eyey = 0; eyez = 63;
        break;
    case 27:  // Escape for exit
        exit(0);
        break;
    default:
        break;
    }
}

void start() {
    if (gv != 1) {
        gv = 1;
        FPS = 0.1;
        score = 0;

        incre1 = 0;
        incre = -500;

        lrIndex = 0;

        car1 = 0;
        lrIndex1 = ((rand() % 3 + 1) * 3) + xMin + 4;
        car2 = -35;
        lrIndex2 = ((rand() % 3 + 1) * 3) + xMin + 4;
        car3 = -70;
        lrIndex3 = ((rand() % 3 + 1) * 3) + xMin + 4;
    }
}

void Race() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(eyex, eyey, eyez, 0, 0, upz, 0, 1, 0);

    sky();
    road();

    cars(0);
    cars(1);
    cars(2);
    cars(3);

    trees(incre1);
    trees(incre1 - z2);

    score_board();

    collision();

    flag();
    
    glutSwapBuffers();
}

void sky() {

    load(2);

    //sky right
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glTexCoord2d(10, 3);
    glVertex3f(xMax, yMin, zMax);
    glTexCoord2d(0, 3);
    glVertex3f(xMax, yMin, zMin);
    glTexCoord2d(0, 0);
    glVertex3f(xMax, yMax, zMin);
    glTexCoord2d(10, 0);
    glVertex3f(xMax, yMax, zMax);

    //sky left
    glTexCoord2d(10, 3);
    glVertex3f(xMin, yMin, zMax);
    glTexCoord2d(0, 3);
    glVertex3f(xMin, yMin, zMin);
    glTexCoord2d(0, 0);
    glVertex3f(xMin, yMax, zMin);
    glTexCoord2d(10, 0);
    glVertex3f(xMin, yMax, zMax);

    //sky top
    glVertex3f(xMin, yMax, zMax);
    glVertex3f(xMax, yMax, zMax);
    glVertex3f(xMax, yMax, zMin);
    glVertex3f(xMin, yMax, zMin);
    
    glEnd();
}

void road() {
    //Road
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(3 * xMin / 5, yMin, zMax);
    glVertex3f(1 * xMin / 5, yMin, zMax);
    glVertex3f(1 * xMin / 5, yMin, zMin);
    glVertex3f(3 * xMin / 5, yMin, zMin);

    glVertex3f(.75 * xMin / 5, yMin, zMax);
    glVertex3f(.75 * xMax / 5, yMin, zMax);
    glVertex3f(.75 * xMax / 5, yMin, zMin);
    glVertex3f(.75 * xMin / 5, yMin, zMin);

    glVertex3f(1 * xMax / 5, yMin, zMax);
    glVertex3f(3 * xMax / 5, yMin, zMax);
    glVertex3f(3 * xMax / 5, yMin, zMin);
    glVertex3f(1 * xMax / 5, yMin, zMin);
    glEnd();

    road_midle();
    road_backbround();
}
void road_midle() {
    //Road Midle
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(1 * xMin / 5, yMin, zMax);
    glVertex3f(.75 * xMin / 5, yMin, zMax);
    glVertex3f(.75 * xMin / 5, yMin, zMin);
    glVertex3f(1 * xMin / 5, yMin, zMin);

    glVertex3f(.75 * xMax / 5, yMin, zMax);
    glVertex3f(1 * xMax / 5, yMin, zMax);
    glVertex3f(1 * xMax / 5, yMin, zMin);
    glVertex3f(.75 * xMax / 5, yMin, zMin);
    glEnd();
}
void road_backbround() {
    //Road side
    glBegin(GL_QUADS);
    glColor3f(0, 1, 0);
    glVertex3f(xMin, yMin, zMax);
    glVertex3f(3.25 * xMin / 5, yMin, zMax);
    glColor3f(.80, 1.00, 0.000);
    glVertex3f(3.25 * xMin / 5, yMin, zMin);
    glVertex3f(xMin, yMin, zMin);

    glColor3f(.5, .5, .5);
    glVertex3f(3.25 * xMin / 5, yMin, zMax);
    glVertex3f(3 * xMin / 5, yMin, zMax);
    glVertex3f(3 * xMin / 5, yMin, zMin);
    glVertex3f(3.25 * xMin / 5, yMin, zMin);

    glVertex3f(3 * xMax / 5, yMin, zMax);
    glVertex3f(3.25 * xMax / 5, yMin, zMax);
    glVertex3f(3.25 * xMax / 5, yMin, zMin);
    glVertex3f(3 * xMax / 5, yMin, zMin);

    glColor3f(0, 1, 0);
    glVertex3f(3.25 * xMax / 5, yMin, zMax);
    glVertex3f(xMax, yMin, zMax);
    glColor3f(1.000, 1.00, 0.000);
    glVertex3f(xMax, yMin, zMin);
    glVertex3f(3.25 * xMax / 5, yMin, zMin);
    glEnd();

}

void cars(int n) {
    if (n == 0) { z = zMax; lrIndex0 = lrIndex; glColor3f(0, 0, 1); }
    else if (n == 1) { z = car1 + zMin; lrIndex0 = lrIndex1; glColor3f(1, 0.1, 0.9); }
    else if (n == 2) { z = car2 + zMin; lrIndex0 = lrIndex2; glColor3f(1, 0.7, 0.5); }
    else if (n == 3) { z = car3 + zMin; lrIndex0 = lrIndex3; glColor3f(1, 0.9, 0.1); }

    //right
    glBegin(GL_POLYGON);
    glVertex3f(lrIndex0 + .4, yMin + .25, z - 2.4);
    glVertex3f(lrIndex0 + .4, yMin + .5, z - 2.4);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - 2);
    glVertex3f(lrIndex0 + .4, yMin + 1, z - 1.95);
    glVertex3f(lrIndex0 + .4, yMin + 1, z - 1.05);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - 1);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - .6);
    glVertex3f(lrIndex0 + .4, yMin + .25, z - .6);
    glEnd();

    //left
    glBegin(GL_POLYGON);
    glVertex3f(lrIndex0 - .4, yMin + .25, z - .6);
    glVertex3f(lrIndex0 - .4, yMin + .25, z - 2.4);
    glVertex3f(lrIndex0 - .4, yMin + .5, z - 2.4);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - 2);
    glVertex3f(lrIndex0 - .4, yMin + 1, z - 1.95);
    glVertex3f(lrIndex0 - .4, yMin + 1, z - 1.05);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - 1);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - .6);
    glEnd();

    glBegin(GL_QUADS);
    //back
    glVertex3f(lrIndex0 - .4, yMin + .25, z - .6);
    glVertex3f(lrIndex0 + .4, yMin + .25, z - .6);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - .6);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - .6);

    if (n == 0) { z = zMax; lrIndex0 = lrIndex; glColor3f(0, 0.3, 1); }
    else if (n == 1) { z = car1 + zMin; lrIndex0 = lrIndex1; glColor3f(1, 0.1, 0.5); }
    else if (n == 2) { z = car2 + zMin; lrIndex0 = lrIndex2; glColor3f(1, 0.2, 0.5); }
    else if (n == 3) { z = car3 + zMin; lrIndex0 = lrIndex3; glColor3f(1, 0.2, 0.5); }
    
    //top car
    glVertex3f(lrIndex0 - .4, yMin + 1, z - 1.05);
    glVertex3f(lrIndex0 + .4, yMin + 1, z - 1.05);
    glVertex3f(lrIndex0 + .4, yMin + 1, z - 1.95);
    glVertex3f(lrIndex0 - .4, yMin + 1, z - 1.95);

    //back top
    glVertex3f(lrIndex0 - .4, yMin + .75, z - .6);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - .6);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - 1);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - 1);

    //back glass
    glColor3f(0.4, 0.4, 0.4);
    glVertex3f(lrIndex0 - .4, yMin + .75, z - 1);
    glVertex3f(lrIndex0 + .4, yMin + .75, z - 1);
    glVertex3f(lrIndex0 + .4, yMin + 1, z - 1.05);
    glVertex3f(lrIndex0 - .4, yMin + 1, z - 1.05);
    glEnd();

    //wheels
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.2);
    //back right wheel
    glTranslatef(lrIndex0 + .4, yMin + .25, z - 1);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 25);
    glPopMatrix();

    glPushMatrix();
    //back left wheel
    glTranslatef(lrIndex0 - .4, yMin + .25, z - 1);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 25);
    glPopMatrix();

    glPushMatrix();
    //front right wheel
    glTranslatef(lrIndex0 + .4, yMin + .25, z - 2);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 25);
    glPopMatrix();

    glPushMatrix();
    //front left wheel
    glTranslatef(lrIndex0 - .4, yMin + .25, z - 2);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 25);
    glPopMatrix();
}

void tree(float x, float z, int n) {

    //bottom
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_QUADS);
    for (i = 0; i < 360; i += 5) {
        glVertex3f(x + cos(i)/4, yMin + (yMax / 4.6), n + z + sin(i)/4);
        glVertex3f(x + cos(i)/4, yMin, n + z + sin(i)/4);
    }
    glEnd();

    //top
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < 360; i+= 5) {
        glVertex3f(x + cos(i), yMin + (yMax / 4.6), n + z + sin(i));
        glVertex3f(x + cos(i + 6), yMin + (yMax / 4.6), n + z + sin(i + 6));
        glVertex3f(x, yMin + (yMax / 1.8), n + z);
    }
    glEnd();
}
void trees(int incre) {
    z1 = ((zMax + abs(zMin)) / 10);
    //left
    tree(4 * xMin / 5, zMax, incre);
    tree(4 * xMin / 5, zMax - 1 * z1, incre);
    tree(4 * xMin / 5, zMax - 2 * z1, incre);
    tree(4 * xMin / 5, zMax - 3 * z1, incre);
    tree(4 * xMin / 5, zMax - 4 * z1, incre);
    tree(4 * xMin / 5, zMax - 5 * z1, incre);
    tree(4 * xMin / 5, zMax - 6 * z1, incre);
    tree(4 * xMin / 5, zMax - 7 * z1, incre);
    tree(4 * xMin / 5, zMax - 8 * z1, incre);
    tree(4 * xMin / 5, zMax - 9 * z1, incre);

    //right
    tree(4 * xMax / 5, zMax, incre);
    tree(4 * xMax / 5, zMax - 1 * z1, incre);
    tree(4 * xMax / 5, zMax - 2 * z1, incre);
    tree(4 * xMax / 5, zMax - 3 * z1, incre);
    tree(4 * xMax / 5, zMax - 4 * z1, incre);
    tree(4 * xMax / 5, zMax - 5 * z1, incre);
    tree(4 * xMax / 5, zMax - 6 * z1, incre);
    tree(4 * xMax / 5, zMax - 7 * z1, incre);
    tree(4 * xMax / 5, zMax - 8 * z1, incre);
    tree(4 * xMax / 5, zMax - 9 * z1, incre);

}

void flag() {

    //end
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glVertex3f((3.5 * xMin / 5), 0, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 0, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 0.5, (zMin + incre));
    glVertex3f((3.5 * xMin / 5), 0.5, (zMin + incre));

    glColor3f(0, 0, 0);
    glVertex3f((3.5 * xMin / 5), 0.5, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 0.5, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 1, (zMin + incre));
    glVertex3f((3.5 * xMin / 5), 1, (zMin + incre));

    glColor3f(1, 1, 1);
    glVertex3f((3.5 * xMin / 5), 1, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 1, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 1.5, (zMin + incre));
    glVertex3f((3.5 * xMin / 5), 1.5, (zMin + incre));

    glColor3f(0, 0, 0);
    glVertex3f((3.5 * xMin / 5), 1.5, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 1.5, (zMin + incre));
    glVertex3f((3.5 * xMax / 5), 2, (zMin + incre));
    glVertex3f((3.5 * xMin / 5), 2, (zMin + incre));
    glEnd();

    //bottom left
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < 360; i += 5) {
        glVertex3f((3.5 * xMin / 5) + (cos(i) / 4), 2, (zMin + incre) + (sin(i) / 4));
        glVertex3f((3.5 * xMin / 5) + (cos(i) / 4), yMin, (zMin + incre) + (sin(i) / 4));
    }
    glEnd();
    
    //bottom right
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < 360; i += 5) {
        glVertex3f((3.5 * xMax / 5) + (cos(i) / 4), 2, (zMin + incre) + (sin(i) / 4));
        glVertex3f((3.5 * xMax / 5) + (cos(i) / 4), yMin, (zMin + incre) + (sin(i) / 4));
    }
    glEnd();


}

void score_board() {
    //Text Information in Frist Page
    if (gv == 0) {
        glColor3f(1, 1, 0);
        renderBitmapString(xMin/1.7, -1, zMin + plus_z, GLUT_BITMAP_TIMES_ROMAN_24, "START");
    }
    else if (gv == 1 && !pause) {
        //Print Score
        glColor3f(0, 1, 0);
        sprintf(s, "SCORE: %d", score);
        renderBitmapString(xMin, yMax - yMax/10, zMax - 1, (void*)GLUT_BITMAP_HELVETICA_18, s);
        sprintf(s, "SPEED:%dKm/h", int(FPS * 50));
        renderBitmapString(xMin, yMax - yMax/5, zMax - 1, (void*)GLUT_BITMAP_HELVETICA_18, s);
    }
    else if (gv == 1 && pause) {
        //Print Score
        glColor3f(0, 1, 0);
        renderBitmapString(xMin/1.7, -1, zMin + plus_z, (void*)GLUT_BITMAP_TIMES_ROMAN_24, "Paused");
    }
    else if (gv == 2) {
        glColor3f(1, 1, 0);
        renderBitmapString(xMin / 1.2, -1, zMin + plus_z, GLUT_BITMAP_HELVETICA_12, "Play Again");

        glColor3f(1, 0, 0);
        renderBitmapString(xMin, yMax - yMax / 10, zMax - 1, (void*)GLUT_BITMAP_TIMES_ROMAN_24, "Win");

        glColor3f(1, 0, 1);
        sprintf(s, "Your Score: %d", (score * 50));
        renderBitmapString(xMin, yMax - yMax / 5, zMax - 1, GLUT_BITMAP_HELVETICA_18, s);
        sprintf(s, "High Score: %d", hight_score);
        renderBitmapString(xMin, yMax - yMax / 3.3, zMax - 1, GLUT_BITMAP_HELVETICA_18, s);
    }
    else if (gv == 3) {
        glColor3f(1, 1, 0);
        renderBitmapString(xMin/1.2, -1, zMin + plus_z, GLUT_BITMAP_HELVETICA_12, "Play Again");

        glColor3f(1, 0, 0);
        renderBitmapString(xMin, yMax - yMax / 10, zMax - 1, (void*)GLUT_BITMAP_HELVETICA_18, "GAME OVER");

        glColor3f(1, 0, 1);
        sprintf(s, "Your Score: %d", (score * 50));
        renderBitmapString(xMin, yMax - yMax/5, zMax - 1, GLUT_BITMAP_HELVETICA_18, s);
        sprintf(s, "High Score: %d", hight_score);
        renderBitmapString(xMin, yMax - yMax/3.3, zMax - 1, GLUT_BITMAP_HELVETICA_18, s);
    }   
}

void renderBitmapString(float x, float y, float z, void* font, const char* string) {
    const char* c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void collision() {
    //KIll check car
    if (lrIndex == lrIndex1 && (car1 >= z2 - 2.4)) {
        gv = 3;
        if (score > hight_score) hight_score = score;
        car1-=.5;
    }
    else if (lrIndex == lrIndex2 && (car2 >= z2 - 2.4)) {
        gv = 3;
        if (score > hight_score) hight_score = score;
        car2 -= .5;
    }
    else if (lrIndex == lrIndex3 && (car3 >= z2 - 2.4)) {
        gv = 3;
        if (score > hight_score) hight_score = score;
        car3 -= .5;
    }
    else if (incre > z2 - 2) {
        gv = 2;
        if (score > hight_score) hight_score = score;
    }
}

void load(int imgnum) {
    if (imgnum == 1) {
        data = stbi_load("flag.png", &width, &height, &nrChannels, 0);
        check(data);
    }
    else if (imgnum == 2) {
        data = stbi_load("wall.bmp", &width, &height, &nrChannels, 0);
        check(data);
    }
}

void check(unsigned char* data) {
    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}