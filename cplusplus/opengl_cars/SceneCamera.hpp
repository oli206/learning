#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "SceneTypes.hpp"

//-------------------------------------------------------
// SceneCamera definition
//------------------------------------------------------

class SceneCamera
{

	private:

		Coordinate3f position;
		GLfloat pitchAngle;
		GLfloat yawAngle;
		GLfloat yAxisOffset;

	public:

		SceneCamera();
		SceneCamera(Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle, GLfloat _yAxisOffset);
		~SceneCamera();
	
		Coordinate3f getPosition();
		GLfloat getPitchAngle();
		GLfloat getYawAngle();

		GLvoid turnLeft();
		GLvoid turnRight();
		GLvoid moveForward();
		GLvoid moveBackwards();

		GLvoid updatePosition(Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle);

		GLvoid lookUp();
		GLvoid lookDown();
		GLvoid modifyYaw(GLfloat _angle);
		GLvoid modifyPitch(GLfloat _angle);

};
#endif