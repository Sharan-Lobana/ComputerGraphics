void reverse(char *str, int len)
{
  int i=0, j=len-1, temp;
  while (i<j)
  {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++; j--;
  }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
  int i = 0;
  while (x)
  {
    str[i++] = (x%10) + '0';
    x = x/10;
  }

  // If number of digits required is more, then
  // add 0s at the beginning
  while (i < d)
    str[i++] = '0';

  reverse(str, i);
  str[i] = '\0';
  return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
  // Extract integer part
  int ipart = (int)n;

  // Extract floating part
  float fpart = n - (float)ipart;

  // convert integer part to string
  int i = intToStr(ipart, res, 0);

  // check for display option after point
  if (afterpoint != 0)
  {
    res[i] = '.';  // add dot

    // Get the value of fraction part upto given no.
    // of points after dot. The third parameter is needed
    // to handle cases like 233.007
    fpart = fpart * pow(10, afterpoint);

    intToStr((int)fpart, res + i + 1, afterpoint);
  }
}

void specialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_RIGHT)
		world_y += r_step;

	if(key == GLUT_KEY_LEFT)
		world_y -= r_step;

	if(key == GLUT_KEY_UP)
	{
		if(progstep < 0.3)
			progstep += progstep_acc;
	}

	if(key == GLUT_KEY_DOWN)
		if(progstep > -0.3)
		progstep -= progstep_acc;

	if(key == GLUT_KEY_PAGE_DOWN)
	{
		if(world_y_trans < scale - 1)
			world_y_trans += 0.05;
	}
	if(key == GLUT_KEY_PAGE_UP)
	{
		if(world_y_trans > -scale + 1)
			world_y_trans -= 0.05;
	}
	glutPostRedisplay();
}

void rotate()
{
	double acc = progstep*cos(wind_y/180*PI)*wind_acc_factor - wing_speed*turbine_factor;

	wing_speed += acc;
	wing_z += wing_speed;
	progoffset += progstep;

	if(-0.78 + progoffset > 0.99)
		progoffset = 0.0;
	else if(-.98 + progoffset < -0.99)
		progoffset = 1.76;

	glutPostRedisplay();
}

void rotateWind(unsigned char key, int x, int y)
{
	if(key == 'a')
		wind_y -= r_step;
	else if(key == 'd')
		wind_y += r_step;
}

void mouseWheel(int button, int dir, int x, int y)
{
    if (button == 3)
    {
	     // Zoom in
	      if(scale <= 3.0)
		scale += 0.03;
    }
    else if(button == 4)
    {
	// Zoom out
	if(scale >= 0.5)
		scale -= 0.03;
    }
}
