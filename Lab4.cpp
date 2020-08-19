// Lab4 Hw1 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>

void init(void) {
   glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_triangle(void) {
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_POLYGON_SMOOTH);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

   glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
   glBegin(GL_TRIANGLES);

   glVertex2i(250, 250);
   glVertex2i(150, 300);
   glVertex2i(300, 400);

   glColor4f(0.0f, 1.0f, 1.0f, 0.75f);
   glBegin(GL_TRIANGLES);

   glVertex2i(200, 150);
   glVertex2i(350, 200);
   glVertex2i(250, 300);

   glEnd();
}

void draw(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   draw_triangle();
   glFlush();
}

int main(int argc, char** argv) {
   void glBlendFunc(GLenum sfactor, GLenum dfactor);
   void glHint(GLenum target, GLenum mode);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(300, 300);
   glutCreateWindow("12171788 박지은");
   init();

   glutDisplayFunc(draw);

   glutMainLoop();

   return 0;
}
