// Lab5 Transformation 12171788 박지은

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>

int draw_mode = 0; // 그릴 도형

void init(void) {
   glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void main_menu_function(int option) {
   printf("Main menu %d has been selected\n", option);
   if (option == 999) exit(0);
   else if (option == 1) draw_mode = 1; // draw_point
   else if (option == 2) draw_mode = 2; // draw_line
   else if (option == 3) draw_mode = 3; // draw_triangle
   glutPostRedisplay();
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

   glVertex2i(250, 250);
   glVertex2i(150, 300);
   glVertex2i(300, 400);

   glEnd();
}

void draw(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glTranslatef(250, 250, 0);
   glRotatef(60, 0, 0, 1);
   glTranslatef(-250, -250, 0);
   glScalef(0.5, 0.5, 0.5);

   switch (draw_mode) {
   case 1:
      draw_point();
      break;
   case 2:
      draw_line();
      break;
   case 3:
      draw_triangle();
      break;
   }
   glFlush();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(300, 300);
   glutCreateWindow("12171788 박지은");
   init();

   glutCreateMenu(main_menu_function);
   glutAddMenuEntry("Quit", 999);
   glutAddMenuEntry("Point", 1);
   glutAddMenuEntry("Line", 2);
   glutAddMenuEntry("Triangle", 3);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   glutDisplayFunc(draw);

   glutMainLoop();

   return 0;
}
