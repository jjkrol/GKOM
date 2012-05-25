#include "StdAfx.h"
#include "road.h"

using namespace std;
Vertex::Vertex(double x, double y, double z):
x(x), y(y), z(z){
}

void Vertex::registerNeighbour(Vertex * v){
	Direction dir = getNeighbourLocation(v);
	if(canBeRegistered(v)){
		neighboursMap[dir] = v;
		neighbours.insert(v);
	}
	else{
		cout<<"Blad, juz jest taki sasiad!"<<endl;
	}
}

bool Vertex::hasNeighbour(Direction dir){
	return neighboursMap.count(dir);
}
std::map<Direction, Vertex*> Vertex::getNeighboursMap(){
	return neighboursMap;
}
Direction Vertex::getNeighbourLocation(Vertex * v){
	float ep = 0.01,
		dx = v->x - x,
		dy = v->y - y;

	if(abs(dx) > ep)
		if(dx > 0)
			return EAST;
		else
			return WEST;
	else
		if(dy > 0)
			return NORTH;
		else
			return SOUTH;
	}

bool Vertex::canBeRegistered(Vertex * v){
	Direction dir = getNeighbourLocation(v);
	return !hasNeighbour(dir);
}
void Vertex::getNeighbourPeriphery(Vertex * v, double coords[3], double eps){
	//is it a neighbour
	double dx = v->x - x;
	double xOff = ((dx > 0) - (dx < 0))*eps;
	double dy = v->y - y;
	double yOff = ((dy > 0) - (dy < 0))*eps;
	coords[0] = x+xOff;
	coords[1] = y+yOff;
	coords[2] = z;

}
void Vertex::printVertex() const {
	cout<<"Vertex: "
		<<x<<" "
		<<y<<" "
		<<z<<" ";
}

void Vertex::printNeighbours() const{
	set<Vertex *>::iterator it;
	for(it = neighbours.begin(); it!=neighbours.end(); it++){
		cout<<"\t";
		(*it)->printVertex();
		cout<<endl;
	}
}

Vertex * Vertex::getNeighbourByDirection(Direction dir){
	return neighboursMap[dir];
}

Vertex * Vertex::getRandomNeighbour(Vertex * omit){
	set<Vertex *> tempNeigh;
	std::copy(neighbours.begin(), neighbours.end(),
	inserter( tempNeigh, tempNeigh.begin() ) );
	if(tempNeigh.size() > 1)
		tempNeigh.erase(omit);
	set<Vertex *>::iterator it(tempNeigh.begin());
	advance(it, rand() % tempNeigh.size());
	return *it;
}
void Vertex::getNeighboursDirections(bool * table){
	for(int i = 0; i < 4; ++i){
		table[i] = false;
	}
	map<Direction,Vertex*>::iterator it;
	for(it = neighboursMap.begin(); it != neighboursMap.end(); ++it){
		table[(*it).first] = true;
	}
}

Edge::Edge(Vertex * v1, Vertex * v2):
v1(v1),v2(v2){
	v1->registerNeighbour(v2);
	v2->registerNeighbour(v1);
}

Road::Road(float width, UINT * roadTexture):
width(width),texture(roadTexture){
	segmentSize = 10;
	floorHeight = 10;
}


Road::~Road(void)
{
	//delete vertices and edges
}

void Road::addPart(int x1, int y1, int z1, int x2, int y2, int z2){
	//validate data
	if(x1 == x2 && y1 == y2){
		cout << "The same point!" << endl;
		return;
	}
	if(x1 != x2 && y1 != y2){
		cout << "Diagonal!" << endl;
		return;
	}
		cout << "Adding edge "
		<< x1 << " " << y1 << " " << z1 << " "
		<< x2 << " " << y2 << " " << z2 << " " << endl;
	if(x1 == x2){ // vertical
		if(abs((y1-y2)) > 1){
			int step = y1<y2 ? 1 : -1;
			int temp = y1;
			y1 -= step;
			while(temp != y2){
				temp+=step;
				y1 +=step;
				addUnitPart(x1,y1,z1,x2,temp,z2);
				z1 = z2;
			}
		}
		else{
			addUnitPart(x1,y1,z1,x2,y2,z2);
		}
	}

	if(y1 == y2){ // horizontal
		if(abs((x1-x2)) > 1){
			int step = x1<x2 ? 1 : -1;
			int temp = x1;
			x1 -= step;
			while(temp != x2){
				temp+=step;
				x1 +=step;
				addUnitPart(x1,y1,z1,temp,y2,z2);
				z1 = z2;
			}
		}
		else{
			addUnitPart(x1,y1,z1,x2,y2,z2);
		}
	}


}

