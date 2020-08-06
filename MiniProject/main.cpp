#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <GL/freeglut.h>
#include <string.h>
#include <iostream>
using namespace std;

#include "bmpfuncs.h"
#include "ObjParser.h"
#include "GL/glext.h"
#include "scoresum.h"

#include <windows.h>
#include <mmsystem.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.1415926537

// Multiple Viewport
int current_width = WINDOW_WIDTH;
int current_height = WINDOW_HEIGHT;

// 회전 각도
float m_RotateAngle = 0;
float RotateAngle = 0;

// 레고 블럭 종류
int draw_mode = 0;

// 레고 블럭 색상 변경
int color_mode = 0;
float colorR, colorG, colorB;

// 레고 블럭 위치 좌표
float locationX, locationY, locationZ;

// 레고 블럭 회전 각도
float angle;
float angleX, angleY, angleZ;

// 레고 블럭 크기
float scaleX, scaleY, scaleZ;

// object var 전역 변수 생성
ObjParser *lego1, *lego2, *lego3, *lego4, *lego5, *lego6, *lego7, *lego8, *lego9, *lego0;

// Draw Mode
int mode = 0;

// 애니메이션 ON / OFF
bool isIdle = false;

// 구면 좌표계 인자
float theta = 45; // 위도
float phi = 45; // 경도
int radius = 40;
double upY = cos(phi*PI / 180); // up-vector

// 카메라 좌표
float cameraX = 10, cameraY = 10, cameraZ = 10;

// 마우스 현재 위치
int mouseX = 0, mouseY = 0;

// Environment mapping set variable
GLuint background_texture;
GLuint g_nCubeTex, g_nCubeTex1, g_nCubeTex2, g_nCubeTex3, g_nCubeTex4, g_nCubeTex5, g_nCubeTex6;

// quadric object 생성
GLUquadricObj* qobj = gluNewQuadric();

// Keyboard & Mouse Function
void mouse(int button, int state, int x, int y);
void mousewheel(int but, int dir, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

// Menu
void sub_menu_function(int option);
void main_menu_function(int option);

void light(void);
void init(void);
void idle(void);
void resize(int width, int height);
void camera_default(void); // 카메라 시점 초기화
void print(void); // Keyboard Navigation

// 레고 블럭 함수
void lego_save(void); // 생성
void lego_delete(void); // 제거
void lego_init(void); // 초기화
void default_lego(void); // 레고를 새로 그릴 때마다 호출
void load_lego(int i); // Draw Mode
void draw_lego(void); // Struct 인자 그리기
void show_lego(void); // 화면에 띄우기

// Skybox
void draw_skybox();
void environmentMapSetting1(void);
void environmentMapSetting2(void);
void environmentMapSetting3(void);
void texture_off(void);

// File 자체 저장 포맷
void file_write(char *filename);
void file_read(char *filename);

void draw_axis(void); // 좌표축
void preview(void); // Preview
void draw(void);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearDepth(1.0f); // double buffer로 변경

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("12171788 박지은 Mini Project");
	init();

	// quadric object 속성 설정
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	// Menu 선택
	int submenu_level, submenu_write, submenu_read;

	submenu_level = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Level 1", 3);
	glutAddMenuEntry("Level 2", 4);
	glutAddMenuEntry("Level 3", 5);

	submenu_write = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Write 1", 11);
	glutAddMenuEntry("Write 2", 12);
	glutAddMenuEntry("Write 3", 13);

	submenu_read = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Read 1", 21);
	glutAddMenuEntry("Read 2", 22);
	glutAddMenuEntry("Read 3", 23);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Menual", 1);
	glutAddMenuEntry("Making Hamburger", 2);
	glutAddSubMenu("Game Start", submenu_level);
	glutAddSubMenu("Write", submenu_write);
	glutAddSubMenu("Read", submenu_read);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(draw); // draw 콜백 함수
	glutReshapeFunc(&resize); // resize 콜백 함수
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mousewheel);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);

	glutMainLoop();

	return 0;
}

