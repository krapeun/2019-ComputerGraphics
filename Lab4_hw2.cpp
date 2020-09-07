// Lab4 HW2 12171788 박지은

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

void draw_point(void) {
   glColor3f(1.0f, 0.0f, 0.0f);
   glPointSize(20.0f);
   glBegin(GL_POINTS);

   glVertex2i(100, 300);

   glEnd();
}

void draw_line(void) {
   glColor3f(1.0f, 1.0f, 0.0f);
   glBegin(GL_LINES);

   glVertex2i(50, 50);
   glVertex2i(250, 450);
   
   glEnd();
}

void draw_triangle(void) {
   glColor3f(1.0f, 1.0f, 0.0f);
   glBegin(GL_TRIANGLES);

   glVertex2i(350, 250);
   glVertex2i(400, 300);
   glVertex2i(100, 400);

   glEnd();
}

void draw(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   //draw_point();
   //draw_line();
   draw_triangle();
   glFlush();
}

void main_menu_function(int option) {
   printf("Main menu %d has been selected\n", option);
   if (option == 999) exit(0);
   else if (option == 1) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   }
   else if (option == 2) {
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
   }
   glutPostRedisplay();
}

int main(int argc, char** argv) {
   void glBlendFunc(GLenum sfactor, GLenum dfactor);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(300, 300);
   glutCreateWindow("12171788 박지은");
   init();

   glutCreateMenu(main_menu_function);
   glutAddMenuEntry("Quit", 999);
   glutAddMenuEntry("Antialiasing ON", 1);
   glutAddMenuEntry("Antialiasing OFF", 2);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   glutDisplayFunc(draw);

   glutMainLoop();

   return 0;
}
