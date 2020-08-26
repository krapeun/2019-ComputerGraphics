// Lab12 12171788 박지은

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <GL/freeglut.h>
#include <string.h>
#include "bmpfuncs.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

float PI = 3.141592;
float theta = 45; // 위도
float phi = 45; // 경도
int radius = 18;
double upY = cos(phi*PI / 180); // up-vector

// 카메라 좌표
float cameraX = 0;
float cameraY = 0;
float cameraZ = 0;

// texture mapping set variable 
GLuint cube[6]; // cube
GLuint cylinder[3];   // cylinder
GLuint *sphere = new GLuint(); // sphere

// Quadric 객체 생성
GLUquadricObj* m_pQuadric = gluNewQuadric();

void init(void);
void resize(int, int);

void draw(void);
void draw_cube(void);
void draw_cylinder(void);
void draw_sphere(void);
void draw_string(void*, const char*, float, float, float, float, float);

void mouse(int button, int state, int x, int y);
void mousewheel(int but, int dir, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);

void Default(void);

void cube_TextureMapping(void);
void cylinder_TextureMapping(void);
void sphere_TextureMapping(void);

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue);
void draw_axis(void);
void draw(void);

int main(int argc, char **argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glClearDepth(1.0f);
   // double buffer로 변경
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("12171788 박지은");
   init();

   // quadric object 속성 설정
   gluQuadricDrawStyle(m_pQuadric, GLU_FILL);
   gluQuadricNormals(m_pQuadric, GLU_SMOOTH);

   glutDisplayFunc(draw); // draw 콜백 함수
   glutReshapeFunc(&resize); // resize 콜백 함수
   glutMouseFunc(mouse);
   glutMouseWheelFunc(mousewheel);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeyboard);

   glutMainLoop();

   return 0;
}

void init(void) {
   glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
   //glColor3f(1.0f, 1.0f, 1.0f);
   glutReshapeFunc(resize);

   // Light 설정
   GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
   GLfloat light_specular[] = { 0.9f, 0.9f, 0.9f, 0.9f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glEnable(GL_LIGHTING);

   // Material 설정
   GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat material_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

   glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 64);

   // 물체의 컬러 속성 설정
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // Depth-Test Enable
   glFrontFace(GL_CW);
   glEnable(GL_DEPTH_TEST);

   // Texture Mapping 설정
   cube_TextureMapping();
   cylinder_TextureMapping();
   sphere_TextureMapping();
   gluQuadricTexture(m_pQuadric, GL_TRUE);

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glEnable(GL_TEXTURE_2D); // texture 활성화
}

void resize(int width, int height) {
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, (float)width / (float)height, 1, 500);
   // Projection Transformation
   glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
   printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
   glutPostRedisplay();
}

void mousewheel(int but, int dir, int x, int y) {
   if (dir > 0) { // 위로
      if (radius > 2) radius--;
   }
   else { // 아래로
      if (radius < 180) radius++;
   }
   glutPostRedisplay();
}

void motion(int x, int y) {
   printf("Mouse is moving! (%d, %d)\n", x, y);
   glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
   printf("You pressed %c\n", key);
   glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
   printf("You pressed %c\n", key);
   switch (key) {
   case GLUT_KEY_LEFT:
      theta --;
      if (theta <= -360) theta = 0;
      break;
   case GLUT_KEY_RIGHT:
      theta ++;
      if (theta >= 360) theta = 0;
      break;
   case GLUT_KEY_DOWN:
      phi --;
      if (abs((int)phi) % 360 == 0) phi = 0;
      break;
   case GLUT_KEY_UP:
      phi ++;
      if (abs((int)phi) % 360 == 0) phi = 0;
      break;
   }
   glutPostRedisplay();
}

void Default() {
   radius = 18;
   theta = 45.0;
   phi = 45.0;
}

void draw_cube() {
   glColor3f(1.0, 1.0, 1.0);
   // Left
   glBindTexture(GL_TEXTURE_2D, cube[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1.0f, 0, 0); // -x축
   glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
   glTexCoord2f(3, 0); glVertex3f(-1.0, -1.0, -1.0);
   glTexCoord2f(3, 3); glVertex3f(-1.0, -1.0, 1.0);
   glTexCoord2f(0, 3); glVertex3f(-1.0, 1.0, 1.0);
   glEnd();

   // Right
   glBindTexture(GL_TEXTURE_2D, cube[1]);
   glBegin(GL_QUADS);
   glNormal3f(1.0f, 0, 0);   // x축
   glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
   glTexCoord2f(2, 0); glVertex3f(1.0, -1.0, 1.0);
   glTexCoord2f(2, 2); glVertex3f(1.0, -1.0, -1.0);
   glTexCoord2f(0, 2); glVertex3f(1.0, 1.0, -1.0);
   glEnd();

   // Bottom
   glBindTexture(GL_TEXTURE_2D, cube[2]);
   glBegin(GL_QUADS);
   glNormal3f(0, -1.0f, 0); // -y축
   glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
   glTexCoord2f(2, 0); glVertex3f(1.0, -1.0, -1.0);
   glTexCoord2f(2, 2); glVertex3f(1.0, -1.0, 1.0);
   glTexCoord2f(0, 2); glVertex3f(-1.0, -1.0, 1.0);
   glEnd();

   // Top
   glBindTexture(GL_TEXTURE_2D, cube[3]);
   glBegin(GL_QUADS);
   glNormal3f(0, 1.0f, 0);   // y축
   glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
   glTexCoord2f(3, 0); glVertex3f(1.0, 1.0, 1.0);
   glTexCoord2f(3, 3); glVertex3f(1.0, 1.0, -1.0);
   glTexCoord2f(0, 3); glVertex3f(-1.0, 1.0, -1.0);
   glEnd();

   // Near
   glBindTexture(GL_TEXTURE_2D, cube[4]);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, 1.0f);   // z축
   glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
   glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, 1.0);
   glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
   glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, 1.0);
   glEnd();

   // Far
   glBindTexture(GL_TEXTURE_2D, cube[5]);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, -1.0f); // -z축
   glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
   glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, -1.0);
   glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, -1.0);
   glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, -1.0);
   glEnd();
}

