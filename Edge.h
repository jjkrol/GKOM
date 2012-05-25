#pragma once
#include "glIncl.h"
#include "Vertex.h"

class Edge{
public:
	const Vertex * v1;
	const Vertex * v2;
	Edge(Vertex * v1, Vertex * v2);
};