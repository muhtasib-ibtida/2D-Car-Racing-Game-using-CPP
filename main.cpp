#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

//Game Speed
int FPS = 50;
//Game Track
int start = 0;
int gv = 0;
int level = 0;

//Track Score
int score = 0;

//For move track
float roadDivTop = 0;
float roadDivMdl = 0;
float roadDivBtm = 0;

//For Card Left / RIGHT
int lrIndex = 0;

//Car Coming
int car1 = 0;
int lrIndex1 = 0;
int car2 = +35;
int lrIndex2 = 0;
int car3 = +70;
int lrIndex3 = 0;
bool soundOn = true;
int highestScore = 0;

//variables Level 2
int controllableCarSpeed = 0;
int controllableCarLane = 0;

int opponentCars[5] = {0, -20, -40, -60, -80};
int opponentCarSpeeds[5] = {2, 3, 4, 5, 6};
int opponentCarLanes[5] = {0, 1, 2, 3, 4};

//For Display TEXT
const void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;
const void* font2 = GLUT_BITMAP_HELVETICA_18;
const void* font3 = GLUT_BITMAP_8_BY_13;


char s[30];



void renderBitmapString(float x, float y, void *font, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void tree(int x, int y)
{
    int newx = x;
    int newy = y;
    //Bottom
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55);
    glVertex2f(newx + 12, newy + 55 - 10);
    glVertex2f(newx + 10, newy + 55 - 10);

    glEnd();
    //Top
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55 + 3);
    glVertex2f(newx + 12 + 3, newy + 55 - 3);
    glVertex2f(newx + 10 - 3, newy + 55 - 3);
    glEnd();
}

