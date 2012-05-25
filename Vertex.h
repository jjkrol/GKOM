#pragma once
#include "glIncl.h"
#include <map>
#include <set>

enum Direction {NORTH, WEST, SOUTH, EAST, NONE};

class Vertex{
public:
	const double x;
	const double y;
	const double z;
	Vertex(double x, double y, double z);
	void registerNeighbour(Vertex * v);
	void printVertex() const;
	Vertex * getRandomNeighbour(Vertex * omit);
	Vertex * getNeighbourByDirection(Direction dir);
	void printNeighbours() const;
	void getNeighbourPeriphery(Vertex * v, double coords[3], double eps);
	bool hasNeighbour(Direction dir);
	bool canBeRegistered(Vertex * v);
	void getNeighboursDirections(bool * table);
	Direction getNeighbourLocation(Vertex * v);
	std::map<Direction, Vertex*> getNeighboursMap();

private:
	std::set<Vertex*> neighbours;
	std::map<Direction, Vertex*> neighboursMap;
};
