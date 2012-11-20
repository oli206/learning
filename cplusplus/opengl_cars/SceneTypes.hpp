#ifndef SCENETYPES_H
#define SCENETYPES_H

const float piover180 =	0.0174532925f;

struct Coordinate3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct Component
{
	float red;
	float green;
	float blue;
	float alpha;
};

#endif