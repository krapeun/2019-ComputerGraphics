// Lab8 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>

float m_RotateAngle = 0;

void init(void) {
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Light 설정
   GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
   GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
   glMateriali(GL_FRONT, GL_SHININESS, 20);

   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // Depth-Test Enable
   glFrontFace(GL_CW); // Teapot은 CW로 정의됨
   glPolygonMode(GL_BACK, GL_LINE);
   glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);
   glEnable(GL_DEPTH_TEST);
}

void idle(void) {
   m_RotateAngle = m_RotateAngle + 0.1; // 공전 속도
   if (m_RotateAngle > 360) m_RotateAngle -= 360;

   glutPostRedisplay();
}

void resize(int width, int height) {
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, (double)width / (double)height, 10, 500);
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
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
   glutIdleFunc(idle);
   draw_axis();

   glPushMatrix();
   //glRotatef(m_RotateAngle, 0, 1, 0);
   GLdouble eq[4] = { 0.0, 0.0, 1.0, 0.0 };
   glClipPlane(GL_CLIP_PLANE0, eq);
   glEnable(GL_CLIP_PLANE0);
   glPopMatrix();

   glColor3f(1, 1, 0);
   glutSolidTeapot(3);

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

   glutDisplayFunc(draw);
   glutReshapeFunc(&resize);

   glutMainLoop();

   return 0;
}
