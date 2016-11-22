#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>

#define GL_GLEXT_PROTOTYPES
#define PI 3.141592654
#define SHARPNESS_FACTOR 500

double floor_x = -30, world_y = 0, wing_z = 0, z_factor = 100.0, r_step = 5.0, scale = 1.0, world_y_trans = 0.0 , wing_speed = 0.0;
double wind_y = 0, wind_x = 0, wind_acc_factor = .8, turbine_factor = 0.005;
double progoffset = 0.0, progstep = 0.000, progstep_acc = .0005;
double torqueFact = 0.00002;
#include "shapes.h"
#include "user.h"


int w=1000,h=1000;

void print_bitmap_string(void* font, char* s)
{
	if(s&&strlen(s))
	{
		while(*s)
		{
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

void drawWindMill()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color and depth buffers

	glLoadIdentity();
  	glTranslatef(0.0,world_y_trans,0.0);
  	glScalef(scale,scale,scale);	// Reset the model-view matrix

	// rotate the cube according to rx world_y and wing_z parameters , controlled via arrow keys
	glRotatef(world_y+90, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	Shaft(.03,.02,.75);
	
	GLUquadricObj *quadObj = gluNewQuadric();

	glLoadIdentity();
  	glTranslatef(0.0,world_y_trans,0.0);
  	glScalef(scale,scale,scale);
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.03,-.07);
	gluCylinder(quadObj, .03, .03, .05, 100, 100);

	glLoadIdentity();
  	glTranslatef(0.0,world_y_trans,0.0);
  	glScalef(scale,scale,scale);
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.03,-.07);
	Cone(.03,.0,.6,.6);

	GLdouble size;
	GLdouble aspect;
	glLoadIdentity();
	size = (GLdouble)((w >= h) ? w : h) / 2.0;
	if (w <= h) {
	  aspect = (GLdouble)h/(GLdouble)w;
	  glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
	}
	else {
	  aspect = (GLdouble)w/(GLdouble)h;
	  glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
	}

	glColor4f(0.20, 0.20, 0.20, 0.0);
	glRasterPos2f(200, 280.0);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, "DASHBOARD");
	
	//Wing speed
	glColor4f(0.0, .7, 0.7, 0.0);
	glRasterPos2f(200, 240.0);
	char* st = new char[100];
	ftoa(fabs(wing_speed),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, "Wing Speed(deg/frame): ");
	if(wing_speed<0.0)	print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "-");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	//Wind speed
	glColor4f(.7, 0.0, 0.0, 0.0);
	glRasterPos2f(200, 210.0);
	ftoa(1000*fabs(progstep),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, "Wind Speed(km/hr):         ");
	if(progstep<0.0)	print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "-");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	glColor4f(.7, 0.0, .7, 0.0);
	glRasterPos2f(200, 180.0);
	ftoa(1000*fabs(progstep*cos(wind_y/180*PI)*cos(wind_x/180*PI)),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, "Wind Speed z(km/hr):      ");
	if(progstep*cos(wind_y/180*PI)*cos(wind_x/180*PI)<0.0)	print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "-");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	glColor4f(.7, .7, .7, 0.0);
	glRasterPos2f(200, 150.0);
	ftoa(1000*fabs(torqueFact*wing_speed*wing_speed),st,4);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, "Power (MW):                    ");
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, st);

	Arrow(0.005,0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,0.2,-0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,-0.2,-.98,-0.78,0.2,1.0);

	TriangularWing(0.055,0.05,0.4,0.025,30);
	TriangularWing(0.055,0.05,0.4,0.025,150);
	TriangularWing(0.055,0.05,0.4,0.025,270);

	

	glLoadIdentity();
  	glTranslatef(0.0,world_y_trans,0.0);
  	glScalef(scale,scale,scale);
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
	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);
	// Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Set Window Size
	glutInitWindowSize(1000, 1000);
	// Set Window Position
	glutInitWindowPosition(100,100);
	// Create Window
	glutCreateWindow("WindMill");
	// Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
	
	// Callback Functions
	glutDisplayFunc(drawWindMill);
	glutIdleFunc(rotate);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(rotateWind);
	glutMouseFunc(mouseWheel);
	glutMainLoop();
}