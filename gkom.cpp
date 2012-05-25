// gkom.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "gkom.h"

/*
	skrêcanie pod górê (p³ynne)
	czajnik na samochód
	oœwietlenie (normalne, intensywnosc)
	wiêcej pojazdów
	height mapa
	efekty?
	Ÿle skrêca kiedy jedzie za szybko
	enum do obiektow w interfejsie
*/

using namespace std;
const double vertexPerDimension = 30.0;
const int sizeX = 10;
const int sizeY = 10;
const int segmentSize = 10;
const int roadWidth = 4;
const int gridWidth = segmentSize + roadWidth;
const int segmentCount = 7;
float strength = 1;;
const int amountCars = 1;
const int maxAmountInfo = 3;
bool paused = true;

const double floorHeight = 10;
const int mapSize = 512;
UINT heightMap[mapSize*mapSize*3];
Interface * inter;

Building * buildings[segmentCount][segmentCount];

vector<Car*> cars;
Camera* cam;
Road * mainRoad;
UINT SkyboxTexture[6];	
UINT TextureArray[1];
UINT buildingTexture[2];
UINT roadTexture[1];
 
 int selected = 0;
 
 void glSelect(int x, int y);
 void gl_selall(GLint hits, GLuint *buff);
 void mouseClick();
 void mousedw(int x, int y, int but);
 void draw_block(float x, float y, float z);
 void list_hits(GLint hits, GLuint *names);

void generateBuildings(){
	for(int i = 0; i < segmentCount; i++)
		for(int j = 0; j < segmentCount; j++){
			buildings[i][j] = new Building(segmentSize,
				floorHeight + rand() % int(floorHeight*4),
				//10,
				buildingTexture);
		}
}

void generateRoads(){
	//roads
	mainRoad = new Road(roadWidth, roadTexture);
	mainRoad->addPart(0,0,0,0,1,0);	
	mainRoad->addPart(0,1,0,0,3,0);	
	mainRoad->addPart(0,1,0,2,1,0);
	mainRoad->addPart(2,1,0,2,2,0);
	mainRoad->addPart(2,2,0,1,2,0);
	mainRoad->addPart(1,1,0,1,3,0);
	mainRoad->addPart(1,3,0,0,3,0);


	//ring road
	//mainRoad->addPart(0,0,0,7,0,0);
	//mainRoad->addPart(7,0,0,7,7,0);
	//mainRoad->addPart(7,7,0,0,7,0);
	//mainRoad->addPart(0,7,0,0,0,0);

	mainRoad->addPart(2,2,0,2,3,0);
	mainRoad->addPart(2,3,0,3,3,0);

	//first to second
	mainRoad->addPart(0,2,0,2,2,1);
	mainRoad->addPart(3,3,0,4,3,1);
	mainRoad->addPart(3,4,1,3,3,0);

	//second floor
	mainRoad->addPart(1,1,1,2,1,1);
	mainRoad->addPart(2,1,1,2,2,1);
	mainRoad->addPart(2,2,1,1,2,1);
	mainRoad->addPart(1,2,1,1,1,1);
	mainRoad->addPart(4,3,1,4,4,1);
	mainRoad->addPart(4,4,1,3,4,1);
	mainRoad->addPart(2,1,1,4,1,1);
	mainRoad->addPart(4,1,1,4,3,1);

	//second to third
	mainRoad->addPart(4,3,1,5,3,2);
	mainRoad->addPart(3,4,1,3,5,2);

	//third
	mainRoad->addPart(3,5,2,4,5,2);
	mainRoad->addPart(4,5,2,4,3,2);
	mainRoad->addPart(4,3,2,5,3,2);

	//third to fourth
	mainRoad->addPart(5,3,2,5,4,3);
	mainRoad->addPart(4,5,3,3,5,2);

	//fourth
	mainRoad->addPart(5,4,3,4,4,3);
	mainRoad->addPart(4,4,3,4,5,3);

	//fourt to fifth
	mainRoad->addPart(5,4,3,5,5,4);
	mainRoad->addPart(5,5,4,4,5,3);

}

