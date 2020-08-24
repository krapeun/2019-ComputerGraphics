// Lab10 12171788 박지은

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

int draw_mode = 0;

void init(void) {
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Light 설정
   GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);

   GLfloat ambient_Sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat diffuse_Sun[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };

   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
   glMaterialf(GL_FRONT, GL_SHININESS, 64);

   // 물체의 컬러 속성 설정
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // Depth-Test Enable
   glFrontFace(GL_CW);
   glEnable(GL_DEPTH_TEST);
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

   angle_moon1 = angle_moon1 - 2.5;
   if (angle_moon1 > 360) angle_moon1 -= 360;
   angle_moon2 = angle_moon2 + 0.5;
   if (angle_moon2 > 360) angle_moon2 -= 360;

   angle_sun = angle_sun + 0.1;
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

void keyboard(unsigned char key, int x, int y) {
   printf("You pressed %c\n", key);
   switch (key) {
   case '0' : 
      draw_mode = 0;
      glEnable(GL_LIGHT0);
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
      glDisable(GL_LIGHT3);
         break;
   case '1': 
      draw_mode = 1;
      glEnable(GL_LIGHT1);
      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHT2);
      glDisable(GL_LIGHT3);
         break;
   case '2': 
      draw_mode = 2;
      glEnable(GL_LIGHT2);
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHT3);
         break;
   case '3': 
      draw_mode = 3;
      glEnable(GL_LIGHT3);
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
      glDisable(GL_LIGHT0);
         break;
   }
   glutPostRedisplay();
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
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // 조명이 카메라 위치
   GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   GLfloat emission0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   if (draw_mode == 0) {
      glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
   }

   if (draw_mode == 0) {
      emission0[0] = 0.0f;
      glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
   }

   gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
   // Viewing Transformation
   glutIdleFunc(idle);

   // 태양
   glRotatef(angle_sun, 0, 1, 0);
   draw_axis();
   glColor3f(1, 0, 0);

   // 조명이 태양에 위치
   glLightfv(GL_LIGHT1, GL_POSITION, light_position);

   GLfloat emission1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   if (draw_mode == 1) {
      glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
   }
   glutSolidSphere(2.5, 50, 50);
   if (draw_mode == 1) {
      emission1[0] = 0.0f;
      glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
   }

   glPushMatrix(); // 행렬을 스택에 저장

   // 지구
   glRotatef(angle_earth1, 0, 1, 0);
   glTranslatef(5, 0, 0);
   glRotatef(angle_earth2, 0, 1, 0);
   // Modeling Transformation
   glColor3f(0, 0, 1);

   // 조명이 지구에 위치
   glLightfv(GL_LIGHT2, GL_POSITION, light_position);

   GLfloat emission2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   if (draw_mode == 2) {
      glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
   }
   glutSolidSphere(1, 50, 50);
   if (draw_mode == 2) {
      emission2[0] = 0.0f;
      glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
   }

   // 달
   glRotatef(angle_moon1, 0, 1, 0);
   glTranslatef(2, 0, 0);
   glRotatef(angle_moon2, 0, 1, 0);

   glColor3f(0.5, 0.5, 0);

   // 조명이 달에 위치
   glLightfv(GL_LIGHT3, GL_POSITION, light_position);

   GLfloat emission3[] = { 1.0f, 0.0f, 0.0f, 1.0f };
   if (draw_mode == 3) {
      glMaterialfv(GL_FRONT, GL_EMISSION, emission3);
   }
   glutSolidSphere(0.5, 50, 50);
   if (draw_mode == 3) {
      emission3[0] = 0.0f;
      glMaterialfv(GL_FRONT, GL_EMISSION, emission3);
   }

   glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀
   glPushMatrix(); // 현재 행렬을 스택에 다시 저장

   // 화성
   glRotatef(angle_mars1, 0, 1, 0);
   glTranslatef(0, 0, 5);
   glRotatef(angle_mars2, 0, 1, 0);

   glColor3f(0, 1, 1);
   glutSolidSphere(1, 50, 50);

   glutSwapBuffers();
   // back buffer를 front buffer로 변경
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glClearDepth(1.0f);
   // double buffer로 변경
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("12171788 박지은");
   init();

   glutDisplayFunc(draw); // draw 콜백함수
   glutReshapeFunc(&resize); // resize 콜백함수
   glutKeyboardFunc(keyboard);
   //glutPostRedisplay();

   glutMainLoop();

   return 0;
}
