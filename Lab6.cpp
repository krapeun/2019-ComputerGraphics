// Lab6 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>

void init(void) {
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
   glClear(GL_COLOR_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, -20, -10, 0, 0, 0, 0, 1, 0);

   glColor3f(1, 1, 0);
   glutWireTeapot(4);
   draw_axis();

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

   glutDisplayFunc(draw);
   glutReshapeFunc(&resize);

   glutMainLoop();

   return 0;
}