float getHeight(UINT *pHeightMap, int X, int Y) {
    unsigned int x = X % mapSize;               
    unsigned int y = Y % mapSize;
 
    if(!pHeightMap) return 0;
	float red = pHeightMap[(x+(y*mapSize))*3];   
	float green = pHeightMap[(x+(y*mapSize))*3+1];   
	float blue = pHeightMap[(x+(y*mapSize))*3+2];   
    float height = (red+green+blue)/3.0f;
	return height;
}
void loadTextures(){
	//JPEG_Texture(buildingTexture,"textures/highrise.jpg",  0);
	buildingTexture[0] = ilutGLLoadImage("textures/highrise.jpg");
	buildingTexture[1] = ilutGLLoadImage("textures/roof.jpg");
	roadTexture[0] = ilutGLLoadImage("textures/road.jpg");
	//Tex1 = ilutGLLoadImage("tex1.bmp");

	//loadHeightMap(heightMap, mapSize, "textures/heightmap.jpg");
	heightMap[0] = ilutGLLoadImage("textures/heightmap.jpg");
	JPEG_Skybox(SkyboxTexture,"textures/front.jpg",  SKYFRONT);
	JPEG_Skybox(SkyboxTexture,"textures/back.jpg",   SKYBACK);
	JPEG_Skybox(SkyboxTexture,"textures/left.jpg",   SKYLEFT);
	JPEG_Skybox(SkyboxTexture,"textures/right.jpg",  SKYRIGHT);
	JPEG_Skybox(SkyboxTexture,"textures/up.jpg",     SKYUP);
	JPEG_Skybox(SkyboxTexture,"textures/down.jpg",   SKYDOWN);
}
void init_objects(){
	  ilInit();
       iluInit();
        ilutRenderer(ILUT_OPENGL);
	cam = new Camera(30,-30,40,-60,0,0);
	loadTextures();
	/*Floor::initTextures();
	BuildingObject::initTexture();
	GroundFloor::generateGroundFloor();
	HigherFloor::generateHigherFloor();
	Buildings::generateBuildings();
	generateCars();*/

	generateBuildings();
	generateRoads();

	//cars
	for(int i = 0; i < amountCars; ++i){
		cars.push_back(new Car(mainRoad, i+10));
	}
//	cars.push_back(new Car(mainRoad, 2));
	//textures

//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void drawHeightmap(UINT pHeightMap[]){
const int stepSize = 1;
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslated(0,0,1);
    int i = 0, j = 0;
    float x, y, z;                        // Create Some Variables For Readability
	float normalizer = 10;
    if(!pHeightMap) return;                 // Make Sure Our Height Data Is Valid
	glBegin( GL_QUADS ); 
	for ( i = 0; i < (mapSize-stepSize); i += stepSize )
    for ( j = 0; j < (mapSize-stepSize); j += stepSize )
    {
        // Get The (X, Y, Z) Value For The Bottom Left Vertex
        x = i;                         
        y = j;                         
        z = (float)getHeight(pHeightMap, i, j )/normalizer; 
 //       SetVertexColor(pHeightMap, x, z);
 
        glVertex3f(x, y, z);            // Send This Vertex To OpenGL To Be Rendered
		//cout<<z<<" ";
        // Get The (X, Y, Z) Value For The Top Left Vertex
        x = i;                                     
        z = (float)getHeight(pHeightMap, i, j + stepSize )/normalizer; 
        y = j + stepSize ;                        
         
        // Set The Color Value Of The Current Vertex
  //      SetVertexColor(pHeightMap, x, z);
 
        glVertex3f(x, y, z);            // Send This Vertex To OpenGL To Be Rendered
 
        // Get The (X, Y, Z) Value For The Top Right Vertex
        x = i + stepSize;
        z = (float)getHeight(pHeightMap, i + stepSize, j + stepSize )/normalizer;
        y = j + stepSize ;
 
        // Set The Color Value Of The Current Vertex
        //SetVertexColor(pHeightMap, x, z);
         
        glVertex3f(x, y, z);            // Send This Vertex To OpenGL To Be Rendered
 
        // Get The (X, Y, Z) Value For The Bottom Right Vertex
        x = i + stepSize;
        z = (float) getHeight(pHeightMap, i + stepSize, j )/normalizer;
        y = j;
 
        // Set The Color Value Of The Current Vertex
        //SetVertexColor(pHeightMap, x, z);
 
        glVertex3f(x, y, z);            // Send This Vertex To OpenGL To Be Rendered
    }
glEnd();
 glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Reset The Color
}

void init(){
	   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
//GLuint filter;                      // Which Filter To Use
//GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
//GLuint fogfilter= 0;                    // Which Fog To Use
//GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};      // Fog Color
//
//glClearColor(0.5f,0.5f,0.5f,1.0f);          // We'll Clear To The Color Of The Fog ( Modified )
// 
//glFogi(GL_FOG_MODE, fogMode[fogfilter]);        // Fog Mode
//glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
//glFogf(GL_FOG_DENSITY, 0.01f);              // How Dense Will The Fog Be
//glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
//glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
//glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
////glEnable(GL_FOG);                   // Enables GL_FOG
//
//	GLfloat mat_ambient[]    = { 1.0, 1.0,  1.0, 1.0 };
//	GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
//
//	glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
//	glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
//	glMaterialf( GL_FRONT, GL_SHININESS, 50.0 );
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
	glShadeModel( GL_SMOOTH );
//
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
//
//	glEnable( GL_NORMALIZE );
//
//	glClearDepth(1);
//	//glDepthFunc( GL_LESS );
//	glDepthFunc(GL_LEQUAL);
//	glEnable( GL_DEPTH_TEST );


}

