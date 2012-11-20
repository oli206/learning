#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneTypes.hpp"
#include "3ds.hpp"

const int maxLights = 10;
const int maxObjects = 1000;
const int maxCameras = 10;

//-------------------------------------------------------
// SceneManager definition
//------------------------------------------------------

class SceneManager
{

	private:

		GLfloat mapSize;

		int cameraSelected;
		int numberOfObjects;
		int numberOfCameras;
		int numberOfLights;

		int trafficLightOn;

		SceneObject *objects[maxObjects];
		SceneCamera *cameras[maxCameras];
		SceneLight *lights[maxLights];

		GLuint texturesSkyBox[6];
		GLuint textureRoad[1];

		bool transparencyEnabled;
		bool fogEnabled;

		bool fire1;
		bool fire2;

		GLvoid createObjects();
		GLvoid createCameras();
		GLvoid createLights();

		GLvoid addObject(SceneObject *_object);
		GLvoid addCamera(SceneCamera *_camera);
		GLvoid addLight(SceneLight *_light);

		int loadSceneTextures();
		AUX_RGBImageRec *LoadBMP(char *Filename);	

	public:

		SceneManager(GLfloat _mapSize);
		~SceneManager();	

		// Objects & Camera

		GLvoid selectCamera(int _index);
		SceneCamera *getCameraSelected();			
		int getCameraSelectedIndex();
		GLvoid positionCamera();
		GLvoid positionCamera(int _index);
		int getNumberOfObjects();
		int getNumberOfLights();
		int getNumberOfCameras();

		SceneCamera *getCamera(int _index);
		SceneObject *getObject(int _index);
		SceneLight *getLight(int _index);

		// Input

		bool handleMouse(bool _rotateCamera, bool _isDragging, bool _moveForward, GLfloat _yawAngle, GLfloat _pitchAngle);
		GLvoid handleKeyboard(bool _keys[256]);

		// GL

		GLvoid drawAxisScene();
		GLvoid drawFloor(int _bigWidth);
		GLvoid drawSkyBox(float x, float y, float z, float width, float height, float length);

		GLvoid drawText(float x, float y, float z, char* format, ...);

		bool getFogEnabled();
		bool getTransparencyEnabled();

};
#endif