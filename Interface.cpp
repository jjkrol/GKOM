#include "StdAfx.h"
#include "Interface.h"

using namespace std;

LPVOID glutFonts[7] = { 
	GLUT_BITMAP_9_BY_15, 
	GLUT_BITMAP_8_BY_13, 
	GLUT_BITMAP_TIMES_ROMAN_10, 
	GLUT_BITMAP_TIMES_ROMAN_24, 
	GLUT_BITMAP_HELVETICA_10, 
	GLUT_BITMAP_HELVETICA_12, 
	GLUT_BITMAP_HELVETICA_18
};

Interface::Interface(Camera * cam):cam(cam) {
}


Interface::~Interface(void) {
}
void glutPrint(double x, double y, double z,
	LPVOID font, char* text,
	double r, double g, double b, double a) { 
		if(!text || !strlen(text)) return; 
		bool blending = false; 
		if(glIsEnabled(GL_BLEND)) blending = true; 
		glEnable(GL_BLEND); 
		glColor4d(r,g,b,a); 
		glRasterPos3d(x,y,z); 
		while (*text) { 
			glutBitmapCharacter(font, *text); 
			text++; 
		} 
		if(!blending) glDisable(GL_BLEND); 
}  
void Interface::setPosition(){
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glTranslatef(0.0, 0.0, -1);
	glTranslated(-0.76*(float)windowWidth/(float)1024,
		-0.55*(float)windowHeight/(float)768,0);

}
void Interface::drawSpeedometer(){
	float speed = followedCar->getSpeed()*100;
	char* s = new char[5];
	sprintf(s, "%.4g", speed );  
	glutPrint(0.2f, 0.25f, 0.0f , glutFonts[6], s, 0.0f, 0.0f, 1.0f, 0.5f);
	delete [] s;
}
void Interface::draw(){
	glPushMatrix();
	glDisable(GL_LIGHTING);
	setPosition();
	glLoadName(9);
	if(cam->isFollowing()){
		followedCar = cam->getFollowedCar();
		drawSpeedometer();
		drawDirectionChooser();
		drawToggleButtons();

		glPopMatrix();
	}
	else{
		followedCar = NULL;
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();
}


void Interface::drawTextButton(char* text, bool state){
	glPushMatrix();
	if(state)
		glColor3f(0.0, 0.8, 0.0);
	else
		glColor3f(0.8, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0,0,-0.01f);
	glVertex3f(0,0.2,-0.01f);
	glVertex3f(0.2,0.2,-0.01f);
	glVertex3f(0.2,0,-0.01f);
	glEnd();
	glutPrint(0.1f, 0.1f, 0.0f , glutFonts[6], text, 0.0f, 0.0f, 1.0f, 0.5f);
	glPopMatrix();
}

void Interface::drawDirectionChooser(){
	bool dirs[4];	
	followedCar->getPossibleDirections(dirs);
	glPushMatrix();
	double col[4][3]  = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	Direction dir;
	if((dir = followedCar->getNextTurn()) != NONE){
		col[dir][0] = 0.0f;
		col[dir][1] = 1.0f;
		col[dir][2] = 0.0f;
	}

	if(dirs[0])
		glutPrint(0.2f, 0.2f, 0.0f , glutFonts[6], "gora", col[0][0], col[0][1], col[0][2], 1.0f);
	if(dirs[1])
		glutPrint(0.1f, 0.15f, 0.0f , glutFonts[6], "lewo", col[1][0], col[1][1], col[1][2], 1.0f);
	if(dirs[2])
		glutPrint(0.2f, 0.1f, 0.0f , glutFonts[6], "dol", col[2][0], col[2][1], col[2][2], 1.0f);
	if(dirs[3])
		glutPrint(0.3f, 0.15f, 0.0f , glutFonts[6], "prawo", col[3][0], col[3][1], col[3][2], 1.0f);
}

void Interface::drawToggleButtons(){
	glPushMatrix();
	glTranslated(1,0.1,0);
	glLoadName(2);
	drawTextButton("K", followedCar->isShowArrow());
	glTranslated(0.2,0,0);
	glLoadName(3);
	drawTextButton("P", followedCar->isShowNextPoint());
	glPopMatrix();
}

void Interface::objectClicked(int id){
	switch(id){
	case 2:
		followedCar->toggleArrow();
		break;
	case 3:
		followedCar->toggleNextPoint();
		break;
	}
}