#pragma once
#include "gkom.h"

class Building
{
public:
	Building(double size, double height);
	~Building(void);
	void draw();

private:
	UINT texture[1];
	double size, height;
	double red, green, blue;
};