void drawSkybox(float x, float y, float z,
	float width, float height, float length) {
	// Center the Skybox around the given x,y,z position
	x = x - width/4;
	y = y - length/4;
	z = z;

	glEnable(GL_TEXTURE_2D);
	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+length,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+length, z+height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+length, z+height); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+length,		z);
	glEnd();

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,    z+height); 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,	z+height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,	z+height);	
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+length,	z+height); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+length,		z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+length,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+length,	z+height); 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,	z+height);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y, z+height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+length, z+height); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+length,	z+height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,	z+height);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+length,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+length,		z); 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void drawLightning(){


	GLfloat light0_diffuse[] = {strength, strength, strength, 1.0f};	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

	GLfloat light0_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	GLfloat light0_specular[] = {0.1, 0.1, 0.1};
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	GLfloat light0_position[] = {-10, 50, 100, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position );
	glEnable( GL_LIGHT0 );

	glEnable( GL_LIGHTING );

}

void drawGrid(){
	for(float i = -500; i <= 500; i += 5)	{
		glBegin(GL_LINES);
		glColor3ub(150, 190, 150);						
		glVertex3f(-500, i, 0);					
		glVertex3f(500, i, 0);
		glVertex3f(i, -500,0);							
		glVertex3f(i, 500,0);
		glEnd();
	}
}

void drawBuildings(int offsetX, int offsetY, int offsetZ){
	glPushMatrix();
	glTranslated(offsetX+segmentSize/2+roadWidth, offsetY+segmentSize/2+roadWidth, offsetZ);
	for(int i = 0; i < segmentCount; i++)
		for(int j = 0; j < segmentCount; j++){
			glPushMatrix();
			glTranslated(
				(segmentSize+roadWidth)*i,
				(segmentSize+roadWidth)*j,
				0);
			buildings[i][j]->draw();
			glPopMatrix();
		}
	glPopMatrix();
}

void drawCars(){
	vector<Car*>::iterator it;
	for(it = cars.begin(); it != cars.end(); it++){
		if(!paused) (*it)->move();
		(*it)->draw();
	}
}

void displayObjects(){
	glPushMatrix();
			glColor3f(1,1,1);    
		drawSkybox(0,0,0,
			gridWidth*segmentCount*4,
			floorHeight*10 ,
			gridWidth*segmentCount*4);
		drawLightning();
		//drawHeightmap(heightMap);
	glPushMatrix();

	glPopMatrix();
		//floor
		drawBuildings(0,0,0);
	
		mainRoad->draw();
		drawCars();
	glPopMatrix();

}

void display(){
	glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();
 
	glPushMatrix();
	init();
	inter = new Interface(cam);
	inter->draw();
	cam->draw();
	displayObjects();
 	glTranslatef(0.0, 0.0, -5.0);
 
 	glColor3f(1.0, 0.0, 0.0);
 	glLoadName(7); /* Overwrite the first name in the buffer */
 	draw_block(-0.3, 0, 8);
 
 	glColor3f(0.0, 1.0, 0.0);
 	glLoadName(14); /* Overwrite the first name in the buffer */
 	draw_block(0, 0, 10);
 
 	glColor3f(0.0, 0.0, 1.0);
 	glLoadName(21); /* Overwrite the first name in the buffer */
 	draw_block(0.3, 0, 12);
	

	glPopMatrix();
	glutSwapBuffers(); 

	//	updateCars();

}

