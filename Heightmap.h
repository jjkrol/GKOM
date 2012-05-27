#pragma once
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "ilu.lib")
#pragma comment(lib, "ilut.lib")

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>

class Heightmap {
public:
	Heightmap(int mapSize, int stepSize, float scale, char * filename);
	~Heightmap(void);
	void setTexture(char * filename);
	void draw();
	void load();

const int stepSize;
const float scale;
const int mapSize;

char * filename;
char * texture;
ILubyte * image;
int heightmapBPP;
float heightMap[1025][1025]; //FIXME change!
UINT heightmapTexture[1];
struct MyPolygon{
	float v[4][3]; //vertexes
	float t[4]; //texture coords
	float n[3]; // normals
 };
MyPolygon heightMapCalculated[1025][1025];
};

