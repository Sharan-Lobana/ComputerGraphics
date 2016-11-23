
//Utility function for constructing Cone(used as arrow heads)
//Radius is the radius of the base
//height is the height of cone
//c1,c2 are used for coloring
//flag is use for color gradient definition
//(disc of wings axle is constructed using cone with height 0 and flag set to true)
void Cone(double radius, double height, double c1, double c2,bool flag = true)
{
	int steps = 1000;
	for(int i=0;i<steps;++i)
	{
	double th = (360.0/steps)*i;
	glBegin(GL_LINES);
		if(flag)
			glColor3f  (c1,c1,c1);
		else
		glColor3f(c1,0,0);
		glVertex3f(radius*cos(th),radius*sin(th),0);
		if(flag)
		glColor3f(c2,c2,c2);
		else
		glColor3f(c2,0,0);
		glVertex3f(0,0,height);
	glEnd ();
	}
}

//Utility function for constructing ThreeDtriangle
//base_length is the length of the base
//height is the height of triangle
//y_span is the thickness of triangle
//c1,c2 are used for coloring
void ThreeDtriangle(double base_length, double height, double y_span, double c1, double c2)
{
	//Bottom Face
	for(int i=0;i<=SHARPNESS_FACTOR;i++)
	{
		glBegin(GL_LINES);
			glColor3f(c1,c1,c1);	//Base Color
			glVertex3f(((double)i*base_length)/SHARPNESS_FACTOR-base_length/2,0.0,0.0);
			glColor3f(c2,c2,c2);
			glVertex3f(0.0,0.0,height);
		glEnd();
	}
	//Top Face
	for(int i=0;i<=SHARPNESS_FACTOR;i++)
	{
		glBegin(GL_LINES);
			glColor3f(c1,c1,c1);	//Base Color
			glVertex3f(((double)i*base_length)/SHARPNESS_FACTOR-base_length/2,y_span,0.0);
			glColor3f(c2,c2,c2);
			glVertex3f(0.0,y_span,height);
		glEnd();
	}
	//Left Face
	for(int i=0;i<=SHARPNESS_FACTOR;i++)
	{
		glBegin(GL_LINES);
			glColor3f(c2,c2,c2);
			glVertex3f(-1*base_length/2,(double)i*y_span/SHARPNESS_FACTOR,0.0);
			glColor3f(c2,c2,c2);
			glVertex3f(0.0,(double)i*y_span/SHARPNESS_FACTOR,height);
		glEnd();
	}
	//Right Face
	for(int i=0;i<=SHARPNESS_FACTOR;i++)
	{
		glBegin(GL_LINES);
			glColor3f(c2,c2,c2);
			glVertex3f(base_length/2,(double)i*y_span/SHARPNESS_FACTOR,0.0);
			glColor3f(c2,c2,c2);
			glVertex3f(0.0,(double)i*y_span/SHARPNESS_FACTOR,height);
		glEnd();
	}
	//Front Face
	for(int i=0;i<=SHARPNESS_FACTOR;i++)
	{
		glBegin(GL_LINES);
			glColor3f(c1,c1,c1);
			glVertex3f(-1.0*base_length/2,i*y_span/SHARPNESS_FACTOR,0.0);
			glColor3f(c1,c1,c1);
			glVertex3f(1.0*base_length/2,i*y_span/SHARPNESS_FACTOR,0.0);
		glEnd();
	}
}

//Utility Function for constructing Wind Arrows
//Radius is radius of cylindrical tail
//x,y are the coordinates of translation
//pos1,pos2 define the end points of the tail along its axis
//c1,c2 are used for coloring the arrow
void Arrow(double radius,double x, double y, double pos1, double pos2, double c1, double c2)
{
	glLoadIdentity();
	glTranslatef(0.0,world_y_trans,0.0);
	glScalef(scale,scale,scale);
	glTranslatef(x,y,0);
	glRotatef(floor_x/2,1,0,0);
	glRotatef(world_y,0,1.0,0);
	glRotatef(wind_y,0,1.0,0);

	if(progstep < 0)
	{
		double temp = c1;
		c1 = c2;
		c2 = temp;
	}

	if(fabs(progstep) < .00001)
		return;

	for(int i=0;i<100;i++)
	{
		double th = (360.0/100)*i;
		glBegin(GL_LINES);
			glColor3f(c1,0,0);
			glVertex3f(radius*cos(th),radius*sin(th),pos1+progoffset);
			glColor3f(c2,0,0);
			glVertex3f(radius*cos(th),radius*sin(th),pos2+progoffset);
		glEnd();
	}

	if(progstep > 0)
	{
		glTranslatef(0,0,pos2+progoffset);
		Cone(3*radius,fabs(pos2-pos1)*0.4,.5,1.0,false);
	}
	else if(progstep < 0)
	{
		glTranslatef(0,0,pos1+progoffset);
		glRotatef(180,0,1.0,0);
		Cone(3*radius,fabs(pos2-pos1)*0.4,.5,1.0,false);
	}
}

