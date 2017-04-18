//
//  Main3.cpp
//

#include <string>
#include <iostream>
#include <cstdlib>
#include "GetGlut.h"
#include "Sleep.h"
#include "GridArr.h"

using namespace std;

// prototypes
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void update();
void reshape(int w, int h);
void display();

//declare global variables here
GridArr Old;
GridArr New;
float wide;
float hight;


int main (int argc, char** argv)
{
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Third OpenGL Program");
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	initDisplay();

	glutMainLoop();

	return 1;
}

void initDisplay()
{
	Old.InitialGrid();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void update()
{
	//update your variables here
	Old.UpdateCell(New);
	Old.CopyGrid(New);
	New.Reset();
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);

//	int x_center = w / 2;
//	int y_center = h / 2;
	wide = w;
	hight = h;
	
	// set up a 2D view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// NEW: Jan. 11, 2015, 8:10 pm
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display
	
	float red;
	float green;
	float blue;
	float dx = wide / MAXCOLS;
	float dy = hight / MAXROWS;
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			red = Old.mass[x][y];
			green = Old.mass[x][y] * Old.mass[x][y] * 0.05;
			blue = Old.mass[x][y] * Old.mass[x][y] * Old.mass[x][y] * 0.0001;
			glBegin(GL_QUADS);
				glColor3f(red, green, blue);
				float xc = x * dx;
				float yc = y * dy;
				glVertex2f(xc, yc);
				glVertex2f(xc + dx, yc);
				glVertex2f(xc + dx, yc + dy);
				glVertex2f(xc, yc + dy);
			glEnd();
		}
	}
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}