void light(void) {
	// Light 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);

	// Material 설정
	GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat material_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// 물체의 Color 속성 설정
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void init(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// 리소스 로드
	lego1 = new ObjParser("lego1.obj");
	lego2 = new ObjParser("lego2.obj");
	lego3 = new ObjParser("lego3.obj");
	lego4 = new ObjParser("lego4.obj");
	lego5 = new ObjParser("lego5.obj");
	lego6 = new ObjParser("lego6.obj");
	lego7 = new ObjParser("lego7.obj");
	lego8 = new ObjParser("lego8.obj");
	lego9 = new ObjParser("lego9.obj");
	lego0 = new ObjParser("lego0.obj");

	// Default Object Color Set
	glColor3f(1.0f, 1.0f, 1.0f);

	// resize callback
	glutReshapeFunc(resize);

	// Light 설정
	light();

	// Menual Print
	print();

	// TEXTURE MAPPING SET
	gluQuadricTexture(qobj, GL_TRUE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	// Environment Mapping
	environmentMapSetting1();
	environmentMapSetting2();
	environmentMapSetting3();

	// Depth-Test Enable
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
}

void idle(void) {
	if (isIdle == true) {
		theta = theta - 0.1;
		if (theta <= -360) theta = 0;
	}

	glutPostRedisplay();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	current_width = width;
	current_height = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)width / (double)height, 1, 500);
	// Projection Transformation
	glMatrixMode(GL_MODELVIEW);
}

void camera_default() {
	radius = 40;
	//radius = 30;
	theta = 45.0;
	phi = 45.0;
}

void print(void) {
	// Menual
	switch (mode) {
	case 0:
		printf("\n----------Keyboard Navigation----------\n");
		printf("마우스 오른쪽 : 메뉴 선택\n");
		printf("---------------------------------------\n");
		break;
	case 2:
		printf("\n----------Keyboard Navigation----------\n");
		printf("마우스 왼쪽, 휠 : 카메라 시점 이동\n");
		printf("키보드 q : 레고 블럭 색깔 선택\n");
		printf("키보드 숫자 키 : 레고 블럭 종류 선택\n");
		printf("키보드 방향키, <, > : 레고 블럭 위치 이동\n");
		printf("키보드 a, d : 레고 블럭 회전 이동\n");
		printf("키보드 +, - : 레고 블럭 크기 변경\n");
		printf("키보드 s : 레고 블럭 변환 초기화\n"); 
		printf("키보드 x : 전체 삽입 초기화\n");
		printf("키보드 o : 레고 블럭 삽입\n");
		printf("키보드 Esc : 레고 블럭 삽입 취소\n");
		printf("---------------------------------------\n");
		break;
	case 3:
		printf("\n-------------Level 1-------------\n");
		printf("TIP 1 : Fish has a minus score\n");
		printf("TIP 2 : You have to match correct lego blocks\n");
		printf("마우스 왼쪽, 휠 : 카메라 시점 이동\n");
		printf("키보드 q : 레고 블럭 색깔 선택\n");
		printf("키보드 숫자 키 : 레고 블럭 종류 선택\n");
		printf("키보드 방향키, <, > : 레고 블럭 위치 이동\n");
		printf("키보드 a, d : 레고 블럭 회전 이동\n");
		printf("키보드 +, - : 레고 블럭 크기 변경\n");
		printf("키보드 o : 레고 블럭 삽입\n");
		printf("---------------------------------------\n");
		break;
	case 4:
		printf("\n-------------Level 2-------------\n");
		printf("TIP 1 : Fish has a minus score\n");
		printf("TIP 2 : You have to match correct lego blocks\n");
		printf("마우스 왼쪽, 휠 : 카메라 시점 이동\n");
		printf("키보드 q : 레고 블럭 색깔 선택\n");
		printf("키보드 숫자 키 : 레고 블럭 종류 선택\n");
		printf("키보드 방향키, <, > : 레고 블럭 위치 이동\n");
		printf("키보드 a, d : 레고 블럭 회전 이동\n");
		printf("키보드 +, - : 레고 블럭 크기 변경\n");
		printf("키보드 o : 레고 블럭 삽입\n");
		printf("---------------------------------------\n");
		break;
	case 5:
		printf("\n-------------Level 3-------------\n");
		printf("TIP 1 : Fish has a minus score\n");
		printf("TIP 2 : You have to match correct lego blocks\n");
		printf("마우스 왼쪽, 휠 : 카메라 시점 이동\n");
		printf("키보드 q : 레고 블럭 색깔 선택\n");
		printf("키보드 숫자 키 : 레고 블럭 종류 선택\n");
		printf("키보드 방향키, <, > : 레고 블럭 위치 이동\n");
		printf("키보드 a, d : 레고 블럭 회전 이동\n");
		printf("키보드 +, - : 레고 블럭 크기 변경\n");
		printf("키보드 o : 레고 블럭 삽입\n");
		printf("---------------------------------------\n");
		break;
	}
}
  