void reshape(GLsizei w, GLsizei h) {
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications

	glMatrixMode (GL_PROJECTION); //set the matrix to projection

	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1500.0); //set the perspective (angle of sight, width, height, , depth)

	glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void keyboard(unsigned char key, int x, int y) {
	if (key=='5') cam->incZ();
	if (key=='0') cam->decZ();
	if (key=='6') cam->incX();
	if (key=='4') cam->decX();
	if (key=='8') cam->incY();
	if (key=='2') cam->decY();
	if (key=='l') 
		if(cam->isFollowing())
			cam->getFollowedCar()->faster();
	if (key=='k')
		if(cam->isFollowing())
			cam->getFollowedCar()->slower();
	if (key=='a')
		if(cam->isFollowing())
			cam->followRotZcw();
		else
			cam->rotZcw();
	if (key=='d')
		if(cam->isFollowing())
			cam->followRotZccw();
		else
			cam->rotZccw();

	if (key=='s') cam->rotXccw();
	if (key=='w') cam->rotXcw();

    if (key=='e') cam->rotYccw();
    if (key=='q') cam->rotYcw();

	if (key=='p') paused = !paused;

	if (key==' ')
		if(!cam->isFollowing())
			cam->follow(*cars.begin());
		else
			cam->unfollow();

	if(cam->isFollowing()){
		if (key=='\\')
				cam->getFollowedCar()->toggleArrow();
		if (key=='.')
				cam->getFollowedCar()->toggleNextPoint();
		
	}
	if(key == 'r')
		cam->setDefaults();

	//if (key==13){
	//	//x -= (amountX/2)*sizeX -targetFollowed->x;
	//	//y -= (amountY/2)*sizeY -targetFollowed->y;
	//	//isFollowing = false;
	//}

	if (key=='+')
		strength +=0.1;	
	if (key=='-')
		strength -=0.1;

    if (key==27)
		exit(0);
}

void keyboardSpecial(int key, int x, int y){
	if(cam->isFollowing()){
		if (key==GLUT_KEY_UP)
			cam->getFollowedCar()->setNextTurn(NORTH);
		if (key==GLUT_KEY_LEFT)
			cam->getFollowedCar()->setNextTurn(WEST);
		if (key==GLUT_KEY_DOWN)
			cam->getFollowedCar()->setNextTurn(SOUTH);
		if (key==GLUT_KEY_RIGHT)
			cam->getFollowedCar()->setNextTurn(EAST);
	}
}

extern "C" {
	FILE* _iob = NULL;
}

 void mouseClick(int button, int state, int x, int y){
    if 	((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		mousedw(x, y, button);
	}
 }
 
 void mousedw(int x, int y, int but){
 	printf("Mouse button %d pressed at %d %d\n", but, x, y);
 	glSelect(x,glutGet(GLUT_WINDOW_HEIGHT)-y); //Important: gl (0,0) ist bottom left but window coords (0,0) are top left so we have to change this!
 }
 
 void glSelect(int x, int y){
 	GLuint buff[64] = {0};
 	GLint hits, view[4];
 	int id;
 
 	glSelectBuffer(64, buff);
 	glGetIntegerv(GL_VIEWPORT, view);
 	glRenderMode(GL_SELECT);
 	glInitNames();
 	glPushName(0);
 	glMatrixMode(GL_PROJECTION);
 	glPushMatrix();
 		glLoadIdentity();
 		gluPickMatrix(x, y, 1.0, 1.0, view);
		gluPerspective (60,
			(GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT),
			1.0, 1500.0); //set the perspective (angle of sight, width, height, , depth)
 		glMatrixMode(GL_MODELVIEW);
 		glutSwapBuffers();
 		display();
 		glMatrixMode(GL_PROJECTION);
 	glPopMatrix();
 	hits = glRenderMode(GL_RENDER);

	//get id form buffer
   unsigned int i, j;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   printf ("hits = %d\n", hits);
   ptr = (GLuint *) buff;
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }
	  
	  ptr += names+2;
	}
  ptr = ptrNames;

  //get clicked Car
  int clickedId = *ptr;
  cout<<clickedId<<endl;
  if(clickedId >= 10){
  vector<Car*>::iterator it;
	for(it = cars.begin(); it != cars.end(); it++){
		if((*it)->id == clickedId)
			cam->follow(*it);
	}
  }
  else{
	  inter->objectClicked(clickedId);
  }
 
 	glMatrixMode(GL_MODELVIEW);
 }
 
 void draw_block(float x, float y, float z){
 	glPushMatrix();
 		glTranslatef(x, y, z);
 		glutSolidCube(1.0);
 	glPopMatrix();
 }
 

#define WI 1024
#define HE 768
 int main(int argc, char **argv) {
srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(WI, HE);
    glutInitWindowPosition(0, 0);
	glutCreateWindow( "GKOM" );
    glutDisplayFunc(display);
	glutReshapeFunc( reshape );
    glutMouseFunc(mouseClick);
	glutKeyboardFunc (keyboard); 
	glutSpecialFunc (keyboardSpecial); 
	glutIdleFunc( display );
 
	init_objects();
    init();
 
    glutMainLoop();
 
 	return EXIT_SUCCESS;
 }