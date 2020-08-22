// Lab7 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>

float angle_sun = 0;
float angle_earth1 = 0; // 공전
float angle_earth2 = 0; // 자전
float angle_mars1 = 0;
float angle_mars2 = 0;
float angle_moon1 = 0;
float angle_moon2 = 0;
float angle_jupiter1 = 0;
float angle_jupiter2 = 0;
float angle_mercury1 = 0;
float angle_mercury2 = 0;
float angle_venus1 = 0;
float angle_venus2 = 0;

void init(void) {
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void idle(void) {
   angle_earth1 = angle_earth1 + 0.15; // 공전 속도
   if (angle_earth1 > 360) angle_earth1 -= 360;
   angle_earth2 = angle_earth2 + 3; // 자전 속도
   if (angle_earth2 > 360) angle_earth2 -= 360;

   angle_mars1 = angle_mars1 + 0.2;
   if (angle_mars1 > 360) angle_mars1 -= 360;
   angle_mars2 = angle_mars2 + 6;
   if (angle_mars2 > 360) angle_mars2 -= 360;

   angle_moon1 = angle_moon1 -2.5;
   if (angle_moon1 > 360) angle_moon1 -= 360;
   angle_moon2 = angle_moon2 + 0.5;
   if (angle_moon2 > 360) angle_moon2 -= 360;

   angle_jupiter1 = angle_jupiter1 + 0.1;
   if (angle_jupiter1 > 360) angle_jupiter1 -= 360;
   angle_jupiter2 = angle_jupiter2 + 6;
   if (angle_jupiter2 > 360) angle_jupiter2 -= 360;

   angle_mercury1 = angle_mercury1 + 0.6;
   if (angle_mercury1 > 360) angle_mercury1 -= 360;
   angle_mercury2 = angle_mercury2 + 6;
   if (angle_mercury2 > 360) angle_mercury2 -= 360;

   angle_venus1 = angle_venus1 + 0.3;
   if (angle_venus1 > 360) angle_venus1 -= 360;
   angle_venus2 = angle_venus2 + 6;
   if (angle_venus2 > 360) angle_venus2 -= 360;

   angle_sun = angle_sun + 0.2;
   if (angle_sun > 360) angle_sun -= 360;

   glutPostRedisplay();
}

void resize(int width, int height) {
   glViewport(0, 0, width, height); // Viewport Transformation
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, (double)width / (double)height, 10, 500);
   // Projection Transformation
   glMatrixMode(GL_MODELVIEW);
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
   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
   // Viewing Transformation
   glutIdleFunc(idle);

   // 태양
   glRotatef(angle_sun, 0, 1, 0);
   glColor3f(1, 0, 0);
   glutWireSphere(2.5, 50, 50); // radius, slice, stack
   draw_axis();

   glPushMatrix(); // 행렬을 스택에 저장

    // 지구
   glRotatef(angle_earth1, 0, 1, 0);
   glTranslatef(5, 0, 0);
   glRotatef(angle_earth2, 0, 1, 0);
   // Modeling Transformation

   glColor3f(0, 0, 1);
   glutWireSphere(1, 50, 50);

   // 달
   glRotatef(angle_moon1, 0, 1, 0);
   glTranslatef(2, 0, 0);
   glRotatef(angle_moon2, 0, 1, 0);

   glColor3f(0.5, 0.5, 0);
   glutWireSphere(0.5, 50, 50);

   glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀
   glPushMatrix(); // 현재 행렬을 스택에 다시 저장

   // 화성
   glRotatef(angle_mars1, 0, 1, 0);
   glTranslatef(5, 0, 0);
   glRotatef(angle_mars2, 0, 1, 0);

   glColor3f(0, 1, 1);
   glutWireSphere(1, 50, 50);

   glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀
   glPushMatrix();

   // 목성
   glRotatef(angle_jupiter1, 0, 1, 0);
   glTranslatef(6, 0, 0);
   glRotatef(angle_jupiter2, 0, 1, 0);

   glColor3f(0.9, 0.9, 0);
   glutWireSphere(1.7, 50, 50);

   glPopMatrix();
   glPushMatrix();

   // 수성
   glRotatef(angle_mercury1, 0, 1, 0);
   glTranslatef(4, 0, 0);
   glRotatef(angle_mercury2, 0, 1, 0);

   glColor3f(0.3, 0.3, 0.3);
   glutWireSphere(0.6, 50, 50);

   glPopMatrix();
   glPushMatrix();

   // 금성
   glRotatef(angle_venus1, 0, 1, 0);
   glTranslatef(5, 0, 0);
   glRotatef(angle_venus2, 0, 1, 0);

   glColor3f(0.6, 0.4, 0);
   glutWireSphere(0.9, 50, 50);
   glutSwapBuffers();
   // back buffer를 front buffer로 변경
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   // double buffer로 변경
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("12171788 박지은");
   init();

   glutDisplayFunc(draw); // draw 콜백함수
   glutReshapeFunc(&resize); // resize 콜백함수

   glutMainLoop();

   return 0;
}
