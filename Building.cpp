#include "StdAfx.h"
#include "Building.h"


Building::Building(double size, double height):size(size), height(height){
	red = (double)rand()/(double)RAND_MAX;
	green = (double)rand()/(double)RAND_MAX;
	blue = (double)rand()/(double)RAND_MAX;
	//JPEG_Texture(texture,"textures/highrise.jpg",  0);

}

Building::~Building(void){
}

void Building::draw(){
	glPushMatrix();
	glTranslated(0,0,height/2);
	//glScaled(size,size,height);
	glColor3f(red, green, blue);
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -height/2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = height/2;

  for (i = 5; i >= 0; i--) {

	//glBindTexture(GL_TEXTURE_2D, texture[0]);  
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
	glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(&v[faces[i][0]][0]);
	 glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(&v[faces[i][1]][0]);
	 glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(&v[faces[i][2]][0]);
	 glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }

	glPopMatrix();
}