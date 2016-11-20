#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>

#define GL_GLEXT_PROTOTYPES
#define PI 3.1416
double scale = 1.0;
double world_y = 0, wing_z = 0, z_factor = 100.0, r_step = 5.0;
double wind_y = 0, wind_x = 0;
double progoffset = 0.0, progstep = 0.000;
void Cone(double radius, double height, double c1, double c2,bool flag = true)
{
	int steps = 1000;
	for(int i=0;i<steps;++i)
	{
		double th = (360.0/steps)*i;
		glBegin (GL_LINES);
			if(flag)
	    glColor3f  (c1,c1,c1);
			else
			glColor3f(c1,0,0);
			glVertex3f  (radius*cos(th),radius*sin(th) , 0);
			if(flag)
			glColor3f  (c2, c2, c2);
			else
			glColor3f(c2,0,0);
			glVertex3f(0,0,height);
	    glEnd ();
	}
}
void Wing(double radius, double height, double epoch)
{
	glLoadIdentity();
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(.0,-.05,0);
	glRotatef(wing_z+epoch, 0.0, 0.0, 1.0);
	glTranslatef(.05,.0,-.07);
	glRotatef(90, 0.0, 1.0, 0.0);
	Cone(radius,height,.1,1);
}

//Arrow is from pos1 to pos2 with propagation offset of progoffset
//pos2 > pos1
//Initialize pos1 to -1.0 and pos2 to pos1 + length of the arrow
void Arrow(double radius,double x, double y, double pos1, double pos2, double c1, double c2)
{
	glLoadIdentity();
	glRotatef(world_y,0,1.0,0);
	glRotatef(wind_x,1.0,0,0);
	glRotatef(wind_y,0,1.0,0);
	
	if(progstep < 0)
	{
		double temp = c1;
		c1 = c2;
		c2 = temp;
	}
	else if(progstep == 0.0)
	{
		c1 = c2 = 0.0;
	}
	for(int i = 0; i < 100; i++)
	{
		double th = (360.0/100)*i;
		glBegin(GL_LINES);
			glColor3f(c1,0,0);
			glVertex3f(x+radius*cos(th),y+radius*sin(th),pos1+progoffset);
			glColor3f(c2,0,0);
			glVertex3f(x+radius*cos(th),y+radius*sin(th),pos2+progoffset);
		glEnd();
	}
	if(progstep > 0)
	{
		glTranslatef(x,y,pos2+progoffset);
		Cone(3*radius,fabs(pos2-pos1)*0.4,1.0,1.0,false);
	}
	else if(progstep < 0)
	{
		glTranslatef(x,y,pos1+progoffset);
		glRotatef(180,0,1.0,0);
		Cone(3*radius,fabs(pos2-pos1)*0.4,1.0,1.0,false);
	}


}
void drawCube()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color and depth buffers

	glLoadIdentity();	// Reset the model-view matrix

	// rotate the cube according to rx world_y and wing_z parameters , controlled via arrow keys

	glColor3f  (.3, .3, .3);
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	GLUquadricObj *quadObj = gluNewQuadric();
	gluCylinder(quadObj, .03, .05, 0.75, 100, 100);

	glLoadIdentity();
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.05,-.10);
	gluCylinder(quadObj, .05, .05, .07, 100, 100);

	glLoadIdentity();
	glRotatef(world_y, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.05,-.1);
	glRotatef(180, 0.0, 1.0, 0.0);
	Cone(.05,.0,.3,.3);

	Wing(.04,.5,0);
	Wing(.04,.5,90);
	Wing(.04,.5,180);
	Wing(.04,.5,270);

	Arrow(0.005,0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,0.2,-0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,0.2,-.98,-0.78,0.2,1.0);
	Arrow(0.005,-0.2,-0.2,-.98,-0.78,0.2,1.0);

	glLoadIdentity();
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

	glFlush();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	if ( key == GLUT_KEY_RIGHT )
		world_y += r_step;
	if ( key == GLUT_KEY_LEFT )
		world_y -= r_step;
	if ( key == GLUT_KEY_UP )
	{
		//z_step += .05;
		progstep += 0.0005;
	}
	if ( key == GLUT_KEY_DOWN )
	{
		//z_step -= .05;
		progstep -= 0.0005;
	}
	// if ( key == GLUT_KEY_PAGE_UP )
	// 	wing_z += r_step;
	// if ( key == GLUT_KEY_PAGE_DOWN )
	// 	wing_z -= r_step;

	glutPostRedisplay();
}
void rotate()
{
	wing_z += progstep*z_factor*cos(wind_x/180*PI)*cos(wind_y/180*PI);
	progoffset += progstep;
	if(-0.78 + progoffset > 0.99)
		progoffset = 0.0;
	else if(-.98 + progoffset < -0.99)
		progoffset = 1.76;
	glutPostRedisplay();
}
void rotateWind(unsigned char key, int x, int y)
{
	if(key == 'w')
		wind_x += r_step;
	else if(key == 's')
		wind_x -= r_step;
	else if(key == 'a')
		wind_y += r_step;
	else if(key == 'd')
		wind_y -= r_step;
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
	glutCreateWindow("Question 3");
	// Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
	// Callback Functions
	glutDisplayFunc(drawCube);
	glutIdleFunc(rotate);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(rotateWind);
	glutMainLoop();
}