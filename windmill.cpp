#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>

#define GL_GLEXT_PROTOTYPES
#define PI 3.141592654
#define SHARPNESS_FACTOR 500	//Defines number of lines a 3d object is composed of

double floor_x = -30;	//Angle by which the floor is rotated about x-axis
double world_y = 0;	//Used for rotation of world coordinates about y-axis
double wing_z = 0;	//Angle by which wings rotate about z-axis
double wind_y = 0;	//Angle of rotation of direction of wind along y-axis
double r_step = 5.0;	//Angle by which wind_y is incremented/decremented
double scale = 1.0;	//Used for zooming in and out
double world_y_trans = 0.0;	//Used for translation along y-axis
double wing_speed = 0.0;	//Rotational speed of wings
double wind_acc_factor = .8; 	//Used in viscosity equation as coefficient of wind acceleration
double turbine_factor = 0.005;	//Used in viscosity equation as coefficient of wing speed
double progoffset = 0.0;	//Offset of arrow
double progstep = 0.000;	//Value by which progoffset is incremented/decremented
double progstep_acc = .0005;	//Rate of change of progstep
double torqueFact = 0.00002;	//Used for calculation of power

#include "shapes.h"
#include "user.h"

//Utility Function to reload identity matrix for objects
void resetIdentity()
{
	glLoadIdentity();
	glTranslatef(0.0,world_y_trans,0.0);
	glScalef(scale,scale,scale);	//Scaling used for zooming in and out
}

//Display Function
void drawWindMill()
{
	GLUquadricObj *quadObj = gluNewQuadric();	//Used for constructing wing axle
	char* st = new char[100];	//Used for printing DASHBOARD
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color and depth buffers

	resetIdentity();
	glRotatef(world_y+90, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	Shaft(.03,.02,.75);

	resetIdentity();
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.03,-.07);
	gluCylinder(quadObj, .03, .03, .05, 100, 100);

	resetIdentity();
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.03,-.07);
	Cone(.03,.0,.6,.6);

	glLoadIdentity();
	glOrtho(-500, 500, -500, 500, -100000.0, 100000.0);
	glColor4f(0.20, 0.20, 0.20, 0.0);
	glRasterPos2f(200, 280.0);
	strcpy(st,"DASHBOARD");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18,st);

	//Wing speed
	glColor4f(0.0, .7, 0.7, 0.0);
	glRasterPos2f(200, 240.0);
	strcpy(st,"Wing Speed(deg/frame): ");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18,st);
	if(wing_speed<0.0)
	{
		strcpy(st,"-");
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12,st);
	}
	ftoa(fabs(wing_speed),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	//Wind speed
	glColor4f(.7, 0.0, 0.0, 0.0);
	glRasterPos2f(200, 210.0);
	strcpy(st,"Wind Speed(km/hr):         ");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18,st);
	if(progstep<0.0)
	{
		strcpy(st,"-");
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12,st);
	}
	ftoa(1000*fabs(progstep),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	glColor4f(.7, 0.0, .7, 0.0);
	glRasterPos2f(200, 180.0);
	strcpy(st,"Wind Speed z(km/hr):      ");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18,st);
	if(progstep*cos(wind_y/180*PI)<0.0)
	{
		strcpy(st,"-");
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12,st);
	}
	ftoa(1000*fabs(progstep*cos(wind_y/180*PI)),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	glColor4f(.7, .7, .7, 0.0);
	glRasterPos2f(200, 150.0);
	strcpy(st,"Power (MW):                    ");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18,st);
	ftoa(1000*fabs(torqueFact*wing_speed*wing_speed),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	//Construct Four Arrows
	Arrow(0.005,0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,0.2,-0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,-0.2,-.98,-0.78,0.2,1.0);

	//Construct Three Wings
	TriangularWing(0.055,0.05,0.4,0.025,30);
	TriangularWing(0.055,0.05,0.4,0.025,150);
	TriangularWing(0.055,0.05,0.4,0.025,270);

	//Ground below windmill
	resetIdentity();
	glRotatef(-10, 1.0, 0.0, 0.0);
	glRotatef(world_y+45, 0.0, 1.0, 0.0);
	for(int i=0;i<1000;++i)
	{
		glBegin (GL_LINES);
			glColor3f  (0,.1+.3*i/1000.0,0);
			glVertex3f  (-.5+i/1000.0,-.75, .5);
			glColor3f  (0,.7+.3*i/1000.0,0);
			glVertex3f  (-.5+i/1000.0,-.75, -.5);
		glEnd();
	}

	dashboard_arrow(0.005,.6,-.55,(progstep>0 ? .2 : -.2),.2,0.7);

	//Construct Circle around dashboard_arrow
	glLoadIdentity();
	glTranslatef(.6,-.55,0);
	glRotatef(floor_x/2,1,0,0);
	for(int i=0;i<1000;i++)
	{
		double th = (2*PI/1000)*i;
		glBegin (GL_LINES);
			glColor3f  (.6,.6,.6);
			glVertex3f  (.28*cos(th),0, .28*sin(th));
			glVertex3f  (.29*cos(th),0, .29*sin(th));
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Initialize GLUT and process user parameters
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	// Request double buffered true color window with Z-buffer
	glutInitWindowSize(1000, 1000);	// Set Window Size
	glutInitWindowPosition(100,100);	//Specify Position of Window
	glutCreateWindow("WindMill");	// Create Window
	glEnable(GL_DEPTH_TEST);	// Enable Z-buffer depth test

	// Callback Functions
	glutDisplayFunc(drawWindMill);
	glutIdleFunc(rotate);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(rotateWind);
	glutMouseFunc(mouseWheel);
	glutMainLoop();
}
