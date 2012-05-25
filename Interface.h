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
	void objectClicked(int id);
private:
	Camera * cam;
	Car * followedCar;
	void drawTextButton(char * text, bool state);
	void setPosition();
	void drawSpeedometer();
	void drawDirectionChooser();
	void drawToggleButtons();
}; 

