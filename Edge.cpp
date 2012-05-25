#include "StdAfx.h"
#include "Edge.h"

using namespace std;

Edge::Edge(Vertex * v1, Vertex * v2):
v1(v1),v2(v2){
	v1->registerNeighbour(v2);
	v2->registerNeighbour(v1);
}