void saveHighestScore(int score) {
    std::ofstream file("highest_score.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

int loadHighestScore() {
    std::ifstream file("highest_score.txt");
    int score = 0;
    if (file.is_open()) {
        file >> score;
        file.close();
    }
    return score;
}
void startGame()
{
    //Road
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //Road Left Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(23, 100);
    glVertex2f(23, 0);
    glEnd();

    //Road Right Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(77, 0);
    glVertex2f(77, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //TOP
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);
    glVertex2f(48, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 80);
    glEnd();
    roadDivTop = roadDivTop - 0.5;
    if (roadDivTop < -100)
    {
        roadDivTop = 20;
        score++;
    }
    //Middle
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMdl + 40);
    glVertex2f(48, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 40);
    glEnd();
    roadDivMdl = roadDivMdl - 0.5;
    if (roadDivMdl < -60)
    {
        roadDivMdl = 60;
        score++;
    }
    //Bottom
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);
    glVertex2f(48, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 0);
    glEnd();
    roadDivBtm = roadDivBtm - 0.5;
    if (roadDivBtm < -20)
    {
        roadDivBtm = 100;
        score++;
    }

    //Score Board
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);
    glVertex2f(100, 97);
    glVertex2f(100, 98 - 8);
    glVertex2f(80, 98 - 8);
    glEnd();

    //Print Score
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95, (void *)font3, buffer);
    //Speed Print
    char buffer1[50];
    sprintf(buffer1, "SPEED:%dKm/h", FPS);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 2, (void *)font3, buffer1);
    //level Print
    if (score % 50 == 0)
    {
        int last = score / 50;
        if (last != level)
        {
            level = score / 50;
            FPS = FPS + 2;
        }
    }
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level+1);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 4, (void *)font3, level_buffer);

    //Increase Speed With level

    //MAIN car
    //Front Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 5);
    glVertex2f(lrIndex + 26 - 2, 7);
    glVertex2f(lrIndex + 30 + 2, 7);
    glVertex2f(lrIndex + 30 + 2, 5);
    glEnd();
    //Back Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 1);
    glVertex2f(lrIndex + 26 - 2, 3);
    glVertex2f(lrIndex + 30 + 2, 3);
    glVertex2f(lrIndex + 30 + 2, 1);
    glEnd();
    //Car Body
    glColor3f(0.678, 1.000, 0.184);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 1);
    glVertex2f(lrIndex + 26, 8);
    glColor3f(0.000, 0.545, 0.545);

    glVertex2f(lrIndex + 28, 10);
    glVertex2f(lrIndex + 30, 8);
    glVertex2f(lrIndex + 30, 1);
    glEnd();

    //Opposite car 1
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 4);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100);
    glEnd();
    glColor3f(1.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26, car1 + 100);
    glVertex2f(lrIndex1 + 26, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 28, car1 + 100 - 9);
    glVertex2f(lrIndex1 + 30, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 30, car1 + 100);
    glEnd();
    car1--;
    if (car1 < -100)
    {
        car1 = 0;
        lrIndex1 = lrIndex;
    }
    //Kill check car1
    if ((abs(lrIndex - lrIndex1) < 8) && (car1 + 100 < 10))
    {
        PlaySound(TEXT("MyAppSound"), NULL, SND_APPLICATION);
        start = 0;
        gv = 1;

         // Check for new highest score
        if (score > highestScore) {
            highestScore = score;
            saveHighestScore(highestScore);
        }
    }



    //Opposite car 2
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 4);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100);
    glEnd();
    glColor3f(0.300, 0.000, 0.500);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26, car2 + 100);
    glVertex2f(lrIndex2 + 26, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 28, car2 + 100 - 9);
    glVertex2f(lrIndex2 + 30, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 30, car2 + 100);
    glEnd();
    car2--;
    if (car2 < -100)
    {
        car2 = 0;
        lrIndex2 = lrIndex;
    }
    //Kill check car2
    if ((abs(lrIndex - lrIndex2) < 8) && (car2 + 100 < 10))
    {
        PlaySound(TEXT("MyAppSound"), NULL, SND_APPLICATION);
        start = 0;
        gv = 1;

         // Check for new highest score
        if (score > highestScore) {
            highestScore = score;
            saveHighestScore(highestScore);
        }

    }


    //Opposite car 3
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 4);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100);
    glEnd();
    glColor3f(1.000, 0.271, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26, car3 + 100);
    glVertex2f(lrIndex3 + 26, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 28, car3 + 100 - 9);
    glVertex2f(lrIndex3 + 30, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 30, car3 + 100);
    glEnd();
    car3--;
    if (car3 < -100)
    {
        car3 = 0;
        lrIndex3 = lrIndex;
    }
    //Kill check car3
    if ((abs(lrIndex - lrIndex3) < 8) && (car3 + 100 < 10))
    {
        PlaySound(TEXT("MyAppSound"), NULL, SND_APPLICATION);
        start = 0;
        gv = 1;

         // Check for new highest score
        if (score > highestScore) {
            highestScore = score;
            saveHighestScore(highestScore);
        }

    }}

    void startGame2()
{
    //Road
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //Road Left Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(23, 100);
    glVertex2f(23, 0);
    glEnd();

    //Road Right Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(77, 0);
    glVertex2f(77, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //TOP
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);
    glVertex2f(48, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 80);
    glEnd();
    roadDivTop = roadDivTop - 0.5;
    if (roadDivTop < -100)
    {
        roadDivTop = 20;
        score++;
    }
    //Middle
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMdl + 40);
    glVertex2f(48, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 40);
    glEnd();
    roadDivMdl = roadDivMdl - 0.5;
    if (roadDivMdl < -60)
    {
        roadDivMdl = 60;
        score++;
    }
    //Bottom
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);
    glVertex2f(48, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 0);
    glEnd();
    roadDivBtm = roadDivBtm - 0.5;
    if (roadDivBtm < -20)
    {
        roadDivBtm = 100;
        score++;
    }

    //Score Board
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);
    glVertex2f(100, 97);
    glVertex2f(100, 98 - 8);
    glVertex2f(80, 98 - 8);
    glEnd();

    //Print Score
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95, (void *)font3, buffer);
    //Speed Print
    char buffer1[50];
    sprintf(buffer1, "SPEED:%dKm/h", FPS);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 2, (void *)font3, buffer1);
    //level Print
    if (score % 50 == 0)
    {
        int last = score / 50;
        if (last != level)
        {
            level = score / 50;
            FPS = FPS + 2;
        }
    }
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level+1);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 4, (void *)font3, level_buffer);
    // Controllable car
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 5 + controllableCarLane * 20);
    glVertex2f(lrIndex + 26 - 2, 7 + controllableCarLane * 20);
    glVertex2f(lrIndex + 30 + 2, 7 + controllableCarLane * 20);
    glVertex2f(lrIndex + 30 + 2, 5 + controllableCarLane * 20);
    glEnd();

    // Update the controllable car's position based on speed
    controllableCarLane += controllableCarSpeed;
    if (controllableCarLane < 0)
        controllableCarLane = 0;
    if (controllableCarLane > 4)
        controllableCarLane = 4;

    // Opponent cars
    for (int i = 0; i < 5; i++)
    {
        glColor3f(0.000, 0.000, 0.000);
        glBegin(GL_POLYGON);
        glVertex2f(lrIndex + 26 - 2, opponentCars[i] + 100 - 4);
        glVertex2f(lrIndex + 26 - 2, opponentCars[i] + 100 - 6);
        glVertex2f(lrIndex + 30 + 2, opponentCars[i] + 100 - 6);
        glVertex2f(lrIndex + 30 + 2, opponentCars[i] + 100 - 4);
        glEnd();

        glColor3f(0.000, 0.000, 0.000);
        glBegin(GL_POLYGON);
        glVertex2f(lrIndex + 26 - 2, opponentCars[i] + 100);
        glVertex2f(lrIndex + 26 - 2, opponentCars[i] + 100 - 2);
        glVertex2f(lrIndex + 30 + 2, opponentCars[i] + 100 - 2);
        glVertex2f(lrIndex + 30 + 2, opponentCars[i] + 100);
        glEnd();

        glColor3f(0.8, 0.2, 0.2);
        glBegin(GL_POLYGON);
        glVertex2f(lrIndex + 26, opponentCars[i] + 100);
        glVertex2f(lrIndex + 26, opponentCars[i] + 100 - 7);
        glVertex2f(lrIndex + 28, opponentCars[i] + 100 - 9);
        glVertex2f(lrIndex + 30, opponentCars[i] + 100 - 7);
        glVertex2f(lrIndex + 30, opponentCars[i] + 100);
        glEnd();

        opponentCars[i] -= opponentCarSpeeds[i];

        // Collision check with the controllable car
        if ((abs(lrIndex - 26) < 8) && (abs(controllableCarLane * 20 - opponentCars[i] - 100) < 8))
        {
            PlaySound(TEXT("MyAppSound"), NULL, SND_APPLICATION);
            start = 0;
            gv = 1;

            // Check for new highest score
            if (score > highestScore)
            {
                highestScore = score;
                saveHighestScore(highestScore);
            }
        }

        // Reset opponent cars when they go off the screen
        if (opponentCars[i] < -100)
        {
            opponentCars[i] = 0;
        }
    }

    // Your existing code for displaying game over and scores
}

