#include "stdafx.h"
#include "camera.h"

using namespace std;

// double followingRadius = 30;
// double followingHeight = 10;

double followingAngle = 0;
void Camera::follow(Car * car){
	cout<<"Following mode"<<endl;
	followedCar = car;
}

Camera::Camera(double x, double y, double z,
	double rotX, double rotY, double rotZ):
x(x), y(y), z(z),
	rotX(rotX), rotY(rotY), rotZ(rotZ),followRotZ(0),
	distanceDelta(1), degreeDelta(3), followedCar(NULL){

}

void Camera::increaseAngle(double & angle){
	angle += degreeDelta;
	if(angle > 360) angle -= 360;
}
void Camera::decreaseAngle(double & angle){
	angle -= degreeDelta;
	if(angle < 0) angle += 360;
}
void Camera::draw(){
	if(followedCar == NULL){
	glRotatef(rotY,0,1,0);
	glRotatef(rotX,1,0,0);  
	glRotatef(rotZ,0,0,1);
	glTranslated(-x,-y,-z); 
	}
	else{
		double coords[3];
		double velocity[3];
		followedCar->getCameraCoords(coords);
		followedCar->getVelocityVector(velocity);
			glRotatef(rotX,1,0,0);  
			glRotatef(rotY,0,1,0);
			glRotatef(-followedCar->getRotZ() +90 +followRotZ ,0,0,1);
	glTranslated(-coords[0],-coords[1],-coords[2]); 
	}
}