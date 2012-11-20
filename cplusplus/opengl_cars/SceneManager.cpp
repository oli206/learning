#include <windows.h>											// Header File For Windows
#include <math.h>												// Header File For Windows Math Library     ( ADD )
#include <stdio.h>    //  Standard Input\Output C Library
#include <stdarg.h>   //  To use functions with variables arguments
#include <stdlib.h>   //  for malloc
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"												// Header File For The Glaux Library
#include "GLAux.h"												// Header File For The Glaux Library
#include "SceneObject.hpp"
#include "SceneCamera.hpp"
#include "SceneLight.hpp"
#include "SceneManager.hpp"

#define BACK_ID      0                    
#define FRONT_ID   1                    
#define BOTTOM_ID   2                       
#define TOP_ID      3                       
#define LEFT_ID      4                       
#define RIGHT_ID   5   

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;

//-------------------------------------------------------
// SceneManager implementation
//-------------------------------------------------------

SceneManager::SceneManager(GLfloat _mapSize){	

	mapSize = _mapSize;
	cameraSelected = 0;
	numberOfCameras = 0;
	numberOfObjects = 0;
	numberOfLights = 0;

	trafficLightOn = 0;

	transparencyEnabled = false;
	fogEnabled = false;

	fire1 = false;
	fire2 = false;

	createObjects();
	createCameras();
	createLights();

	loadSceneTextures();
}

SceneManager::~SceneManager(){
	
}

GLvoid SceneManager::addObject(SceneObject *_object){

	if (numberOfObjects < maxObjects)
	{
		objects[numberOfObjects] = _object;
		numberOfObjects++;
	}
}

GLvoid SceneManager::addCamera(SceneCamera * _camera){

	if (numberOfCameras < maxCameras)
	{
		cameras[numberOfCameras] = _camera;
		numberOfCameras++;
	}
}

GLvoid SceneManager::addLight(SceneLight * _light){

	if (numberOfLights < maxLights)
	{
		lights[numberOfLights] = _light;
		numberOfLights++;
	}
}

GLvoid SceneManager::selectCamera(int _index){

	if (_index <= numberOfCameras && numberOfCameras > 0)
	{
		cameraSelected = _index;
	}
	
}

SceneCamera * SceneManager::getCameraSelected(){

	return cameras[cameraSelected];
}

int SceneManager::getCameraSelectedIndex(){

	return cameraSelected;
}

GLvoid SceneManager::positionCamera(){

	glRotatef(-getCameraSelected()->getPitchAngle(),		1.0f,0.0f,0.0f);			
	glRotatef(-getCameraSelected()->getYawAngle(),		0.0f,1.0f,0.0f);                
	glTranslatef(-getCameraSelected()->getPosition().x, -getCameraSelected()->getPosition().y-50.0f, -getCameraSelected()->getPosition().z);		
}

GLvoid SceneManager::positionCamera(int _index){

	glRotatef(-cameras[_index]->getPitchAngle(),		1.0f,0.0f,0.0f);			
	glRotatef(-cameras[_index]->getYawAngle(),		0.0f,1.0f,0.0f);                
	glTranslatef(-cameras[_index]->getPosition().x, -cameras[_index]->getPosition().y-50.0f, -cameras[_index]->getPosition().z);		
}

int SceneManager::getNumberOfCameras(){
	return numberOfCameras;
}

int SceneManager::getNumberOfLights(){
	return numberOfLights;
}

int SceneManager::getNumberOfObjects(){
	return numberOfObjects;
}

SceneCamera * SceneManager::getCamera(int _index){

	return cameras[_index];
}

SceneObject * SceneManager::getObject(int _index){

	return objects[_index];
}

SceneLight * SceneManager::getLight(int _index){

	return lights[_index];
}

