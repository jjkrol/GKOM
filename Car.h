#pragma once

#include "gkom.h"

class Road;
class Vertex;
class Arrow;
enum Direction;

class Car
{
public:
	Car(Road * road, int id);
	~Car(void);
	void draw();
	void move();
	void getCoords(double coords[3]);
	void getCameraCoords(double coords[3]);
	void getVelocityVector(double vel[3]);
	Direction getCurrentDirection(){return currentDirection;};
	void faster(){speed += 0.01; if(speed > 0.4) speed = 0.4;}
	void slower(){speed -= 0.01; if(speed < 0) speed = 0;}
	void toggleArrow(){showArrow = !showArrow;}
	void toggleNextPoint(){showNextPoint= !showNextPoint;}
	void getPossibleDirections(bool * table);
	void setNextTurn(Direction dir);
	void setNewTarget();
	Arrow * getArrow(){return arr;}
	Direction getNextTurn();
	double getRotY();
	double getRotZ();
	const int id;

private:
	Vertex * nextVertex;
	Vertex * currentVertex;
	Vertex * targetVertex;
	Arrow * arr;
	Direction currentDirection;
	Direction nextTurn;
	double turningTargets[200][3];
	int turningCount;
	double target[3];
	bool turning;
	double speed;
	double height;
	double x, y, z;
	double rotX, rotY, rotZ;
	double vec[3]; //velocity vector
	bool showArrow;
	bool showNextPoint;
	Road * road;
	void calculateDirection(Direction & dir);
	void chooseNextVertex();
	const double baseEp;
	double ep;
};

