// Mini Project scoresum 12171788 박지은

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <GL/freeglut.h>
#include <string.h>
#include <iostream>
#include "bmpfuncs.h"
#include "ObjParser.h"
#include "GL/glext.h"

// Next Step
int next_step = -1;

// Score
int score_sum = 0;
bool plusminus; // +면 true -면 false

bool preview_mode = false;

struct lego {
   int id;

   // 레고 색
   int lego_color;
   float lego_R, lego_G, lego_B;

   // 레고 위치 좌표
   float lego_X, lego_Y, lego_Z;

   // 레고 회전 각도
   float lego_angle;
   float lego_angleX, lego_angleY, lego_angleZ;

   // 레고 크기
   float lego_scaleX, lego_scaleY, lego_scaleZ;
};

// Lego Struct
lego legos[100];
int lego_number = 0;

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue);
void level1(void);
void level1_score(void);
void level2(void);
void level2_score(void);
void level3(void);
void level3_score(void);

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(-10, 10, -10, 10);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glColor3f(red, green, blue);
   glRasterPos3f(x_position, y_position, 0);
   for (unsigned int i = 0; i < strlen(str); i++) {
      glutBitmapCharacter(font, str[i]);
   }
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopAttrib();
}

void level1(void) { // 빵 + 토마토 + 양상추 + 패티 + 빵
   switch (next_step) {
   case -1:
      draw_string(GLUT_BITMAP_HELVETICA_18, "Bottom Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 0:
      // 이전 단계 점수 출력
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);

      draw_string(GLUT_BITMAP_HELVETICA_18, "Tomato : Red", -2, 6, 1, 0, 0);
      break;
   case 1:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Lettuce : Green", -2, 6, 0.5, 1, 0);
      break;
   case 2:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Meat : Brown", -2, 6, 0.34, 0.34, 0.16);
      break;
   case 3:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Top Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 4:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      glClearColor(0.5, 0.5, 0.5, 0);
      // Score
      char buf[100];
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -5, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -6, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -5, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -6, 0.4, 0.4, 0.4);
      }

      preview_mode = false; // 게임이 끝나면 무조건 OFF
      break;
   case 5:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      glClearColor(0.5, 0.5, 0.5, 0);
      // Score
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -5, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -6, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -5, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -6, 0.4, 0.4, 0.4);
      }

      preview_mode = false; // 게임이 끝나면 무조건 OFF
      break;
   }
}

void level1_score(void) {
   switch (next_step) {
   case 0: // 빵인지 확인
      // Check
      if (legos[0].id == 2) {
         if (legos[0].lego_color == 8) {
            score_sum = score_sum + 20;
            plusminus = true;
            printf("+20\n");
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
            printf("+10\n");
         }
         break;
      }
      else if (legos[0].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         printf("-20\n");
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         printf("-10\n");
         break;
      }
      break;
   case 1:
      // Check
      if (legos[1].id == 5) {
         if (legos[1].lego_color == 0) {
            score_sum = score_sum + 20;
            plusminus = true;
            printf("+20\n");
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true; 
            printf("+10\n");
         }
         break;
      }
      else if (legos[1].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         printf("-20\n");
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         printf("-10\n");
         break;
      }
      break;
   case 2:
      // Check
      if (legos[2].id == 9) {
         if (legos[2].lego_color == 2) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[2].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 3:
      // Check
      if (legos[3].id == 5) {
         if (legos[3].lego_color == 7) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[3].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 4:
      // Check
      if (legos[4].id == 1) {
         if (legos[4].lego_color == 8) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[4].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 5:
      score_sum = score_sum;
      break;
   }
}

void level2(void) { // // 빵 + 토마토 + 양상추 + 패티 + 베이컨 + 베이컨 + 치즈 + 빵
   switch (next_step) {
   case -1:
      draw_string(GLUT_BITMAP_HELVETICA_18, "Bottom Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 0:
      // 이전 단계 점수 출력
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);

      draw_string(GLUT_BITMAP_HELVETICA_18, "Tomato : Red", -2, 6, 1, 0, 0);
      break;
   case 1:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Lettuce : Green", -2, 6, 0.5, 1, 0);
      break;
   case 2:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Meat : Brown", -2, 6, 0.34, 0.34, 0.16);
      break;
   case 3:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Bacon : Pink", -2, 6, 1, 0.5, 0.5);
      break;
   case 4:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Bacon : Pink", -2, 6, 1, 0.5, 0.5);
      break;
   case 5:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Cheese : Yello", -2, 6, 0.8, 0.8, 0);
      break;
   case 6:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Top Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 7:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      glClearColor(0.5, 0.5, 0.5, 0);
      // Score
      char buf[100];
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -4, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -4, 0.4, 0.4, 0.4);
      }
      preview_mode = false;
      break;
   case 8:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      glClearColor(0.5, 0.5, 0.5, 0);
      // Score
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -4, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -4, 0.4, 0.4, 0.4);
      }
      preview_mode = false;
      break;
   }
}