bool SceneManager::handleMouse(bool _rotateCamera, bool _isDragging, bool _moveForward, GLfloat _yawAngle, GLfloat _pitchAngle){	

	bool toReturn = true;
	int cameraSelectedIndex = getCameraSelectedIndex();	

	if ( _rotateCamera && _isDragging)	
	{			
		getCameraSelected()->modifyYaw(_yawAngle);
		getCameraSelected()->modifyPitch(_pitchAngle);
		if (cameraSelectedIndex >= 1 && cameraSelectedIndex <= 2) {
			getObject(cameraSelectedIndex-1)->modifyYaw(_yawAngle);		
			getObject(cameraSelectedIndex-1)->modifyPitch(_pitchAngle);		
		}
		toReturn = false;
	}

	return toReturn;
}

GLvoid SceneManager::handleKeyboard(bool _keys[256]){

	int cameraSelectedIndex = getCameraSelectedIndex();

	if (cameraSelectedIndex == 0){
		if (_keys[VK_UP]) getCameraSelected()->moveForward();
		if (_keys[VK_DOWN]) getCameraSelected()->moveBackwards();	
		if (_keys[VK_LEFT]) getCameraSelected()->turnLeft();
		if (_keys[VK_RIGHT]) getCameraSelected()->turnRight();
		if (_keys[VK_PRIOR]) getCameraSelected()->lookUp();
		if (_keys[VK_NEXT]) getCameraSelected()->lookDown();
	}	

	if (_keys['a'-32])	getObject(0)->turnLeft(true);	
	else getObject(0)->turnLeft(false);
	if (_keys['w'-32])	getObject(0)->accelerate(true);
	else getObject(0)->accelerate(false);
	if (_keys['d'-32])	getObject(0)->turnRight(true);	
	else getObject(0)->turnRight(false);
	if (_keys['s'-32])	getObject(0)->brake(true);
	else getObject(0)->brake(false);

	if (_keys['j'-32])	getObject(1)->turnLeft(true);	
	else getObject(1)->turnLeft(false);
	if (_keys['i'-32])	getObject(1)->accelerate(true);
	else getObject(1)->accelerate(false);
	if (_keys['l'-32])	getObject(1)->turnRight(true);
	else getObject(1)->turnRight(false);
	if (_keys['k'-32])	getObject(1)->brake(true);
	else getObject(1)->brake(false);
	
	if (_keys['t'-32])	transparencyEnabled = !transparencyEnabled;
	if (_keys['f'-32])	fogEnabled = !fogEnabled;
	if (_keys['g'-32])  getLight(0)->switchState();
	if (_keys['h'-32])	{
		switch (trafficLightOn){
			case 0: getLight(1)->switchState(); trafficLightOn++; break;
			case 1: getLight(1)->switchState(); getLight(2)->switchState(); trafficLightOn++; break;
			case 2: getLight(2)->switchState(); getLight(3)->switchState(); trafficLightOn++; break;
			case 3: getLight(3)->switchState(); trafficLightOn = 0; break;
		}
	}

	if (_keys[49])	selectCamera(0);
	if (_keys[50])	selectCamera(1);
	if (_keys[51])	selectCamera(2);
}