void mouse(int button, int state, int x, int y) {
	mouseX = x;
	mouseY = y;

	glutPostRedisplay();
}

void mousewheel(int but, int dir, int x, int y) {
	if (dir > 0) { // 위로
		if (radius < 6) radius = 6; // 시점 제한
		if (radius > 2) radius--;
	}
	else { // 아래로
		if (radius > 100) radius = 100; // 시점 제한
		if (radius < 180) radius++;
	}
	glutPostRedisplay();
}

void motion(int x, int y) {
	if (x > mouseX) {
		theta = theta - 1;
		if (theta <= -360) theta = 0;
	}
	else if (x < mouseX) {
		theta = theta + 1;
		if (theta >= 360) theta = 0;
	}

	if (y < mouseY) {
		if (phi < 5) phi = 5;
		phi = phi - 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
	}
	else if (y > mouseY) {
		if (phi > 80) phi = 80;
		phi = phi + 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	show_lego();
	switch (key) {
	// 레고의 종류 변경
	case '1':
		draw_mode = 1;
		default_lego();
		break;
	case '2':
		draw_mode = 2;
		default_lego();
		break;
	case '3':
		draw_mode = 3;
		default_lego();
		break;          
	case '4':
		draw_mode = 4;
		default_lego();
		break;
	case '5':
		draw_mode = 5;
		default_lego();
		break;
	case '6':
		draw_mode = 6;
		default_lego();
		break;
	case '7':
		draw_mode = 7;
		default_lego();
		break;
	case '8':
		draw_mode = 8;
		default_lego();
		break;
	case '9':
		draw_mode = 9;
		default_lego();
		break;
	case '0':
		draw_mode = 0;
		default_lego();
		break;
	// 레고의 색 변경
	case 'q':
		color_mode = color_mode + 1;
		if (color_mode == 10) color_mode = color_mode - 10;
		break;
	// 레고의 회전
	case 'a': // 좌
		angleX = 0;
		angleY = 1;
		angleZ = 0;
		angle = angle + 2;
		if (angle > 360) angle = angle - 360;
		break;
	case 'd': // 우
		angleX = 0;
		angleY = 1;
		angleZ = 0;
		angle = angle - 2;
		if (angle > 360) angle = angle - 360;
		break;
	// 레고 위치, 색깔 초기화
	case 's':
		default_lego();
		break;
	// 레고의 초기화
	case 'x':
		if (lego_number != 0) {
			lego_init();
			sndPlaySound(TEXT("pung.wav"), SND_ASYNC | SND_NODEFAULT);
		}
		else printf("No lego!\n");
		break;
	// 레고의 이동
	case '>': // 뒤
		if (locationZ < -50) locationZ = -50;
		locationZ = locationZ - 1;
		break;
	case '<': // 앞
		if (locationZ > 50) locationZ = 50;
		locationZ = locationZ + 1;
		break;
	// 레고의 크기 변화
	case '+': // 확대
		scaleX = scaleX + 0.1;
		scaleY = scaleY + 0.1;
		scaleZ = scaleZ + 0.1;
		// 크기 제한
		if (scaleX > 2) scaleX = 2;
		if (scaleY > 2) scaleY = 2;
		if (scaleZ > 2) scaleZ = 2;
		break;
	case '-': // 축소
		scaleX = scaleX - 0.1;
		scaleY = scaleY - 0.1;
		scaleZ = scaleZ - 0.1;
		// 크기 제한
		if (scaleX < 0.5) scaleX = 0.5;
		if (scaleY < 0.5) scaleY = 0.5;
		if (scaleZ < 0.5) scaleZ = 0.5;
		break;
	// Preview Mode ON/OFF
	case 'p':
		if (preview_mode == true) {
			preview_mode = false;
			break;
		}
		else {
			preview_mode = true;
			break;
		}
		break;
	// Undo
	case 27: // Esc
		if (mode == 3) printf("You cannot undo!\n");
		else if (mode == 4) printf("You cannot undo!\n");
		else if (mode == 5) printf("You cannot undo!\n");
		else {
			lego_delete();
			sndPlaySound(TEXT("ding.wav"), SND_ASYNC | SND_NODEFAULT);
		}

		break;
	// 레고의 저장
	case 'o':
		switch (mode) {
		case 2: // Making Mode일 때 정상적으로 동작
			lego_save();
			sndPlaySound(TEXT("ring.wav"), SND_ASYNC | SND_NODEFAULT);
			break;
		case 3: // Game Mode일 때 stage clear 시 동작 X
			if (next_step >= 4) {
				printf("Cannot Save!\n");
				next_step = 5;
			}
			else {
				lego_save();
				next_step++;
				sndPlaySound(TEXT("ring.wav"), SND_ASYNC | SND_NODEFAULT);
			}
			level1_score();
			break;
		case 4:
			if (next_step >= 7) {
				printf("Cannot Save!\n");
				next_step = 8;
			}
			else {
				lego_save();
				next_step++;
				sndPlaySound(TEXT("ring.wav"), SND_ASYNC | SND_NODEFAULT);
			}
			level2_score();
			break;
		case 5:
			if (next_step >= 9) {
				printf("Cannot Save!\n");
				next_step = 10;
			}
			else {
				lego_save();
				next_step++;
				sndPlaySound(TEXT("ring.wav"), SND_ASYNC | SND_NODEFAULT);
			}
			level3_score();
			break;
		case 6:
			break;
		}
	}
	glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y) {
	switch (key) {
		// 레고의 이동
	case GLUT_KEY_LEFT: // 좌
		if (locationX < -50) locationX = -50;
		locationX = locationX - 1;
		break;
	case GLUT_KEY_RIGHT: // 우
		if (locationX > 50) locationX = 50;
		locationX = locationX + 1;
		break;
	case GLUT_KEY_DOWN: // 하
		if (locationY < 1) locationY = 1;
		locationY = locationY - 0.5;
		break;
	case GLUT_KEY_UP: // 상
		if (locationY > 20) locationY = 20;
		locationY = locationY + 0.5;
		break;
	}
	glutPostRedisplay();
}

void sub_menu_function(int option) {
	printf("Submenu %d has been selected\n", option);
	switch (option) {
	case 3:
		mode = 3;
		draw_mode = -1; // 화면 초기화
		default_lego();
		next_step = -1; // Level 초기화
		isIdle = false;
		score_sum = 0;
		upY = 1;
		camera_default(); // 카메라 시점 초기화
		lego_init(); // Lego Index 초기화
		print();
		break;
	case 4:
		mode = 4;
		draw_mode = -1; // 화면 초기화
		default_lego();
		next_step = -1; // Level 초기화
		isIdle = false;
		score_sum = 0;
		upY = 1;
		camera_default(); // 카메라 시점 초기화
		lego_init(); // Lego Index 초기화
		print();
		break;
	case 5:
		mode = 5;
		draw_mode = -1; // 화면 초기화
		default_lego();
		next_step = -1; // Level 초기화
		isIdle = false;
		score_sum = 0;
		upY = 1;
		camera_default(); // 카메라 시점 초기화
		lego_init(); // Lego Index 초기화
		print();
		break;
	case 11:
		char *filename;	// 파일명
		filename = "file_1.txt";
		file_write(filename);
		mode = 2;
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Save Success", -4, -5, 0, 0, 0);
		sndPlaySound(TEXT("click.wav"), SND_ASYNC | SND_NODEFAULT);
		break;
	case 12:
		filename = "file_2.txt";
		file_write(filename);
		mode = 2;
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Save Success", -4, -5, 0, 0, 0);
		sndPlaySound(TEXT("click.wav"), SND_ASYNC | SND_NODEFAULT);
		break;
	case 13:
		filename = "file_3.txt";
		file_write(filename);
		mode = 2;
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Save Success", -4, -5, 0, 0, 0);
		sndPlaySound(TEXT("click.wav"), SND_ASYNC | SND_NODEFAULT);
		break;
	case 21:
		filename = "file_1.txt";
		file_read(filename);
		mode = 11;
		break;
	case 22:
		filename = "file_2.txt";
		file_read(filename);
		mode = 11;
		break;
	case 23:
		filename = "file_3.txt";
		file_read(filename);
		mode = 11;
		break;
	}
	glutPostRedisplay();
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	switch (option) {
	case 0: // 첫 화면
		mode = 0;
		break;
	case 1: // How to do
		mode = 1;
		glClearColor(0, 0, 0, 0.0f);
		break;
	case 2: // Making Hamburger
		mode = 2;
		draw_mode = -1; // 화면 초기화
		isIdle = false;
		upY = 1;
		camera_default();
		lego_init();
		break;
	case 999:
		exit(0);
		break;
	}
	// Menual Print
	print();
	glutPostRedisplay();
}

void draw_skybox() {
	glClearColor(1, 1, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);

	switch (mode) {
	case 0:
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex1);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex1);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex1);
		break;
	case 4:
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex2);
		break;
	case 5:
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex3);
		break;
	}

	float g_nSkySize = 100.0f;

	glBegin(GL_QUADS);
	//px
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, 0, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, -g_nSkySize);

	//nx
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, 0, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, -g_nSkySize);

	//py
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, -g_nSkySize);

	//ny
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, 0, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, 0, -g_nSkySize);

	//pz
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, 0, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, g_nSkySize);

	//nz
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, 0, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, 0, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, 2*g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, 2*g_nSkySize, -g_nSkySize);

	glEnd();
}

