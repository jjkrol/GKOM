#include "StdAfx.h"
#include "Heightmap.h"


Heightmap::Heightmap(int mapSize, int stepSize, float scale, char * filename):
mapSize(mapSize), stepSize(stepSize), scale(scale), filename(filename){
}


Heightmap::~Heightmap(void) {
}
void Heightmap::load(){
	ilInit();
	iluInit();
    ilutRenderer(ILUT_OPENGL);
	ilLoadImage("textures/heightmap.jpg");
	image = ilGetData();
	heightmapBPP = ilGetInteger(IL_IMAGE_BPP);

	for (int y=0; y < mapSize; ++y){
		for (int x=0; x < mapSize; ++x)    {
			int pos = (y*mapSize+x)*heightmapBPP;
			heightMap[x][y] = image[pos];
		}
	}
    int i = 0, j = 0;
	float normalizer = 5;
	float x, y, z;
		float denominator = (float)(mapSize-stepSize);
	for ( i = 1; i < denominator; i += stepSize )
    for ( j = 1; j < denominator; j += stepSize ){
		MyPolygon p;
		p.v[0][0] = p.v[1][0] = i*scale;
		p.v[0][1] = p.v[3][1] = j*scale;
		p.v[2][0] = p.v[3][0] = (i + stepSize)*scale;
		p.v[1][1] = p.v[2][1] = (j + stepSize)*scale;
		p.v[0][2] = heightMap[i][j]/normalizer;
		p.v[1][2] = heightMap[i][j+stepSize]/normalizer;
		p.v[2][2] = heightMap[i+stepSize][j+stepSize]/normalizer;
		p.v[3][2] = heightMap[i+stepSize][j]/normalizer;

		p.n[0] = p.n[1] = p.n[2] = 0;
		for(int i = 0; i<4;++i){
			float * curr = p.v[i];
			float * next = p.v[(i+1)%4];
			p.n[0] += (curr[1] - next[1])*(curr[2]+next[2]);
			p.n[1] += (curr[2] - next[2])*(curr[0]+next[0]);
			p.n[2] += (curr[0] - next[0])*(curr[1]+next[1]);
		}
		p.t[0] = (float)i/denominator;
		p.t[1] = 1 - (float)j/denominator;
		p.t[2] = (float)(i+1)/denominator;
		p.t[3] = 1 - (float)(j+1)/denominator;
		heightMapCalculated[i][j] = p;
}
}

void Heightmap::draw(){
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslated(-70,-70,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, heightmapTexture[0]);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin( GL_QUADS ); 
		float denominator = (float)(mapSize-stepSize);
	for (int i = 1; i < denominator; i += stepSize )
    for (int j = 1; j < denominator; j += stepSize ){
		MyPolygon p = heightMapCalculated[i][j];
		glNormal3fv(p.n); 
		glTexCoord2f(p.t[0], p.t[1]);
        glVertex3fv(p.v[0]);
		glTexCoord2f(p.t[0], p.t[3]);
        glVertex3fv(p.v[1]);  
		glTexCoord2f(p.t[2], p.t[3]);
        glVertex3fv(p.v[2]);
		glTexCoord2f(p.t[2], p.t[1]);
        glVertex3fv(p.v[3]); 
    }
glEnd();
glDisable(GL_TEXTURE_2D);
 glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Reset The Color
 glPopMatrix();
}

void Heightmap::setTexture(char * filename){
	texture = filename;
	ilInit();
	iluInit();
    ilutRenderer(ILUT_OPENGL);
	heightmapTexture[0] = ilutGLLoadImage(texture);
}