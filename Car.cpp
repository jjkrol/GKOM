#include "StdAfx.h"
#include "Car.h"

using namespace std;
void Car::setNewTarget(){
	targetVertex = road->getRandomVertex();
	arr->setTarget(targetVertex);
}
Car::Car(Road * road, int id):road(road),id(id),ep(0.01),baseEp(0.01),
	height(1.5), speed(0.05), turning(true), showArrow(true),
showNextPoint(true), nextTurn(NONE), rotX(90), rotY(0), rotZ(0){
	arr = new Arrow();
	currentVertex = road->getRandomVertex();
	x = currentVertex->x;
	y = currentVertex->y;
	z = currentVertex->z;
	setNewTarget();


	nextVertex = currentVertex->getRandomNeighbour(currentVertex);
	currentVertex->getNeighbourPeriphery(nextVertex, target, road->width/2);
			calculateDirection(currentDirection);
				turningTargets[0][0] = target[0];
				turningTargets[0][1] = target[1];
				turningTargets[0][2] = target[2];

}

void Car::getCoords(double coords[3]){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}

void Car::getCameraCoords(double coords[3]){
	coords[0] = x-cos(rotZ*M_PI/180)*3;
	coords[1] = y-sin(rotZ*M_PI/180)*3;
	//coords[2] = z+sin((rotY)*M_PI/180)*4+4; turn on when camera rotY is ready
	coords[2] = z + 4;
}

void Car::getVelocityVector(double vel[3]){
	vel[0] = vec[0];
	vel[1] = vec[1];
	vel[2] = vec[2];
}
void Car::setNextTurn(Direction dir){
	if(currentDirection < 4){
	nextTurn = (Direction)((dir+currentDirection)%4);
		}
}

