// Lab4 HW3 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>
#include <string.h>

void init(void) {
   glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_triangle(void) {
   glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
   glBegin(GL_TRIANGLES);

   glVertex2i(250, 250);
   glVertex2i(150, 300);
   glVertex2i(350, 400);

   glEnd();
}

void draw_string(void * font, const char* str, int x, int y) {
   unsigned int i;
   glRasterPos2i(x, y);
   for (i = 0; i < strlen(str); i++) {
      glutBitmapCharacter(font, str[i]);
   }
}

void draw(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   draw_triangle();
   glColor3f(1.0f, 1.0f, 0);
   draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "Graphics are cool!", 10, 10);
   glColor3f(0.5f, 1.0f, 0);
   draw_string(GLUT_BITMAP_HELVETICA_12, "I Like Computer Graphics!!", 250, 475);
   glFlush();
}

int main(int argc, char** argv) {
   void glBlendFunc(GLenum sfactor, GLenum dfactor);
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
