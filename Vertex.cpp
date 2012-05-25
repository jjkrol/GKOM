#include "StdAfx.h"
#include "Vertex.h"

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