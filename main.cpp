#define _USE_MATH_DEFINES

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>
#include <random>
#include <algorithm>
#include <windows.h>
#include <MMSystem.h>
#include "vec.h"
#include "main.h"
#include "stl.h"

const int KEY_ESC = 27;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

// camera variables

bool isFocused = false;
vec3<float> camera_pos;
vec2<int> direction;

// target variables

const int NO_OBJECTS = 20;
const int NO_NG_OBJECTS = 4;
GLuint objects;
bool eliminated[NO_OBJECTS];
bool ng[NO_OBJECTS];
vec3<float> targetLoc[NO_OBJECTS];
const GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat gray[] = { 0.2, 0.2, 0.2, 1.0 };

// state

int second = 0;
int eliminated_ok_num = 0;
int score = 0;
bool isCleared = false;
bool isStarted = false;

std::random_device rnd;
std::mt19937 mt(rnd());

// gun model variables

Stl gun("ascii_USP45.stl");


void renderString(const std::string &str, float x, float y)
{
    // 参考 http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/lecture/iml/text/screen_character.html
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glColor3d(1.0, 1.0, 1.0);
        glLoadIdentity();
        gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glRasterPos2f(x, y);
            for (char c : str) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawGround()
{
    double ground_max_x = 500.0;
    double ground_max_z = 500.0;
        glPushMatrix();
            glColor3d(0.8, 0.8, 0.8);
            glBegin(GL_LINES);
            for(double lz = -ground_max_z; lz <= ground_max_z; lz+=20.0) {
                glVertex3d(-ground_max_x, 0, lz);
                glVertex3d(ground_max_x, 0, lz);
            }
            for(double lx = -ground_max_x; lx <= ground_max_x; lx+=20.0) {
                glVertex3d(lx, 0, ground_max_z);
                glVertex3d(lx, 0, -ground_max_z);
            }
            glEnd();
        glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // カメラの設定
    glLoadIdentity();
    gluLookAt(
        camera_pos.x, camera_pos.y, camera_pos.z,
        camera_pos.x + cos(direction.x * M_PI / 180.0), camera_pos.y + cos(direction.y * M_PI / 180.0), camera_pos.z + sin(direction.x * M_PI / 180.0),
        0.0, 1.0, 0.0
    );

    // ワールドの描画
    glPushMatrix();
        glPushMatrix();
            drawGround();
        glPopMatrix();

        glEnable(GL_DEPTH_TEST); /*Z Buffer enable*/
        glEnable(GL_LIGHTING);	 /*Shading計算の開始*/
        // ターゲットの描画
        if(isStarted) {
            glPushMatrix();
                for (int i = 0; i < NO_OBJECTS; i++) {
                    if(eliminated[i]) {
                        continue;
                    }

                    if(ng[i]) {
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
                    } else {
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
                    }
                    glCallList(objects + i);
                }
            glPopMatrix();
        }

        // 銃の描画
        // https://free3d.com/3d-model/usp-45-44387.html
        glPushMatrix();
            glTranslatef(camera_pos.x + 1.8 * cos(direction.x * (M_PI / 180.0) + (M_PI / 4.0)), camera_pos.y + cos(direction.y * (M_PI / 180.0)) - 0.7, camera_pos.z + 1.8 * sin(direction.x * (M_PI / 180.0) + (M_PI / 4.0)));
            glRotatef(direction.y - 95, sin(direction.x * M_PI / 180.0), 0, -cos(direction.x * (M_PI / 180.0)));
            glRotatef(-90, 1, 0, 0);
            glRotatef(-direction.x + 180, 0, 0, 1);
            glScalef(0.2, 0.2, 0.2);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0 );
            for(int i=0; i < gun.tnum; i++) {
                glBegin(GL_POLYGON);
                    glNormal3f(gun.nx[i], gun.ny[i], gun.nz[i]);
                    glVertex3f(gun.x[3*i+0], gun.y[3*i+0], gun.z[3*i+0]);
                    glVertex3f(gun.x[3*i+1], gun.y[3*i+1], gun.z[3*i+1]);
                    glVertex3f(gun.x[3*i+2], gun.y[3*i+2], gun.z[3*i+2]);
                glEnd();
            }
        glPopMatrix();
        glDisable(GL_LIGHTING); /*Shading計算終了*/
        glDisable(GL_DEPTH_TEST);

        // カーソルの代わりに円を描画
        glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(camera_pos.x + cos(direction.x * M_PI / 180.0), camera_pos.y + cos(direction.y * M_PI / 180.0), camera_pos.z + sin(direction.x * M_PI / 180.0));
            glutSolidSphere(0.005, 20, 20);
        glPopMatrix();
    glPopMatrix();

    // UIの描画
    glPushMatrix();
        renderString(std::to_string(second) + " seconds elapsed", 10, 30);
        renderString(std::to_string(score) + " score", 10, 60);

        if(!isStarted) {
            glColor3f(0, 0, 0);
            renderString("PRESS 'S' TO START", WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2);
        }

        if(isCleared) {
            glColor3f(0, 1.0, 0);
            renderString("GAME CLEAR! Score " + std::to_string(score) + " (" + std::to_string(second) + " seconds)", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 120);
        }
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case KEY_ESC:
        if(isFocused) {
            isFocused = false;
#ifdef DEBUG
            std::cout << "Focus end" << std::endl;
#endif
            glutSetCursor(GLUT_CURSOR_INHERIT);
            // 負荷の軽減になるか？
            glutMotionFunc(nullptr);
            glutPassiveMotionFunc(nullptr);
        }
        break;
    case 's':
        // game start
        if(!isStarted) {
            initState();
            initTarget();
            isStarted = true;
            std::cout << "Game Start!" << std::endl;
            PlaySound(TEXT("sounds/start.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            glutTimerFunc(1000, timer, 0);
        }
        break;
    case 'r':
        // reset
        initState();
        initTarget();
        break;
    }
}

void pickUp(int x, int y)
{
    const int SELECTION_MAX = 100;
    GLuint selectBuf[SELECTION_MAX];  /* セレクションバッファ　　　　　 */
    GLuint hits = 0;                  /* セレクトされたオブジェクトの数 */
    
    GLuint *ptr;
    GLint viewport[4];

    if(!isStarted) {
        return;
    }

    glSelectBuffer(SELECTION_MAX, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glGetIntegerv(GL_VIEWPORT, viewport);
        // 表示領域をマウス周辺に制限
        gluPickMatrix(x, y, 2, 2, viewport);
        gluPerspective(30.0, (double)viewport[2] / (double)viewport[3], 1.0, 100.0);

        glMatrixMode(GL_MODELVIEW);
        // ターゲットの描画
        for (int i=0; i < NO_OBJECTS; i++) {
            if(!eliminated[i]) {
                glLoadName(i);
                glCallList(objects + i);
            }
        }

        glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopName();

    hits = glRenderMode(GL_RENDER);
#ifdef DEBUG
    std::cout << hits << " hits!" << std::endl;
#endif

    if (hits > 0) {
        ptr = selectBuf;
        // ptr[3]に選択されたオブジェクトの番号が入る
        int num = ptr[3];
#ifdef DEBUG
        std::cout << ptr[0] << ", " << ptr[1] << ", " << ptr[2] << ", " << ptr[3] << ", " << ptr[4] << ", " << ptr[5] << ", " << ptr[6] << ", " << ptr[7] << std::endl;
        std::cout << "You clicked " << num  << "(" << targetLoc[num].x << "," << targetLoc[num].y << "," << targetLoc[num].z << ")" << std::endl;
#endif
        eliminated[num] = 1; /* オブジェクトを描画しないようにする */
        if(ng[num]) {
            score -= 2;
        } else {
            score += 1;
            eliminated_ok_num++;
        }

        if(eliminated_ok_num == NO_OBJECTS - NO_NG_OBJECTS) {
            std::cout << "GAME CLEAR!: " << second << " seconds" << std::endl;
            isCleared = true;
            isStarted = false;
            PlaySound(TEXT("sounds/clear.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
}

void motion(int x, int y)
{
    if (isFocused) {
        // std::cout << "focusing" << std::endl;
        int dx = x - (WINDOW_WIDTH / 2.0);
        int dy = y - (WINDOW_HEIGHT / 2.0);

        if(dx != 0 || dy != 0) {
            direction.x = (direction.x + dx) % 360;
            direction.y = direction.y + dy;

            // 上向き最大
            if(direction.y < 60) {
                direction.y = 60;
            }
            // 下向き最大
            if(direction.y > 150) {
                direction.y = 150;
            }
            // std::cout << "dy = " << dy << ", " << direction.y << std::endl;
            glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (!isFocused)
        {
#ifdef DEBUG
            std::cout << "Focus start" << std::endl;
#endif
            isFocused = true;
            glutSetCursor(GLUT_CURSOR_NONE);
            glutMotionFunc(motion);
            glutPassiveMotionFunc(motion);
        } else {
            if (state == GLUT_DOWN) {
                // FIXME: 遅延する可能性があるのでメモリ上に展開したい
                PlaySound(TEXT("sounds/shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
                pickUp(x, y);
            }
        }
        break;
    default:
        break;
    }
}

void reshape(int width, int height)
{
#ifdef DEBUG
    std::cout << "Resize: " << width << "x" << height << std::endl;
#endif
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
    if(isCleared || !isStarted) {
        return;
    }
    second++;
    glutTimerFunc(1000, timer, 0);
}

void initTarget()
{
    std::uniform_real_distribution<double> randTheta(-M_PI, M_PI);
    std::uniform_int_distribution<int> randY(-6, 6);
    std::uniform_real_distribution<double> randR(100.0, 400.0);
    double r, theta;
    std::vector<int> rndInt;
    for(int i=0; i<NO_OBJECTS; i++) {
        rndInt.emplace_back(i);
    }

    std::shuffle(rndInt.begin(), rndInt.end(), mt);
#ifdef DEBUG
    for(int i=0; i<NO_OBJECTS; i++) {
        std::cout << rndInt[i] << " ";
    }
#endif

    glPushMatrix();
    glLoadIdentity();

    for(int i=0; i<NO_NG_OBJECTS; i++) {
        ng[rndInt[i]] = true;
    }
    
    for(int i=0; i < NO_OBJECTS; i++)
    {
        // 極座標系で考える
        r = std::sqrt(randR(mt));
        theta = randTheta(mt);
        targetLoc[i].x = r * std::cos(theta);
        targetLoc[i].y = camera_pos.y + randY(mt);
        targetLoc[i].z = r * std::sin(theta);

        glNewList(objects + i, GL_COMPILE);
            // draw sphere
            glPushMatrix();
                glTranslatef(targetLoc[i].x, targetLoc[i].y, targetLoc[i].z);
                glutSolidSphere(1, 10, 10);
            glPopMatrix();
        glEndList();
    }
    glPopMatrix();
}

void initState()
{
    isCleared = false;
    isStarted = false;
    second = 0;
    eliminated_ok_num = 0;
    score = 0;
    for(int i=0; i < NO_OBJECTS; i++) {
        ng[i] = false;
        eliminated[i] = false;
    }
}

void init(char *progname)
{
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    camera_pos.x = 0.0;
    camera_pos.y = 50.0;
    camera_pos.z = -1.0;
    direction.x = 0;
    direction.y = 90;

    objects = glGenLists(NO_OBJECTS);

    // set fallback functions
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    initState();
    initTarget();

    glEnable(GL_LIGHT0); /*0番のライトを利用可能にする*/
}

void idle()
{
    if(isFocused) {
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init(argv[0]);
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutMainLoop();
    return (0);
}