int currentLevel = 1;

void fristDesign()
{
    //Road BackGround
    glColor3f(0.000, 0.350, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glColor3f(0.6, 0.8, 0.3);
    glVertex2f(100, 50 - 50);
    glVertex2f(0, 50 - 50);
    glEnd();

    //Road Design In Front Page
    glColor3f(0, 0.3, 0.3);
    glBegin(GL_TRIANGLES);
    glVertex2f(32 - 2 + 21, 55);
    glVertex2f(32 + 58, 50 - 50);
    glVertex2f(32 - 22, 50 - 50);
    glEnd();
    //Road Middle
    glColor3f(0.8, 0.8, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(32 - 2 + 21, 55);
    glVertex2f(50 + 2, 50 - 50);
    glVertex2f(50 - 2, 50 - 50);
    glEnd();

    //Road Sky
    glColor3f(0.000, 0.60, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glColor3f(0.933, 0.933, 0.933);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glEnd();

    //Hill 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(20, 55 + 10);
    glVertex2f(20 + 7, 55);
    glVertex2f(0, 55);
    glEnd();

    //Hill 2
    glColor3f(0.000, 0.400, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(20 + 15, 55 + 12);
    glVertex2f(20 + 20 + 10, 55);
    glVertex2f(0 + 10, 55);
    glEnd();

    //Hill 4
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(87, 55 + 10);
    glVertex2f(100, 55);
    glVertex2f(60, 55);
    glEnd();

    //Hill 3
    glColor3f(0.000, 0.400, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(70, 70);
    glVertex2f(90, 55);
    glVertex2f(50, 55);
    glEnd();

    //Tree on front page
    tree(5, -15);
    tree(0, 0);
    tree(9, 5);
    tree(80, 0);
    tree(75, -15);
    tree(72, 5);

    // "AIUB" Text
    glColor3f(1.000, 1.000, 1.000);  // Blue color
    renderBitmapString(45, 90, (void *)font1, "AIUB");

    glColor3f(1.000, 1.000, 1.000);  // Blue color
    renderBitmapString(13, 85, (void *)font1, "Course: Computer Graphics    Section:G");

     //"2D Racing Car" text.
    glColor3f(1.000, 1.000, 0.000);
    renderBitmapString(30, 80, (void *)font1, "2D Car Racing Game ");



    // White rectangle background for names and IDs
    glColor3f(1.000, 1.000, 1.000);  // White color
    glBegin(GL_POLYGON);
    glVertex2f(22-7, 58);
    glVertex2f(98-7, 58);
    glVertex2f(98-7, 38);
    glVertex2f(22-7, 38);
    glEnd();

    // Members' names and IDs
    glColor3f(0.000, 0.000, 0.000);  // Black color
    renderBitmapString(24-4, 55, (void *)font3, "       Name              ID    Contribution  ");
    renderBitmapString(24-4, 52, (void *)font3, "Nasimul Arafin Rounok   21-45353-2  -  20%  ");
    renderBitmapString(24-4, 49, (void *)font3, "Fardin Abu Ubaid        21-44863-2  -  20%  ");
    renderBitmapString(24-4, 46, (void *)font3, "Muhtasib Ibtida Kousik  21-44864-2  -  20%  ");
    renderBitmapString(24-4, 43, (void *)font3, "Adil Ahmed Shamim       21-45190-2  -  20%  ");
    renderBitmapString(24-4, 40, (void *)font3, "Hasin Aabrar Khan       21-45297-2  -  20%  ");



    //Menu Place Holder
    glColor3f(0.098, 0.098, 0.439);
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 30);
    glVertex2f(32 + 46, 30);
    glVertex2f(32 + 46, 13);
    glVertex2f(32 - 4, 13);
    glEnd();
   /* glColor3f(00, 0, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 37);
    glVertex2f(32 + 46,36);
    glVertex2f(32 + 46, 33);
    glVertex2f(32 - 4, 30);
    glEnd();*/
    /*glBegin(GL_POLYGON);
    glVertex2f(32 + 45, 37);
    glVertex2f(32 + 46, 37);
    glVertex2f(32 + 46, 33);
    glVertex2f(32 + 45, 30);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 - 14 + 10);
    glVertex2f(32 + 46, 50 - 14 + 10);
    glVertex2f(32 + 46, 50 - 15 + 10);
    glVertex2f(32 - 4, 50 - 15 + 10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 + 5 + 10);
    glVertex2f(32 - 5, 50 + 5 + 10);
    glVertex2f(32 - 5, 50 - 15 + 10);
    glVertex2f(32 - 4, 50 - 15 + 10);
    glEnd();*/

    //Texts inside the rectangle box
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(30, 27, (void *)font2, "Press SPACE to Start");
    renderBitmapString(30, 24, (void *)font2, "Press ESC      to Exit");

    glColor3f(1.000, 1.000, 1.000);
    renderBitmapString(30, 22, (void *)font3, "Press UP    to Increase Speed");
    renderBitmapString(30, 20, (void *)font3, "Press DWON  to Decrease Speed");
    renderBitmapString(30, 18, (void *)font3, "Press RIGHT to Turn Right");
    renderBitmapString(30, 16, (void *)font3, "Press LEFT  to Turn Left");


    if (gv == 1)
    {
        glColor3f(1.000, 0.000, 0.000);
        renderBitmapString(35, 75 , (void *)font1, "GAME OVER");
        glColor3f(1.000, 0.000, 0.000);
        char buffer2[50];
        sprintf(buffer2, "Your Score is : %d", score);
        renderBitmapString(33, 65 , (void *)font1, buffer2);

        // Display highest score
        char highestScoreBuffer[50];
        sprintf(highestScoreBuffer, "Highest Score: %d", highestScore);
        glColor3f(1.000, 0.000, 0.000);
        renderBitmapString(33, 60 , (void *)font1, highestScoreBuffer);
    }


}



/*void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 1)
    {
        glClearColor(0.000, 0.392, 0.000, 1);
        startGame();
    }

    else
    {
        fristDesign();
    }

    glFlush();
    glutSwapBuffers();
}*/

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 1)
    {
        glClearColor(0.000, 0.392, 0.000, 1);
        if (currentLevel == 1) {
            startGame();
        } else if (currentLevel == 2) {
            startGame2();
        }
    }
    else
    {
        fristDesign();
    }

    glFlush();
    glutSwapBuffers();
}

void spe_key(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        if (FPS > (50 + (level * 2)))
            FPS = FPS - 2;
        break;
    case GLUT_KEY_UP:
        FPS = FPS + 2;
        break;

    case GLUT_KEY_LEFT:
        if (lrIndex >= 0)
        {
            lrIndex = lrIndex - (FPS / 10);
            if (lrIndex < 0)
            {
                lrIndex = -1;
            }
        }
        break;

    case GLUT_KEY_RIGHT:
        if (lrIndex <= 44)
        {
            lrIndex = lrIndex + (FPS / 10);
            if (lrIndex > 44)
            {
                lrIndex = 45;
            }
        }
        break;

    default:
        break;
    }
}

void processKeys(unsigned char key, int x, int y)
{

    switch (key)
    {
    case ' ':
        if (start == 0)
        {
            PlaySound("car.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
            start = 1;
            gv = 0;
            FPS = 50;
            roadDivTop = 0;
            roadDivMdl = 0;
            roadDivBtm = 0;
            lrIndex = 0;
            car1 = 0;
            lrIndex1 = 0;
            car2 = +35;
            lrIndex2 = 0;
            car3 = +70;
            lrIndex3 = 0;
            score = 0;
            level = 0;
            // Reset the current level to 1 when starting a new game
            currentLevel = 1;
        }
        break;
        case 'L':
    case 'l':
        // Press 'L' to switch between levels
        if (start == 1) {
            currentLevel = (currentLevel == 1) ? 2 : 1;
            std::cout << "Switched to Level " << currentLevel << std::endl;
        }
        break;

    case 27:
        exit(0);
        break;
    case 'S':
    case 's':
        soundOn = !soundOn;
        if (soundOn)
            PlaySound("car.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        else
            PlaySound(NULL, 0, 0);
        break;
    default:
        break;
    }
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 650);
    glutInitWindowPosition(200, 20);
    glutCreateWindow("2D-Car-Game");


    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(processKeys);

    highestScore = loadHighestScore();

    glOrtho(0, 100, 0, 100, -1, 1);
    glClearColor(0.184, 0.310, 0.310, 1);

    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;
}