Car::~Car(void){
}
void Car::getPossibleDirections(bool * table){
	nextVertex->getNeighboursDirections(table);
	if(currentDirection < 4)
		for(int i = 0; i < currentDirection; ++i){
			bool temp = table[0];
			table[0] = table[1];
			table[1] = table[2];
			table[2] = table[3];
			table[3] = temp;
		}
}
Direction Car::getNextTurn(){
	if(nextTurn == NONE)
		return nextTurn;
	int difference = nextTurn - currentDirection;
	if(difference > 0)
		return (Direction) difference;
	else
		return (Direction) (4 + difference);
}
void Car::move(){
	double eps = speed*2;
	ep = baseEp * speed/0.05;
	if(	abs(targetVertex->x-x) < road->width &&
		abs(targetVertex->y-y) < road->width &&
		abs(targetVertex->z-z) < road->width){
		setNewTarget();
		std::cout<<"Udalo sie"<<std::endl;
	}
	vec[1] = target[1]-y;
	vec[2] = target[2]-z;

	vec[0] = target[0]-x;
	vec[1] = target[1]-y;
	vec[2] = target[2]-z;
	//create velocity vector
	if(abs(vec[0]) < eps && abs(vec[1]) < eps && abs(vec[2]) < eps){ //we got to the point
			//cout<<"Got to the point"<<endl;

		if(turning){
			if(turningCount >= 0){
				target[0] = turningTargets[turningCount][0];
				target[1] = turningTargets[turningCount][1];
				target[2] = turningTargets[turningCount][2];
				turningCount -= 1;
			}
			else{
				nextVertex->getNeighbourPeriphery(currentVertex, target, road->width/2);
				turning = false;
			}
		}
		else{
			enum Turn {RIGHT, LEFT, STRAIGHT};
			Direction ridingDir;
			Turn ridingTurn;
			calculateDirection(ridingDir);

			chooseNextVertex();
			currentVertex->getNeighbourPeriphery(nextVertex, target, road->width/2);
			calculateDirection(currentDirection);

			turning = true;
			//get turning direction
			float dx = target[0] - x;
			float dy = target[1] - y;
			if(abs(dx) < ep || abs(dy) < ep){
				ridingTurn = STRAIGHT;
				turningTargets[0][0] = target[0];	
				turningTargets[0][1] = target[1];	
				turningTargets[0][2] = target[2];	
				turningCount = 0;
			}
			else{
				ridingTurn = LEFT;
			if(dx > 0 && dy > 0)
				if(ridingDir == NORTH)
					ridingTurn = RIGHT;
				else if(ridingDir == EAST)
					ridingTurn = LEFT;
				else
					cout<<"Blad! x y "<<endl;
			if(dx < 0 && dy > 0)
				if(ridingDir == NORTH)
					ridingTurn = LEFT;
				else if(ridingDir == WEST)
					ridingTurn = RIGHT;
				else
					cout<<"Blad! -x y"<<endl;
			if(dx < 0 && dy < 0)
				if(ridingDir == SOUTH)
					ridingTurn = RIGHT;
				else if(ridingDir == WEST)
					ridingTurn = LEFT;
				else
					cout<<"Blad! -x -y"<<endl;
			if(dx > 0 && dy < 0)
				if(ridingDir == EAST)
					ridingTurn = RIGHT;
				else if(ridingDir == SOUTH)
					ridingTurn = LEFT;
				else
					cout<<"Blad! x -y"<<endl;
			
			//for(turningCount = 0; turningCount < 2; turningCount++){
			//}
			float a = speed;
			int n = M_PI/(4*asin(a/(2*road->width/2)));
			if (n > 200) n = 200;

			float angleStep = (float)360/(float)n/(float)4;
			if(ridingTurn == LEFT)
				angleStep *= -1;
			float angle = angleStep;
			double prevPoint[3] = {x,y,z};

			for(int i = 0; i <= n; i++){
			float relY = cos(angle*M_PI/180)*a;
			float relX = sin(angle*M_PI/180)*a;

			float addX;
			float addY;
			
			if(ridingDir == NORTH){
				addY = relY;
					addX = relX;
			}
			if(ridingDir == SOUTH){
				addY = -relY;
					addX = -relX;
			}
			if(ridingDir == EAST){
				addX = relY;
					addY = -relX;
			}
			if(ridingDir == WEST){
				addX = - relY;
					addY = relX;
			}
			prevPoint[0] = turningTargets[n-i][0] = prevPoint[0]+addX;
			prevPoint[1] = turningTargets[n-i][1] = prevPoint[1]+addY;
			prevPoint[2] = turningTargets[n-i][2] = prevPoint[2];
			angle += angleStep;
			}

			turningCount = n;
			}
				target[0] = turningTargets[turningCount][0];
				target[1] = turningTargets[turningCount][1];
				target[2] = turningTargets[turningCount][2];
				turningCount -=1;
		}
		}
			if(showNextPoint){
			glPushMatrix();
				glTranslated(target[0],target[1],target[2]);
				glColor3f(1,0,0);
				glutSolidCube(0.3);
			glPopMatrix();
			}
	vec[0] = target[0]-x;
	vec[1] = target[1]-y;
	vec[2] = target[2]-z;
	//normalize
	double m = sqrt(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
	for(int i = 0; i < 3; i++)
		vec[i] = vec[i]/m * speed;
	x += vec[0];
	y += vec[1];
	z += vec[2];

	// rotations:
	// vertical
	double angle;
	double planarDirection = sqrt( pow(vec[0],2)+ pow(vec[1],2));
	angle = 180/M_PI*atan(vec[2]/planarDirection);
	rotY = - angle;
	//horizontal
	angle = 180/M_PI*atan(vec[1]/vec[0]);
	if(vec[0] < 0) angle -=180;
	rotZ = angle; 
	//are we moving closer?
}
double Car::getRotY(){
	return rotY;
}
double Car::getRotZ(){
	return rotZ;
}
void Car::draw(){
	glPushMatrix();
	//draw target
	glPushMatrix();
		glTranslated(targetVertex->x, targetVertex->y, targetVertex->z+1);
		glColor3f(0,1,0);
		glutSolidCube(2);
	glPopMatrix();

	glColor3f(1,0,0);
	glTranslated(x, y, z+height/4 * (abs(sin((rotX)*M_PI/180))+1));
	glPushMatrix();

	if(showArrow){
		arr->draw(x,y,z);
	}
	glLoadName(id);
	glRotated(rotZ, 0, 0, 1);
	glRotated(rotY, 0, 1, 0);
	glRotated(rotX, 1, 0, 0);
	glutSolidTeapot(height/2);
	glPopMatrix();
	glPopMatrix();
}

void Car::calculateDirection(Direction & dir){
	double directions[2] = {
	nextVertex->x - currentVertex->x,
	nextVertex->y - currentVertex->y};
			if(abs(directions[0]) > ep * speed/0.05)
				if(directions[0] > 0)
					dir = EAST;
				else
					dir = WEST;
			else
				if(directions[1] > 0)
					dir = NORTH;
				else
					dir = SOUTH;
}

void Car::chooseNextVertex(){
			Vertex * temp = nextVertex;
			bool possibleDirections[4];
			nextVertex->getNeighboursDirections(possibleDirections);
			if(nextTurn != NONE && possibleDirections[nextTurn]){
				nextVertex = nextVertex->getNeighbourByDirection(nextTurn);
				nextTurn = NONE;
			}
			else{
				nextVertex = nextVertex->getRandomNeighbour(currentVertex);
			}
			currentVertex = temp;

}