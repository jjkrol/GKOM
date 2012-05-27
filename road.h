#pragma once
#include <set>
#include <map>
#include <iostream>
#include <iterator>
#include "glIncl.h"
#include "Vertex.h"
#include "Edge.h"

class Vertex;
class Edge;

class Road
{
public:
	Road(float width, char * roadTexture);
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
	UINT roadTexture[1];
	bool hasTextures;

};