GLvoid SceneManager::drawAxisScene(){

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glLineWidth(2.5f);

	glEnable(GL_LINE_STIPPLE);	
		
	glColor3f(1.0,0.0,0.0);	

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(200,0,0);		
	glEnd();

	glColor3f(0.0,1.0,0.0);

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(0,200,0);		
	glEnd();

	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(0,0,200);		
	glEnd();

	glDisable(GL_LINE_STIPPLE);

	glLineWidth(0.2f);	

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

GLvoid SceneManager::drawFloor(int _bigWidth){

	GLfloat width = (float) _bigWidth / (float) 100;

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	GLfloat materialAmbient[4] =		{0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat materialDiffuse[4] =		{0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat materialSpecular[4] =		{0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat materialEmission[4] =		{0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);	

	glBindTexture(GL_TEXTURE_2D, textureRoad[0]);  
	
	for (int i = -50; i < 50; i++)
	{
		for (int j = -50; j < 50; j++)
		{
			glBegin(GL_QUADS);     
			  glTexCoord2f(0.0f, 0.0f); glVertex3f(i*width, 0, j*width);
			  glTexCoord2f(1.0f, 0.0f); glVertex3f(i*width+width, 0, j*width);  
			  glTexCoord2f(1.0f, 1.0f); glVertex3f(i*width+width, 0, j*width+width);   
			  glTexCoord2f(0.0f, 1.0f); glVertex3f(i*width, 0, j*width+width);  
			glEnd();  
		}	
	}	

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

AUX_RGBImageRec * SceneManager::LoadBMP(char *Filename){

    FILE *File=NULL;											
 
    if (!Filename)												
    {
        return NULL;											
    }
 
    File=fopen(Filename,"r");									
 
    if (File)													
    {
        fclose(File);											
        return auxDIBImageLoad(Filename);						
    }
    return NULL;												
}

GLvoid SceneManager::drawSkyBox(float x, float y, float z, float width, float height, float length){
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glColor3f(1.0,1.0,1.0);	  
 
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[BACK_ID]);  
   
	x = x - width  / 2;  
	y = y - height / 2;  
	z = z - length / 2;  

	glBegin(GL_QUADS);        
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);  
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z);   
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height,	z);  
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,			z);          
	glEnd();  
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[FRONT_ID]);  
	glBegin(GL_QUADS);     
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,			z + length);  
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height,	z + length);  
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z + length);   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);  
	glEnd();    
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[BOTTOM_ID]);  
	glBegin(GL_QUADS);             
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,			z);  
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y,			z + length);  
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length);   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);  
	glEnd();  
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[TOP_ID]);  
	glBegin(GL_QUADS);        
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height,	z);  
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height,	z + length);   
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height,   z + length);  
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height,   z);  
	glEnd();  
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[LEFT_ID]);  
	glBegin(GL_QUADS);        
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height,   z);     
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height,   z + length);   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,			z + length);  
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,			z);                
	glEnd();  
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[RIGHT_ID]);  
	glBegin(GL_QUADS);     
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);  
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);  
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,   z + length);   
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,   z);  
	glEnd();  

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