void environmentMapSetting1(void) {
	glGenTextures(1, &g_nCubeTex1);
	int width, height, channels;
	uchar* img0, *img1, *img2, *img3, *img4, *img5;

	img0 = readImageData("brick_brown_1.bmp", &width, &height, &channels);
	img1 = readImageData("brick_brown_3.bmp", &width, &height, &channels);
	img2 = readImageData("brick_brown_top.bmp", &width, &height, &channels);
	img3 = readImageData("brick_brown_top.bmp", &width, &height, &channels);
	img4 = readImageData("brick_brown_1.bmp", &width, &height, &channels);
	img5 = readImageData("brick_brown_2.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
}

void environmentMapSetting2(void) {
	glGenTextures(1, &g_nCubeTex2);
	int width, height, channels;
	uchar* img0, *img1, *img2, *img3, *img4, *img5;

	img0 = readImageData("brick_blue_1.bmp", &width, &height, &channels);
	img1 = readImageData("brick_blue_3.bmp", &width, &height, &channels);
	img2 = readImageData("brick_blue_top.bmp", &width, &height, &channels);
	img3 = readImageData("brick_blue_top.bmp", &width, &height, &channels);
	img4 = readImageData("brick_blue_1.bmp", &width, &height, &channels);
	img5 = readImageData("brick_blue_2.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex2);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
}

void environmentMapSetting3(void) {
	glGenTextures(1, &g_nCubeTex3);
	int width, height, channels;
	uchar* img0, *img1, *img2, *img3, *img4, *img5;

	img0 = readImageData("brick_green_1.bmp", &width, &height, &channels);
	img1 = readImageData("brick_green_3.bmp", &width, &height, &channels);
	img2 = readImageData("brick_green_top.bmp", &width, &height, &channels);
	img3 = readImageData("brick_green_top.bmp", &width, &height, &channels);
	img4 = readImageData("brick_green_1.bmp", &width, &height, &channels);
	img5 = readImageData("brick_green_2.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex3);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
}

void texture_off(void) {
	// SkyBox와 반대로
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void file_write(char *filename) {
	printf("Save File!\n");
	FILE *fp; // 읽어온 파일 저장
	fp = fopen(filename, "w+"); // w+ 형식 사용

	// File Save
	fprintf(fp, "%d ", lego_number);
	for (int i = 0; i < lego_number; i++) {
		fprintf(fp, "%d ", legos[i].id);
		fprintf(fp, "%d ", legos[i].lego_color);
		fprintf(fp, "%f %f %f ", legos[i].lego_R, legos[i].lego_G, legos[i].lego_B);
		fprintf(fp, "%f %f %f ", legos[i].lego_X, legos[i].lego_Y, legos[i].lego_Z);
		fprintf(fp, "%f ", legos[i].lego_angle);
		fprintf(fp, "%f %f %f ", legos[i].lego_angleX, legos[i].lego_angleY, legos[i].lego_angleZ);
		fprintf(fp, "%f %f %f ", legos[i].lego_scaleX, legos[i].lego_scaleY, legos[i].lego_scaleZ);
	}
	fclose(fp);
}

void file_read(char *filename) {
	printf("Load File!\n");
	FILE *fp; // 저장한 파일 읽기
	fp = fopen(filename, "r+"); // r+ 형식 사용

	// File Load
	fscanf(fp, "%d ", &lego_number);
	for (int i = 0; i < lego_number; i++) {
		fscanf(fp, "%d ", &legos[i].id);
		fscanf(fp, "%d ", &legos[i].lego_color);
		fscanf(fp, "%f %f %f ", &legos[i].lego_R, &legos[i].lego_G, &legos[i].lego_B);
		fscanf(fp, "%f %f %f ", &legos[i].lego_X, &legos[i].lego_Y, &legos[i].lego_Z);
		fscanf(fp, "%f ", &legos[i].lego_angle);
		fscanf(fp, "%f %f %f ", &legos[i].lego_angleX, &legos[i].lego_angleY, &legos[i].lego_angleZ);
		fscanf(fp, "%f %f %f ", &legos[i].lego_scaleX, &legos[i].lego_scaleY, &legos[i].lego_scaleZ);
	}
	fclose(fp);
}

void lego_save(void) {
	for (int i = 0; i < 100; i++) {
		if (draw_mode == -1) {
			printf("No Lego!\n");
			break;
		}
		else if (legos[i].id == -1) {
			printf("Save Success!\n");
			lego_number++;

			legos[i].id = draw_mode;

			// 레고 색
			legos[i].lego_color = color_mode;
			legos[i].lego_R = colorR;
			legos[i].lego_G = colorG;
			legos[i].lego_B = colorB;

			// 레고 위치 좌표
			legos[i].lego_X = locationX;
			legos[i].lego_Y = locationY;
			legos[i].lego_Z = locationZ;

			// 레고 회전 각도
			legos[i].lego_angle = angle;
			legos[i].lego_angleX = angleX;
			legos[i].lego_angleY = angleY;
			legos[i].lego_angleZ = angleZ;

			// 레고 크기
			legos[i].lego_scaleX = scaleX;
			legos[i].lego_scaleY = scaleY;
			legos[i].lego_scaleZ = scaleZ;

			break;
		}
	}
	default_lego();
}

void lego_delete(void) {
	for (int i = 0; i < 100; i++) {
		if (lego_number == 0) printf("No Lego!\n");
		if (legos[i].id == -1) {
			i--; // index 이동
			lego_number--; // 개수 감소
			legos[i].id = -1; // 초기화
			break;
		}
	}
}

void lego_init(void) {
	printf("Init Success\n");
	for (int i = 0; i < 100; i++) {
		legos[i].id = -1; // id -1 means empty slot
	}
}

void load_lego(int i) {
	switch (i) {
	case 1:
		draw_obj(lego1);
		break;
	case 2:
		draw_obj(lego2);
		break;
	case 3:
		draw_obj(lego3);
		break;
	case 4:
		draw_obj(lego4);
		break;
	case 5:
		draw_obj(lego5);
		break;
	case 6:
		draw_obj(lego6);
		break;
	case 7:
		draw_obj(lego7);
		break;
	case 8:
		draw_obj(lego8);
		break;
	case 9:
		draw_obj(lego9);
		break;
	case 0:
		draw_obj(lego0);
		break;
	}
}

void draw_lego(void) {
	// index에 있는 레고 그리기
	for (int i = 0; i < 100; i++) {
		if (legos[i].id != -1) {
			glPushMatrix();

			// Modeling Transformation
			glTranslatef(legos[i].lego_X, legos[i].lego_Y, legos[i].lego_Z);
			glRotatef(legos[i].lego_angle, legos[i].lego_angleX, legos[i].lego_angleY, legos[i].lego_angleZ);
			glScalef(legos[i].lego_scaleX, legos[i].lego_scaleY, legos[i].lego_scaleZ);

			glColor3f(legos[i].lego_R, legos[i].lego_G, legos[i].lego_B);
			load_lego(legos[i].id);

			glClearColor(1, 1, 1, 0);
			
			glPopMatrix();
		}
	}
	glFlush();
}

void default_lego(void) {
	// 레고 위치 좌표
	locationX = 0;
	locationY = 0;
	locationZ = 0;

	// 레고 회전 각도
	angle = 0;
	angleX = 0;
	angleY = 0;
	angleZ = 0;

	// 레고 크기
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// 레고 색깔
	colorR = 1, colorG = 0, colorB = 0;
	color_mode = 0;
}

void show_lego(void) {
	// Modeling Transformation
	glTranslatef(locationX, locationY, locationZ);
	glRotatef(angle, angleX, angleY, angleZ);
	glScalef(scaleX, scaleY, scaleZ);

	// 색 변경
	switch (color_mode) {
	case 0: // Red
		colorR = 1, colorG = 0, colorB = 0;
		break;
	case 1: // Yellow
		colorR = 0.8, colorG = 0.8, colorB = 0;
		break;
	case 2: // Green
		colorR = 0.5, colorG = 1, colorB = 0;
		break;
	case 3: // Darkgreen
		colorR = 0, colorG = 0.3, colorB = 0;
		break;
	case 4: // Beige
		colorR = 0.96, colorG = 0.64, colorB = 0.37;
		break;
	case 5: // Pink
		colorR = 1, colorG = 0.5, colorB = 0.5;
		break;
	case 6: // Skyblue
		colorR = 0.5, colorG = 0.7, colorB = 1;
		break;
	case 7: // Brown
		colorR = 0.34, colorG = 0.16, colorB = 0.16;
		break;
	case 8: // Saddlebrown
		colorR = 0.65, colorG = 0.37, colorB = 0.17;
		break;
	case 9: // Gray
		colorR = 0.6, colorG = 0.6, colorB = 0.6;
		break;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(colorR, colorG, colorB, 0.3);
	glClearColor(1, 1, 1, 0);

	load_lego(draw_mode);
	glDisable(GL_BLEND);

	glFlush();
}

void draw_axis(void) {
	glPushMatrix();

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
	glLineWidth(1); // 두께 다시 환원

	glPopMatrix();

	glClearColor(1, 1, 1, 0);
}

void preview(void) {
	if (preview_mode == true) {
		glViewport(current_width / 2, current_height / 2, current_width / 2, current_height / 2);
		glLoadIdentity();

		// Viewing Transformation
		gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

		glPushMatrix();
		draw_string(GLUT_BITMAP_9_BY_15, "Preview Mode", -1.5, 6, 0, 0, 0);

		// 색 변경
		glColor3f(colorR, colorG, colorB);

		load_lego(draw_mode);

		glClearColor(1, 1, 1, 0);
		glColor3f(1, 1, 1);

		glPopMatrix();
	}
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, current_width, current_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 조명이 카메라 위치
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Background
	glClearColor(1, 1, 1, 0);

	// 카메라 좌표계
	cameraX = radius*sin(theta*PI / 180)*cos(phi*PI / 180);
	cameraY = radius*sin(phi*PI / 180);
	cameraZ = radius*cos(theta*PI / 180)*cos(phi*PI / 180);
	upY = cos(phi*PI / 180);

	switch (mode) {
	case 0: // 첫 시작
		glClearColor(1, 1, 1, 0);

		// Viewing Transformation
		gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------------------------", -7, 4, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Welcome to Hamburger World!", -6, 2, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------------------------", -7, 0, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Click the Right Button & Selet the Menu", -7.3, -3, 0.2, 0.2, 0.2);
		break;
	case 1: // How to do
		glClearColor(0, 0, 0, 0.0f);
		glColor3f(1, 1, 1);
		texture_off();
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "------------------------------", -7, 6, 1, 1, 0.5);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "How To Do", -3, 5, 1, 1, 0.4);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "------------------------------", -7, 4, 1, 1, 0.5);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "1. Making Hamburger Mode : You can make your hamburger.", -7.5, 1, 1, 1, 1);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "2. Game Start Mode : You can enjoy each level of game.", -7.5, -1, 1, 1, 1);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "3. Write Mode : You can save your file.", -7.5, -3, 1, 1, 1);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "3. Read Mode : You can show your file.", -7.5, -5, 1, 1, 1);
		break;
	case 2: // Making Hamburger Mode
		glClearColor(1, 1, 1, 0);

		// Viewing Transformation
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);

		// Skybox
		glColor3f(1, 1, 1);
		draw_skybox();
		texture_off();

		draw_axis();
		draw_string(GLUT_BITMAP_HELVETICA_18, "Make your own hamburger!", -9, 9, 0, 0, 0);

		draw_lego();
		show_lego();
		preview();

		break;
	case 3: // Level 1
		glClearColor(1, 1, 1, 0);

		if (next_step >= 4) {
			isIdle = true;
			radius = 20;
			glutIdleFunc(idle);
		}

		// Viewing Transformation
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);

		// Skybox
		glColor3f(1, 1, 1);
		draw_skybox();
		texture_off();

		if (next_step < 4) draw_axis();
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 9, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Level 1", -8.4, 8, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 7, 0, 0, 0);

		draw_lego();
		if (next_step < 4) show_lego();
		level1();
		preview();

		break;
	case 4: // Level 2
		glClearColor(1, 1, 1, 0);

		if (next_step >= 7) {
			isIdle = true;
			radius = 20;
			glutIdleFunc(idle);
		}

		// Viewing Transformation
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);

		// Skybox
		glColor3f(1, 1, 1);
		draw_skybox();
		texture_off();

		if (next_step < 7) draw_axis();
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 9, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Level 2", -8.4, 8, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 7, 0, 0, 0);

		draw_lego();
		if (next_step < 7) show_lego();
		level2();
		preview();

		break;
	case 5: // Level 3
		glClearColor(1, 1, 1, 0);

		if (next_step >= 9) {
			isIdle = true;
			radius = 20;
			glutIdleFunc(idle);
		}

		// Viewing Transformation
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);

		// Skybox
		glColor3f(1, 1, 1);
		draw_skybox();
		texture_off();

		if (next_step < 9) draw_axis();
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 9, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Level 3", -8.4, 8, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "---------", -9, 7, 0, 0, 0);

		draw_lego();
		if (next_step < 9) show_lego();
		level3();
		preview();

		break;
	case 11:
		glClearColor(1, 1, 1, 0);

		isIdle = false;

		// Viewing Transformation
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);

		// Skybox
		glColor3f(1, 1, 1);
		draw_skybox();
		texture_off();

		draw_lego();
		break;
	}
	glutSwapBuffers(); // back buffer를 front buffer로 변경
}
