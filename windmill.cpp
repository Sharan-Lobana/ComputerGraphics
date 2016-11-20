#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>

#define GL_GLEXT_PROTOTYPES

double scale = 1.0;
double rx = 0, ry = 0, rz = 0, z_step = 1.0, r_step = 5.0;

void Cone(double radius, double height, double c1, double c2)
{
	int steps = 1000;
	for(int i=0;i<steps;++i)
	{
		double th = (360.0/steps)*i;
		glBegin (GL_LINES);
	    glColor3f  (c1,c1,c1);
	    glVertex3f  (radius*cos(th),radius*sin(th) , 0);
	    glColor3f  (c2, c2, c2);
	    glVertex3f(0,0,height);
	    glEnd ();
	}
}
void Wing(double radius, double height, double epoch)
{
	glLoadIdentity();
	glRotatef(ry, 0.0, 1.0, 0.0);
	glTranslatef(.0,-.05,0);
	glRotatef(rz+epoch, 0.0, 0.0, 1.0);
	glTranslatef(.05,.0,-.07);
	glRotatef(90, 0.0, 1.0, 0.0);
	Cone(radius,height,.1,1);
}
void drawCube()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color and depth buffers

	glLoadIdentity();	// Reset the model-view matrix

	// rotate the cube according to rx ry and rz parameters , controlled via arrow keys

	glColor3f  (.3, .3, .3);
	glRotatef(ry, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	GLUquadricObj *quadObj = gluNewQuadric();
	gluCylinder(quadObj, .03, .05, 1.0, 100, 100);

	glLoadIdentity();
	glRotatef(ry, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.05,-.10);
	gluCylinder(quadObj, .05, .05, .07, 100, 100);

	glLoadIdentity();
	glRotatef(ry, 0.0, 1.0, 0.0);
	glTranslatef(0,-0.05,-.1);
	glRotatef(180, 0.0, 1.0, 0.0);
	Cone(.05,.0,.3,.3);

	Wing(.04,.5,0);
	Wing(.04,.5,90);
	Wing(.04,.5,180);
	Wing(.04,.5,270);

	glFlush();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	if ( key == GLUT_KEY_RIGHT )
		ry += r_step;
	if ( key == GLUT_KEY_LEFT )
		ry -= r_step;
	if ( key == GLUT_KEY_UP )
		z_step += .05;
	if ( key == GLUT_KEY_DOWN )
		z_step -= .05;
	// if ( key == GLUT_KEY_PAGE_UP )
	// 	rz += r_step;
	// if ( key == GLUT_KEY_PAGE_DOWN )
	// 	rz -= r_step;

	glutPostRedisplay();
}
void rotate()
{
	rz += z_step;
	glutPostRedisplay();
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

	glutMainLoop();
}