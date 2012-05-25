#pragma once

#include "glIncl.h"
#include <iostream>
#include <math.h>
#include <cmath>

#include "Car.h"

#define M_PI                3.14159265358979323846 //FIXME

class Car;
class Camera{
public:
	Camera(double x, double y, double z,
		double rotX, double rotY, double rotZ);
	virtual ~Camera(){};
	void draw();

	bool isFollowing(){return followedCar == NULL ? false : true; }
	void follow(Car * car);
	void unfollow(){followedCar = NULL;}

	void increaseAngle(double & angle);
	void decreaseAngle(double & angle);
	
	void incZ(){z += distanceDelta;}
	void decZ(){z -= distanceDelta;}
	void incX(){x += distanceDelta;}
	void decX(){x -= distanceDelta;}
	void incY(){y += distanceDelta;}
	void decY(){y -= distanceDelta;}

	void rotXccw(){increaseAngle(rotX);}
	void rotXcw(){decreaseAngle(rotX);}
	void rotYccw(){increaseAngle(rotY);}
	void rotYcw(){decreaseAngle(rotY);}
	void rotZccw(){increaseAngle(rotZ);}
	void rotZcw(){decreaseAngle(rotZ);}
	void followRotZccw(){increaseAngle(followRotZ);}
	void followRotZcw(){decreaseAngle(followRotZ);}

	Car * getFollowedCar(){return followedCar; }

	double x, y, z;
	double rotX, rotY, rotZ;
	double followRotZ;

private:
	const double distanceDelta;
	const int degreeDelta;
	Car * followedCar;
};