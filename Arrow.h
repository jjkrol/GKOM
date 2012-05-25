#pragma once
#include "glIncl.h"
class Arrow
{
public:
	Arrow(void);
	~Arrow(void);
void draw(double x, double y, double z);
void setTarget(double x, double y, double z);
void setTarget(Vertex * v);


private:
	double target[3];
};

