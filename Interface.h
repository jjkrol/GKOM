#pragma once

#include "glIncl.h"
#include "Camera.h"
#include "Car.h"

class Car;
class Camera;

class Interface {
public:
	Interface(Camera * cam);
	~Interface(void);
	void draw();
private:
	Camera * cam;
	void drawTextButton(char * text);
}; 

