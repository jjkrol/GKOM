#pragma once
#include <set>
#include <map>
#include <iostream>
#include <iterator>
#include "glIncl.h"

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



class Edge{
public:
	const Vertex * v1;
	const Vertex * v2;
	Edge(Vertex * v1, Vertex * v2);

};



class Road
{
public:
	Road(float width);
	~Road(void);
	void addPart(int x1, int y1, int z1, int x2, int y2, int z2);
	void print();
	void draw();
	Vertex * getRandomVertex();
	const double width;
private:

	int segmentSize, floorHeight;
	std::set<Vertex*> vertices;
	std::set<Edge*> edges;

	void addUnitPart(int x1, int y1, int z1, int x2, int y2, int z2);

};
