#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "SceneTypes.hpp"
#include "3ds.hpp"

const GLfloat accelFactor = 0.5f;
const GLfloat brakeFactor = 0.2f;
const GLfloat turnFactor = 2.0f;
const GLfloat frictionFactor = 0.1f;

const GLfloat maxSpeed = 10.0f;
const GLfloat maxAccel = 0.5f;
const GLfloat maxBrake = 0.3f;
const GLfloat maxTurn = 5.0f;

//-------------------------------------------------------
// SceneObject definition
//------------------------------------------------------

class SceneObject
{

	private:

		int zAxis;					// Para saber en que eje apunta el objeto
		GLuint texture[1];    
		L3DS model;

		Coordinate3f position;
		GLfloat pitchAngle;
		GLfloat yawAngle;
		GLfloat rollAngle;
		GLfloat speedZ;

		bool accelerating;
		bool braking;
		bool turningLeft;
		bool turningRight;

		GLfloat currentAccelFactor;
		GLfloat currentBrakeFactor;
		GLfloat currentTurnFactor;

		bool LoadTextures(char* _file_texture);
		AUX_RGBImageRec * LoadBMP(char *Filename);	
		GLvoid DrawAxis();

		// Vertex Buffer Objects

		unsigned int    m_nVBOVertices;                     // Vertex VBO Name
		unsigned int    m_nVBOTexCoords;                    // Texture Coordinate VBO Name

		bool IsExtensionSupported( char* szTargetExtension );

	public:

		SceneObject();
		SceneObject(char* _file_3ds, int _zAxis, char* _file_Texture, Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle, GLfloat _rollAngle);
		~SceneObject();
	
		Coordinate3f getPosition();
		GLfloat getPitchAngle();
		GLfloat getYawAngle();
		GLfloat getRollAngle();

		GLvoid calculatePhysics();
		GLvoid accelerate(bool _enable);
		GLvoid brake(bool _enable);
		GLvoid turnLeft(bool _enable);
		GLvoid turnRight(bool _enable);

		GLvoid modifyYaw(GLfloat _angle);
		GLvoid modifyPitch(GLfloat _angle);
		GLvoid modifyRoll(GLfloat _angle);

		GLvoid draw(bool _drawAxis, GLfloat _scale);

		// Vertex Buffer Objects

		GLvoid buildVBOs();
		GLvoid renderVBOs();

};
#endif