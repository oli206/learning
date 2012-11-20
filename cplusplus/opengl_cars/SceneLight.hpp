#ifndef SCENELIGHT_H
#define SCENELIGHT_H

#include "SceneTypes.hpp"

//-------------------------------------------------------
// SceneLight definition
//------------------------------------------------------

class SceneLight
{

	private:

		int index;
		float size;
		Coordinate3f position;
		Component ambient;
		Component diffuse;
		Component specular;
		Component emission;		

		bool enabled;

	public:

		SceneLight(int _index, bool _enabled, float _size, Coordinate3f _position, Component _ambient, Component _diffuse, Component _specular, Component _emission);
		~SceneLight();
	
		Coordinate3f getPosition();
		Component getComponentAmbient();
		Component getComponentDiffuse();
		Component getComponentSpecular();
		Component getComponentEmission();	

		GLvoid switchState();

		GLvoid draw();

};
#endif