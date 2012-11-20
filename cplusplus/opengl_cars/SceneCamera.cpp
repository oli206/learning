#include <windows.h>											// Header File For Windows
#include <math.h>												// Header File For Windows Math Library     ( ADD )
#include <stdio.h>												// Header File For Standard Input/Output    ( ADD )
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"												// Header File For The Glaux Library
#include "GLAux.h"												// Header File For The Glaux Library
#include "SceneCamera.hpp"

//-------------------------------------------------------
// SceneCamera implementation
//-------------------------------------------------------

SceneCamera::SceneCamera()
{

}

SceneCamera::SceneCamera(Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle, GLfloat _yAxisOffset){

	position = _pos;
	pitchAngle = _pitchAngle;
	yawAngle = _yawAngle;
	yAxisOffset = _yAxisOffset;
	position.y += yAxisOffset;

}

SceneCamera::~SceneCamera(){

}

Coordinate3f SceneCamera::getPosition(){

	return position;
}

GLfloat SceneCamera::getPitchAngle(){

	return pitchAngle;
}

GLfloat SceneCamera::getYawAngle(){

	return yawAngle;
}

GLvoid SceneCamera::moveForward(){

	position.x -= (float)sin(yawAngle*piover180) * 10.0f;
	position.z -= (float)cos(yawAngle*piover180) * 10.0f;
}

GLvoid SceneCamera::moveBackwards(){

	position.x += (float)sin(yawAngle*piover180) * 10.0f;
	position.z += (float)cos(yawAngle*piover180) * 10.0f;
}

GLvoid SceneCamera::turnLeft(){

	yawAngle += 10.0f;
}

GLvoid SceneCamera::turnRight(){

	yawAngle -= 10.0f;
}

GLvoid SceneCamera::updatePosition(Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle){

	position = _pos;
	pitchAngle = _pitchAngle;
	yawAngle = _yawAngle;
	position.y += yAxisOffset;
}

GLvoid SceneCamera::lookUp(){

	pitchAngle -= 5.0f;
}

GLvoid SceneCamera::lookDown(){

	pitchAngle += 5.0f;
}

GLvoid SceneCamera::modifyYaw(GLfloat _angle){

	yawAngle -= _angle;
}

GLvoid SceneCamera::modifyPitch(GLfloat _angle){

	pitchAngle += _angle;
}