void level2_score(void) {
   switch (next_step) {
   case 0:
      if (legos[0].id == 2) {
         if (legos[0].lego_color == 8) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[0].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 1:
      if (legos[1].id == 5) {
         if (legos[1].lego_color == 0) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[1].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 2:
      if (legos[2].id == 9) {
         if (legos[2].lego_color == 2) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[2].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 3:
      if (legos[3].id == 5) {
         if (legos[3].lego_color == 7) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[3].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 4:
      if (legos[4].id == 4) {
         if (legos[4].lego_color == 5) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[4].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 5:
      if (legos[5].id == 4) {
         if (legos[5].lego_color == 5) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[5].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 6:
      if (legos[6].id == 3) {
         if (legos[6].lego_color == 1) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[6].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 7:
      if (legos[7].id == 1) {
         if (legos[7].lego_color == 8) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[7].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 8:
      score_sum = score_sum;
      break;
   }
}

void level3(void) { // 빵 + 토마토 + 양상추 + 패티 + 피클 + 피클 + 치즈 + 빵 + 깨 + 깨
   switch (next_step) {
   case -1:
      draw_string(GLUT_BITMAP_HELVETICA_18, "Bottom Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 0:
      // 이전 단계 점수 출력
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);

      draw_string(GLUT_BITMAP_HELVETICA_18, "Tomato : Red", -2, 6, 1, 0, 0);
      break;
   case 1:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Lettuce : Green", -2, 6, 0.5, 1, 0);
      break;
   case 2:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Meat : Brown", -2, 6, 0.34, 0.34, 0.16);
      break;
   case 3:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Pickle : Dark Green", -2, 6, 0, 0.3, 0);
      break;
   case 4:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Pickle : Dark Green", -2, 6, 0, 0.3, 0);
      break;
   case 5:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Cheese : Yello", -2, 6, 0.8, 0.8, 0);
      break;
   case 6:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Top Bread : Saddle Brown", -2, 6, 0.65, 0.37, 0.17);
      break;
   case 7:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Sesame : Beige", -2, 6, 0.96, 0.64, 0.37);
      break;
   case 8:
      if (plusminus == true) draw_string(GLUT_BITMAP_HELVETICA_18, "You got plus score! :)", -2.5, -6, 0, 0, 0);
      else if (plusminus == false) draw_string(GLUT_BITMAP_HELVETICA_18, "You got minus score! :(", -2.5, -6, 0, 0, 0);
      draw_string(GLUT_BITMAP_HELVETICA_18, "Sesame : Beige", -2, 6, 0.96, 0.64, 0.37);
      break;
   case 9:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      // Score
      glClearColor(0.5, 0.5, 0.5, 0);
      char buf[100];
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -4, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -4, 0.4, 0.4, 0.4);
      }
      preview_mode = false;
      break;
   case 10:
      draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Stage complete!", -2, 6, 0, 0, 0);

      // Score
      glClearColor(0.5, 0.5, 0.5, 0);
      sprintf(buf, "Your Score : %d", score_sum);
      buf[strlen(buf)] = 0;

      if (score_sum >= 0) {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Very Good Job! :)", -2, -4, 0.4, 0.4, 0.4);
      }
      else {
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, -2, -3, 0, 0, 0);
         draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Try Again! :(", -2, -4, 0.4, 0.4, 0.4);
      }
      preview_mode = false;
      break;
   }
}

void level3_score(void) {
   switch (next_step) {
   case 0:
      if (legos[0].id == 2) {
         if (legos[0].lego_color == 8) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[0].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 1:
      if (legos[1].id == 5) {
         if (legos[1].lego_color == 0) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[1].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 2:
      if (legos[2].id == 9) {
         if (legos[2].lego_color == 2) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[2].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 3:
      if (legos[3].id == 5) {
         if (legos[3].lego_color == 7) {
            score_sum = score_sum + 20;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         break;
      }
      else if (legos[3].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 4:
      if (legos[4].id == 6) {
         if (legos[4].lego_color == 3) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[4].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 5:
      if (legos[5].id == 6) {
         if (legos[5].lego_color == 3) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[5].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 6:
      if (legos[6].id == 3) {
         if (legos[6].lego_color == 1) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[6].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 7:
      if (legos[7].id == 1) {
         if (legos[7].lego_color == 7) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[7].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 8:
      if (legos[8].id == 7) {
         if (legos[8].lego_color == 4) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[8].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 9:
      if (legos[9].id == 7) {
         if (legos[9].lego_color == 4) {
            score_sum = score_sum + 10;
            plusminus = true;
         }
         else {
            score_sum = score_sum + 5;
            plusminus = true;
         }
         break;
      }
      else if (legos[9].id == 0) {
         score_sum = score_sum - 20; // 생선은 항상 -20
         plusminus = false;
         break;
      }
      else {
         score_sum = score_sum - 10;
         plusminus = false;
         break;
      }
      break;
   case 10:
      score_sum = score_sum;
      break;
   }
}