//Utility Function for constructing dashboard_arrow
//Radius is radius of cylindrical tail
//x,y are the coordinates of translation
//len is the length of the tail
//c1,c2 are used for coloring the arrow
void dashboard_arrow(double radius,double x, double y, double len, double c1, double c2)
{
	glLoadIdentity();
	glTranslatef(x,y,0);
	glRotatef(floor_x/2,1.0,0.0,0);
	glRotatef(world_y,0,1.0,0);
	glRotatef(wind_y,0,1.0,0);

	if(fabs(progstep) < .00001)
		return;

	for(int i = 0; i < 100; i++)
	{
		double th = (360.0/100)*i;
		glBegin(GL_LINES);
			glColor3f(c1,0,0);
			glVertex3f(radius*cos(th),radius*sin(th),0);
			glColor3f(c2,0,0);
			glVertex3f(radius*cos(th),radius*sin(th),len);
		glEnd();
	}

	if(progstep > 0)
	{
		glTranslatef(0,0,len);
		Cone(3*radius,len*0.4,.2,.6,false);
	}
	else if(progstep < 0)
	{
		glTranslatef(0,0,len);
		glRotatef(180,0,1.0,0);
		Cone(3*radius,-len*0.4,.2,.6,false);
	}
}

//Utility Function for constructing TriangularWing
//base_length is the length of base of 3d triangle
//small_length is the height of smaller 3d triangle
//large_height is the height of larger 3d triangle
//y_span is the thickness of wing
//epoch is the intial angular offset of rotation about z-axis
void TriangularWing(double base_length, double small_height,double large_height, double y_span, double epoch)
{
	glLoadIdentity();
	glTranslatef(0.0,world_y_trans,0.0);
	glScalef(scale,scale,scale);
	glRotatef(world_y,0.0,1.0,0.0);
	glTranslatef(.0,-0.03,-0.065);
	glRotatef(-wing_z+epoch,0.0,0.0,1.0);
	glRotatef(60,1.0,0.0,0.0);
	glTranslatef(-1.4*small_height,0.0,0.0);
	glRotatef(90,0.0,1.0,0.0);
	ThreeDtriangle(base_length, small_height, y_span, 1.0, 0.6);

	glLoadIdentity();
	glTranslatef(0.0,world_y_trans,0.0);
	glScalef(scale,scale,scale);
	glRotatef(world_y,0.0,1.0,0.0);
	glTranslatef(.0,-0.03,-0.065);
	glRotatef(-wing_z+epoch,0.0,0.0,1.0);
	glRotatef(60,1.0,0.0,0.0);
	glTranslatef(-1.4*small_height,0.0,0.0);
	glRotatef(270,0.0,1.0,0.0);
	ThreeDtriangle(base_length, large_height, y_span, 1.0, 0.6);
}

//Utility Function for Constructing Shaft
//r_bottom is bottom radius
//r_top is top radius
//height is the height of staff
void Shaft(double r_bottom, double r_top, double height)
{
	int steps = 1000;
	for(int i=0;i<steps;++i)
	{
		double th = (2*PI/steps)*i;
		glBegin (GL_LINES);
			double c = .7 + cos(th)*.2;
			glColor3f  (c , c, c);
			glVertex3f  (r_top*cos(th),r_top*sin(th) , 0);
			glVertex3f  (r_bottom*cos(th),r_bottom*sin(th) , height);
		glEnd ();
	}
}

//Utility Function to print characters on screen
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
