#pragma once
#include "gkom.h"

class Building
{
public:
	Building(double size, double height, UINT texture[1]);
	~Building(void);
	void draw();

private:
	UINT * texture;
	double size, height;
	double red, green, blue;
};