void cube_TextureMapping() {
   glGenTextures(6, cube); // Texture Name 할당
   int imgWidth, imgHeight, channels;

   for (int i = 0; i < 6; i++) {
      glBindTexture(GL_TEXTURE_2D, cube[i]);
      char buf[100];
      sprintf(buf, "TexImage%d.bmp", i);
      buf[strlen(buf)] = 0;
      unsigned char* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
      glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   }
}

void draw_cylinder() {
   glRotatef(-90, 1, 0, 0); // 깡통 세우기
   glTranslatef(0, 0, -1);    // 다른 object랑 y축으로 맞추기
   glColor3f(1.0, 1.0, 1.0);

   // top view
   glPushMatrix();
   glTranslatef(0, 0, 2);
   glBindTexture(GL_TEXTURE_2D, cylinder[0]);
   gluPartialDisk(m_pQuadric, 0, 0.7, 30, 3, 0, 360);
   // radius, height, slices, stacks
   glPopMatrix();

   // middle
   glBindTexture(GL_TEXTURE_2D, cylinder[1]);
   gluCylinder(m_pQuadric, 0.7, 0.7, 2, 20, 20);
   // radius, height, slices, stacks

   // bottom view
   glPushMatrix();
   glRotatef(180, 1, 0, 0); // normal vector 설정과 같음
   glBindTexture(GL_TEXTURE_2D, cylinder[2]);
   gluPartialDisk(m_pQuadric, 0, 0.7, 30, 3, 0, 360);
   glPopMatrix();
}

void cylinder_TextureMapping() {
   glGenTextures(3, cylinder);

   glBindTexture(GL_TEXTURE_2D, cylinder[0]);
   int width, height, channels;
   uchar* img = readImageData("CIDER_T.bmp", &width, &height, &channels);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glBindTexture(GL_TEXTURE_2D, cylinder[1]);
   img = readImageData("CIDER_S.bmp", &width, &height, &channels);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glBindTexture(GL_TEXTURE_2D, cylinder[2]);
   img = readImageData("CIDER_B.bmp", &width, &height, &channels);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void draw_sphere() {
   glPushMatrix();
   glRotatef(-90, 1, 0, 0);
   glColor3f(1, 1, 1);

   glBindTexture(GL_TEXTURE_2D, *sphere);
   gluSphere(m_pQuadric, 0.7, 100, 100);
   // radius, slices, stacks
   glPopMatrix();
}

void sphere_TextureMapping() {
   glGenTextures(1, sphere);
   uchar* img;
   int width, height, channels;

   glBindTexture(GL_TEXTURE_2D, *sphere);
   img = readImageData("EARTH.bmp", &width, &height, &channels);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
   glPushAttrib(GL_LIGHTING_BIT);
   //glDisable(GL_LIGHTING);
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(-10, 10, -10, 10);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glColor3f(red, green, blue);
   glRasterPos3f(x_position, y_position, 0);
   for (unsigned int i = 0; i < strlen(str); i++) {
      glutBitmapCharacter(font, str[i]);
   }
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopAttrib();
}

void draw_axis(void) {
   glLineWidth(3); // 좌표축 두께
   glBegin(GL_LINES);
   glColor3f(1, 0, 0); // x축
   glVertex3f(0, 0, 0);
   glVertex3f(4, 0, 0);

   glColor3f(0, 1, 0); // y축
   glVertex3f(0, 0, 0);
   glVertex3f(0, 4, 0);

   glColor3f(0, 0, 1); // z축
   glVertex3f(0, 0, 0);
   glVertex3f(0, 0, 4);
   glEnd();
   glLineWidth(1);
}

void draw(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 카메라 좌표계
   cameraX = radius*sin(theta*PI / 180)*cos(phi*PI / 180);
   cameraY = radius*sin(phi*PI / 180);
   cameraZ = radius*cos(theta*PI / 180)*cos(phi*PI / 180);
   upY = cos(phi*PI / 180);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);
   draw_axis();

   // Cube
   glPushMatrix();
   draw_cube();
   glPopMatrix();

   // Cylinder
   glPushMatrix();
   glTranslatef(3, 0, 0);
   draw_cylinder();
   glPopMatrix();

   // Sphere
   glPushMatrix();
   glTranslatef(-5, -5, 0);
   draw_sphere();
   glPopMatrix();

   draw_string(GLUT_BITMAP_HELVETICA_18, "12171788 Park Jieun", -9, 9, 0, 0, 0);

   glutSwapBuffers();
   // back buffer를 front buffer로 변경
}