int SceneManager::loadSceneTextures(){

    int Status=FALSE;											

	AUX_RGBImageRec *TextureImageSkyBox[6];							
	AUX_RGBImageRec *TextureImageRoad[1];							
 
	memset(TextureImageSkyBox,	0,sizeof(void *)*6);					
	memset(TextureImageRoad,	0,sizeof(void *)*1);					

	TextureImageSkyBox[0]=LoadBMP("Data/cubemap/Back.bmp");
	TextureImageSkyBox[1]=LoadBMP("Data/cubemap/Front.bmp");
	TextureImageSkyBox[2]=LoadBMP("Data/cubemap/Bottom.bmp");
	TextureImageSkyBox[3]=LoadBMP("Data/cubemap/Top.bmp");
	TextureImageSkyBox[4]=LoadBMP("Data/cubemap/Left.bmp");
	TextureImageSkyBox[5]=LoadBMP("Data/cubemap/Right.bmp");
	
	glGenTextures(6, &texturesSkyBox[0]);												
		
	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[0]->sizeX, TextureImageSkyBox[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[0]->data); // ( NEW )

	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[1]->sizeX, TextureImageSkyBox[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[1]->data); // ( NEW )

	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[2]->sizeX, TextureImageSkyBox[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[2]->data); // ( NEW )

	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[3]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[3]->sizeX, TextureImageSkyBox[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[3]->data); // ( NEW )

	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[4]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[4]->sizeX, TextureImageSkyBox[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[4]->data); // ( NEW )

	glBindTexture(GL_TEXTURE_2D, texturesSkyBox[5]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageSkyBox[5]->sizeX, TextureImageSkyBox[5]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageSkyBox[5]->data); // ( NEW )

	for (int i = 0; i <6; i++){
		if (TextureImageSkyBox[i])										
			{
				if (TextureImageSkyBox[i]->data)								
				{
					free(TextureImageSkyBox[i]->data);						
				}
 
				free(TextureImageSkyBox[i]);									
			}
	}	

	if (TextureImageRoad[0]=LoadBMP("Data/road/stoneroad.bmp"))
	{
		Status=TRUE;											
		glGenTextures(1, &textureRoad[0]);							
		glBindTexture(GL_TEXTURE_2D, textureRoad[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImageRoad[0]->sizeX, TextureImageRoad[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImageRoad[0]->data); 
	}
	if (TextureImageRoad[0])										
	{
		if (TextureImageRoad[0]->data)								
		{
			free(TextureImageRoad[0]->data);						
		}
 
		free(TextureImageRoad[0]);									
	}

    return Status;												
}

GLvoid SceneManager::drawText(float x, float y, float z, char* format, ...){
    va_list args;   //  Variable argument list
    int len;        // String length
    int i;          //  Iterator
    char * text;    // Text
 
    //  Initialize a variable argument list
    va_start(args, format);
 
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    len = _vscprintf(format, args) + 1;
 
    //  Allocate memory for a string of the specified size
    text = (char*) malloc(len * sizeof(char));
 
    //  Write formatted output using a pointer to the list of arguments
    vsprintf_s(text, len, format, args);
 
    //  End using variable argument list
    va_end(args);
 
    //  Specify the raster position for pixel operations.
    glRasterPos3f (x, y, z);
 
    //  Draw the characters one by one
    for (i = 0; text[i] != '\0'; i++)
    glutBitmapCharacter(font_style, text[i]);
 
    //  Free the allocated memory for the string
    free(text);
}

bool SceneManager::getFogEnabled(){

	return fogEnabled;
}

bool SceneManager::getTransparencyEnabled(){

	return transparencyEnabled;
}

GLvoid SceneManager::createLights(){

	SceneLight *lightWhite;
	SceneLight *lightRed;
	SceneLight *lightYellow;
	SceneLight *lightGreen;	

	Coordinate3f positionLightWhite;
	Coordinate3f positionLightRed;
	Coordinate3f positionLightYellow;
	Coordinate3f positionLightGreen;	

	Component lightWhiteAmbient; Component lightWhiteDiffuse; Component lightWhiteSpecular; Component lightWhiteEmission;
	Component lightRedAmbient;	 Component lightRedDiffuse;   Component lightRedSpecular;   Component lightRedEmission;
	Component lightYellowAmbient;Component lightYellowDiffuse;Component lightYellowSpecular;Component lightYellowEmission;
	Component lightGreenAmbient; Component lightGreenDiffuse; Component lightGreenSpecular; Component lightGreenEmission;	

	positionLightWhite.x = 0.0f;		positionLightWhite.y = 300.0f;		positionLightWhite.z = -300.0f;
	positionLightRed.x = -60.0f;		positionLightRed.y = 186.0f;		positionLightRed.z = -140.0f;
	positionLightYellow.x = -60.0f;		positionLightYellow.y = 166.0f;		positionLightYellow.z = -140.0f;
	positionLightGreen.x = -60.0f;		positionLightGreen.y = 146.0f;		positionLightGreen.z = -140.0f;
	
	lightWhiteAmbient.red = 0.0f;  lightWhiteAmbient.green = 0.0f;  lightWhiteAmbient.blue = 0.0f;  lightWhiteAmbient.alpha = 1.0f;
	lightWhiteDiffuse.red = 0.8f;  lightWhiteDiffuse.green = 0.6f;  lightWhiteDiffuse.blue = 0.6f;  lightWhiteDiffuse.alpha = 1.0f;
	lightWhiteSpecular.red = 0.9f; lightWhiteSpecular.green = 0.7f; lightWhiteSpecular.blue = 0.7f; lightWhiteSpecular.alpha = 1.0f;
	lightWhiteEmission.red = 0.9f; lightWhiteEmission.green = 0.8f; lightWhiteEmission.blue = 0.8f; lightWhiteEmission.alpha = 1.0f;

	lightRedAmbient.red = 0.0f;  lightRedAmbient.green = 0.0f;  lightRedAmbient.blue = 0.0f;  lightRedAmbient.alpha = 1.0f;
	lightRedDiffuse.red = 0.8f;  lightRedDiffuse.green = 0.0f;  lightRedDiffuse.blue = 0.0f;  lightRedDiffuse.alpha = 1.0f;
	lightRedSpecular.red = 0.3f; lightRedSpecular.green = 0.0f; lightRedSpecular.blue = 0.0f; lightRedSpecular.alpha = 1.0f;
	lightRedEmission.red = 0.9f; lightRedEmission.green = 0.0f; lightRedEmission.blue = 0.0f; lightRedEmission.alpha = 1.0f;

	lightYellowAmbient.red = 0.0f;  lightYellowAmbient.green = 0.0f;  lightYellowAmbient.blue = 0.0f;  lightYellowAmbient.alpha = 1.0f;
	lightYellowDiffuse.red = 0.8f;  lightYellowDiffuse.green = 0.8f;  lightYellowDiffuse.blue = 0.0f;  lightYellowDiffuse.alpha = 1.0f;
	lightYellowSpecular.red = 0.3f; lightYellowSpecular.green = 0.3f; lightYellowSpecular.blue = 0.0f; lightYellowSpecular.alpha = 1.0f;
	lightYellowEmission.red = 0.9f; lightYellowEmission.green = 0.9f; lightYellowEmission.blue = 0.0f; lightYellowEmission.alpha = 1.0f;

	lightGreenAmbient.red = 0.0f;  lightGreenAmbient.green = 0.0f;  lightGreenAmbient.blue = 0.0f;  lightGreenAmbient.alpha = 1.0f;
	lightGreenDiffuse.red = 0.0f;  lightGreenDiffuse.green = 0.8f;  lightGreenDiffuse.blue = 0.0f;  lightGreenDiffuse.alpha = 1.0f;
	lightGreenSpecular.red = 0.0f; lightGreenSpecular.green = 0.3f; lightGreenSpecular.blue = 0.0f; lightGreenSpecular.alpha = 1.0f;
	lightGreenEmission.red = 0.0f; lightGreenEmission.green = 0.9f; lightGreenEmission.blue = 0.0f; lightGreenEmission.alpha = 1.0f;

	lightWhite =	new SceneLight(0,true, 20,positionLightWhite, lightWhiteAmbient, lightWhiteDiffuse, lightWhiteSpecular, lightWhiteEmission);
	lightRed =		new SceneLight(1,false,5,positionLightRed,   lightRedAmbient,   lightRedDiffuse,   lightRedSpecular,   lightRedEmission);
	lightYellow =	new SceneLight(2,false,5,positionLightYellow,lightYellowAmbient,lightYellowDiffuse,lightYellowSpecular,lightYellowEmission);
	lightGreen =	new SceneLight(3,false,5,positionLightGreen, lightGreenAmbient, lightGreenDiffuse, lightGreenSpecular, lightGreenEmission);
	

	addLight(lightWhite);
	addLight(lightRed);
	addLight(lightYellow);
	addLight(lightGreen);	
}

GLvoid SceneManager::createCameras(){

	SceneCamera *cameraScene;
	SceneCamera *cameraCar1;
	SceneCamera *cameraCar2;	

	Coordinate3f positionScene; 
	Coordinate3f positionCar1; 
	Coordinate3f positionCar2; 

	positionScene.x = 300.0f;			positionScene.y = 100.0f;			positionScene.z = 300.0f;
	positionCar1.x = -200.0f;			positionCar1.y = 0.0f;				positionCar1.z = 200.0f;
	positionCar2.x = 200.0f;			positionCar2.y = 0.0f;				positionCar2.z = 200.0f;

	cameraScene = new SceneCamera(positionScene,  -40.0f,  45.0f, 0.0f);	
	cameraCar1 = new SceneCamera (positionCar1,		0.0f,	0.0f, 0.0f);
	cameraCar2 = new SceneCamera (positionCar2,		0.0f,   0.0f, 50.0f);

	addCamera(cameraScene);
	addCamera(cameraCar1);
	addCamera(cameraCar2);
}

GLvoid SceneManager::createObjects(){

	SceneObject *car1;
	SceneObject *car2;
	SceneObject *postBox;
	SceneObject *trafficLight;

	Coordinate3f positionCar1; 
	Coordinate3f positionCar2; 
	Coordinate3f positionPostBox; 
	Coordinate3f positionTrafficLight;

	char* archivo_3dsCar1 = "Data/cars/chevelle.3ds";
	char* archivo_3dsCar2 = "Data/cars/kia_rio.3ds";
	char* archivo_3dsPostbox = "Data/road/postbox/postbox.3ds";
	char* archivo_3dsTrafficBarrel = "Data/road/trafficbarrel/trafficbarrel.3ds";
	char* archivo_3dsTrafficLight = "Data/road/trafficlight/trafficlight.3ds";

	char* archivo_textCar1 = "Data/cars/chevelle.bmp";			
	char* archivo_textCar2 = "Data/cars/kia_rio.bmp";		
	char* archivo_textPostbox = "Data/road/postbox/postbox.bmp";	
	char* archivo_textTrafficBarrel = "Data/road/trafficbarrel/trafficbarrel.bmp";	
	char* archivo_textTrafficLight = "Data/road/trafficlight/trafficlight.bmp";	

	positionCar1.x = -200.0f;			positionCar1.y = 0.0f;				positionCar1.z = 200.0f;
	positionCar2.x = 200.0f;			positionCar2.y = 0.0f;				positionCar2.z = 200.0f;	
	positionPostBox.x = 550.0f;			positionPostBox.y = 10.0f;			positionPostBox.z = 0.0f;	
	positionTrafficLight.x = 540;	positionTrafficLight.y = 180.0f;	positionTrafficLight.z = -50.0f;

	car1 = new SceneObject(archivo_3dsCar1, 1, archivo_textCar1, positionCar1, 0.0f, 0.0f, 0.0f);
	car2 = new SceneObject(archivo_3dsCar2, 0, archivo_textCar2, positionCar2, 0.0f, 0.0f, 0.0f);
	postBox = new SceneObject(archivo_3dsPostbox, 0, archivo_textPostbox, positionPostBox, 0.0f, 0.0f, 90.0f);
	trafficLight = new SceneObject(archivo_3dsTrafficLight, 0, archivo_textTrafficLight, positionTrafficLight, 0.0f, 180.0f, 270.0f);

	addObject(car1);
	addObject(car2);
	addObject(postBox);
	addObject(trafficLight);

	
	int size = mapSize / 2;
	for (int i = -2000; i< 2000; i+= 200){
		Coordinate3f positionLeft;
		positionLeft.x = -400; positionLeft.y = 30.0f; positionLeft.z = i;
		SceneObject * trafficBarrelToAddLeft = new SceneObject(archivo_3dsTrafficBarrel, 0, archivo_textTrafficBarrel, positionLeft, 0.0f, 0.0f, 90.0f);
		addObject(trafficBarrelToAddLeft);

		Coordinate3f positionCenter;
		positionCenter.x = 0.0f; positionCenter.y = 30.0f; positionCenter.z = i;
		SceneObject * trafficBarrelToAddCenter = new SceneObject(archivo_3dsTrafficBarrel, 0, archivo_textTrafficBarrel, positionCenter, 0.0f, 0.0f, 90.0f);
		addObject(trafficBarrelToAddCenter);

		Coordinate3f positionRight;
		positionRight.x = 400; positionRight.y = 30.0f; positionRight.z = i;
		SceneObject * trafficBarrelToAddRight = new SceneObject(archivo_3dsTrafficBarrel, 0, archivo_textTrafficBarrel, positionRight, 0.0f, 0.0f, 90.0f);
		addObject(trafficBarrelToAddRight);
	}
	

}