void Road::addUnitPart(int x1, int y1, int z1, int x2, int y2, int z2){
	Vertex * v1 = NULL;
	Vertex * v2 = NULL;
	set<Vertex*>::iterator it;
	double f = segmentSize+width; //factor
	double
		dx1=x1*f+width/2, dy1=y1*f+width/2, dz1=z1*floorHeight +0.01,
		dx2=x2*f+width/2, dy2=y2*f+width/2, dz2=z2*floorHeight +0.01;
	
	//find if any of the vertices is already in the road
	for(it = vertices.begin(); it!=vertices.end(); it++){
		if((*it)->x == dx1 && (*it)->y == dy1 && (*it)->z == dz1)
			v1 = *it;
		if((*it)->x == dx2 && (*it)->y == dy2 && (*it)->z == dz2)
			v2 = *it;
	}
	bool addv1 = false, addv2 = false;
	if(v1 == NULL){
		v1 = new Vertex(dx1, dy1, dz1);
		addv1 = true;
	}
	if(v2 == NULL){
		v2 = new Vertex(dx2, dy2, dz2);
		addv2 = true;
	}
	if(v1->canBeRegistered(v2) && v2->canBeRegistered(v1)){
		if(addv1) vertices.insert(v1); //TODO are duplicates inserted?
		if(addv2) vertices.insert(v2);
		cout << "\t Adding unit edge "
		<< x1 << " " << y1 << " " << z1 << " "
		<< x2 << " " << y2 << " " << z2 << " " << endl;
	edges.insert(new Edge(v1, v2));
	}
	else{
		cout<<"Nie dodaje!"<<endl;
		//TODO delete created vertexes
	}
}

void Road::draw(){
	//draw joints
	double d = width/2;
	set<Vertex*>::iterator vertIt;
	for(vertIt = vertices.begin(); vertIt != vertices.end(); vertIt++){
		Vertex * v = (*vertIt);
		double x = v->x;
		double y = v->y;
		double z = v->z;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);  
		glColor3f(0.5f,0.5f,1.0f); 
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x-d, y+d, z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+d, y+d, z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+d, y-d, z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x-d, y-d, z);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	}

	//draw connections
	set<Edge*>::iterator edgeIt;
	for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++){
		const Vertex * v1 = (*edgeIt)->v1;
		const Vertex * v2 = (*edgeIt)->v2;
		double	x1=0,x2=0,x3=0,x4=0,
				y1=0,y2=0,y3=0,y4=0,
				z1=0,z2=0,z3=0,z4=0;
		float width, length, ratio;
		if(v1->x == v2->x){ //along y axis
		x1 = v1->x-d; //top left
		x2 = v1->x+d; //top right
		x3 = v2->x+d; //bottom right
		x4 = v2->x-d; //bottom left
		y1 = y2 = v1->y;
		y3 = y4 = v2->y;

			if(v1->y > v2->y){
				y1 = y2 -= d;
				y3 = y4 += d;
			}
			else{
				y1 = y2 += d;
				y3 = y4 -= d;
			}
			width = abs(x1-x2);
			length = abs(y1-y3);
		}

		if(v1->y == v2->y){ //along x axis
		x1 = x2 = v1->x; //top left, bottom left
		x3 = x4 = v2->x; //bottom right, top right
		y1 = v1->y+d;
		y2 = v1->y-d;
		y3 = v2->y-d;
		y4 = v2->y+d;

			if(v1->x > v2->x){
				x1 = x2 -= d;
				x3 = x4 += d;
			}
			else{
				x1 = x2 += d;
				x3 = x4 -= d;
			}
			width = abs(y1-y2);
			length = abs(x1-x3);
		}

		z1 = z2 = v1->z;
		z3 = z4 = v2->z;
	ratio = length/width;	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);  
		glColor3f(0.5f,0.5f,1.0f);    
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 1.0f*ratio);
			glVertex3f(x1,y1,z1);
			glTexCoord2f(1.0f, 1.0f*ratio);
			glVertex3f(x2,y2,z2);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x3,y3,z3);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x4,y4,z4);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	}
}

void Road::print(){
	//draw joints
	set<Vertex*>::iterator vertIt;
	for(vertIt = vertices.begin(); vertIt != vertices.end(); vertIt++){
		(*vertIt)->printVertex();
		cout<<endl;
		(*vertIt)->printNeighbours();
		cout<<endl;
	}

	//draw connections
	set<Edge*>::iterator edgeIt;
	for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++){
		cout<<"Edge: ";
		(*edgeIt)->v1->printVertex();
		cout<<" - ";
		(*edgeIt)->v2->printVertex();
		cout<<endl;

	}
}

Vertex * Road::getRandomVertex(){
	set<Vertex *>::iterator it(vertices.begin());
	advance(it, rand() % vertices.size());
	return *it;
}