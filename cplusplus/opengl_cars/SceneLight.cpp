#include <windows.h>											// Header File For Windows
#include <math.h>												// Header File For Windows Math Library     ( ADD )
#include <stdio.h>												// Header File For Standard Input/Output    ( ADD )
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"												// Header File For The Glaux Library
#include "GLAux.h"												// Header File For The Glaux Library
#include "SceneLight.hpp"

//-------------------------------------------------------
// SceneLight implementation
//-------------------------------------------------------

SceneLight::SceneLight(int _index, bool _enabled, float _size, Coordinate3f _position, Component _ambient, Component _diffuse, Component _specular, Component _emission){
	index = _index;

	position = _position;
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	emission = _emission;

	size = _size;

	enabled = _enabled;

	GLfloat ambientFloat[4] = {ambient.red, ambient.green, ambient.blue, ambient.alpha};
	GLfloat diffuseFloat[4] = {diffuse.red, diffuse.green, diffuse.blue, diffuse.alpha};
	GLfloat specularFloat[4] = {specular.red, specular.green, specular.blue, specular.alpha};

	switch (index){
		case 0: 
					glLightfv(GL_LIGHT0, GL_AMBIENT, ambientFloat);
					glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseFloat);
					glLightfv(GL_LIGHT0, GL_SPECULAR, specularFloat);
					break;				
		case 1: 
					glLightfv(GL_LIGHT1, GL_AMBIENT, ambientFloat);
					glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseFloat);
					glLightfv(GL_LIGHT1, GL_SPECULAR, specularFloat);
					break;
		case 2: 
					glLightfv(GL_LIGHT2, GL_AMBIENT, ambientFloat);
					glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseFloat);
					glLightfv(GL_LIGHT2, GL_SPECULAR, specularFloat);
					break;
		case 3: 
					glLightfv(GL_LIGHT3, GL_AMBIENT, ambientFloat);
					glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseFloat);
					glLightfv(GL_LIGHT3, GL_SPECULAR, specularFloat);
					break;
	}
}

SceneLight::~SceneLight(){

}

Coordinate3f SceneLight::getPosition(){

	return position;
}

Component SceneLight::getComponentAmbient(){	

	return ambient;
}

Component SceneLight::getComponentDiffuse(){	

	return diffuse;
}

Component SceneLight::getComponentSpecular(){	

	return specular;
}

Component SceneLight::getComponentEmission(){	

	return emission;
}

GLvoid SceneLight::switchState(){

	enabled = !enabled;
}

GLvoid SceneLight::draw(){

	GLfloat positionFloat[3] = {position.x, position.y, position.z};
	GLfloat ambientFloat[4] = {ambient.red, ambient.green, ambient.blue, ambient.alpha};
	GLfloat diffuseFloat[4] = {diffuse.red, diffuse.green, diffuse.blue, diffuse.alpha};
	GLfloat specularFloat[4] = {specular.red, specular.green, specular.blue, specular.alpha};
	GLfloat emissionFloat[4] = {emission.red, emission.green, emission.blue, emission.alpha};
	GLfloat noEmissionFloat[4] = {0.3f, 0.3f, 0.3f, 1.0f};

	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);		

		if (enabled) {
			glMaterialfv(GL_FRONT, GL_EMISSION,emissionFloat);	
			switch (index){
				case 0: glEnable(GL_LIGHT0); break;
				case 1: glEnable(GL_LIGHT1); break;
				case 2: glEnable(GL_LIGHT2); break;
				case 3: glEnable(GL_LIGHT3); break;
			}
		}
		else {
			glMaterialfv(GL_FRONT,GL_EMISSION,noEmissionFloat);
			switch (index){
				case 0: glDisable(GL_LIGHT0); break;
				case 1: glDisable(GL_LIGHT1); break;
				case 2: glDisable(GL_LIGHT2); break;
				case 3: glDisable(GL_LIGHT3); break;
			}
			glColor3f(0.3f,0.3f,0.3f);
		}

	GLfloat materialAmbient[4] =		{0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat materialDiffuse[4] =		{0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat materialSpecular[4] =		{0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat materialEmission[4] =		{0.0f, 0.0f, 0.0f, 1.0f};

		glLightfv(GL_LIGHT0, GL_POSITION, positionFloat);	

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);

		glColor3f(1.0f,1.0f,1.0f);
		glutSolidSphere(size,20,10);

		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmissionFloat);

	glPopMatrix();
}