#include "StdAfx.h"
#include "Arrow.h"


Arrow::Arrow(void) {
	target[0] = target[1] = target[2] = 0;
}


Arrow::~Arrow(void)
{
}

void Arrow::setTarget(double x, double y, double z){
	target[0] = x;
	target[1] = y;
	target[2] = z;
}
void Arrow::setTarget(Vertex * v){
	target[0] = v->x;
	target[1] = v->y;
	target[2] = v->z;
}

void Arrow::draw(double x, double y, double z){
	glPushMatrix();
	double vec[3] = {
		target[0]-x,
		target[1]-y,
		target[2]-z
	};
	double angle = 180/M_PI*atan(vec[1]/vec[0]) + 90;
	if(vec[0] < 0) angle -=180;
	glTranslated(0,0,2);
	glRotated(angle, 0,0,1);
	angle =  - 180/M_PI*atan(vec[2]/sqrt(vec[0]*vec[0]+vec[1]*vec[1]));
	glRotated(angle, 1,0,0);
	glScaled(0.5,2,0.5);
	glRotated(90,1,0,0);
	glutSolidCone(1,1,10,10);
	glPopMatrix();
}
