// 컴퓨터그래픽스설계 12171788 박지은

#include <gl/glut.h>
#include <stdio.h>

void init(void) {
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);	// Blue

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_point(void) {	// Drawing point
	glColor3f(1.0f, 0.0f, 0.0f);	// Red
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	glVertex2i(5, 5);
	glVertex2i(200, 200);
	glVertex2i(450, 300);
	glEnd();
}

void draw_line(void) {	// Drawing line
	void glLineStipple(GLint factor, GLushort pattern);
	glLineWidth(2.0f);
	glEnable(GL_LINE_STIPPLE);	// State ON
	glLineStipple(3, 0xAAAA);

	glColor3f(1.0f, 1.0f, 0.0f);	// Yellow
	glBegin(GL_LINES);

	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);
	glEnd();
}

void draw_triangle(void) {	// Drawing triangle
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2i(50, 50);
	glVertex2i(250, 250);
	glVertex2i(400, 100);
	glVertex2i(100, 150);
	glVertex2i(150, 300);
	glVertex2i(300, 400);
	glEnd();
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	//draw_point();
	//draw_line();
	draw_triangle();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init();

	glutDisplayFunc(draw);

	glutMainLoop();

	return 0;
}