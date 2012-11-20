#include <windows.h>											// Header File For Windows
#include <math.h>												// Header File For Windows Math Library     ( ADD )
#include <stdio.h>												// Header File For Standard Input/Output    ( ADD )
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"												// Header File For The Glaux Library
#include "GLAux.h"												// Header File For The Glaux Library
#include "SceneObject.hpp" 

//#define NO_VBOS                               // If Defined, VBOs Will Be Forced Off


// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

bool        g_fVBOSupported = false;                    // ARB_vertex_buffer_object supported?
 
// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;                  // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;                  // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;                  // VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;                // VBO Deletion Procedure

//-------------------------------------------------------
// IGRObject implementation
//-------------------------------------------------------

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(char* _file_3ds, int _zAxis, char* _file_Texture, Coordinate3f _pos, GLfloat _pitchAngle, GLfloat _yawAngle, GLfloat _rollAngle)
{
	position = _pos;
	pitchAngle = _pitchAngle;
	yawAngle = _yawAngle;
	rollAngle = _rollAngle;
	zAxis = _zAxis;
	speedZ = 0.0f;
	accelerating = false;
	braking = false;
	turningLeft = false;
	turningRight = false;

	currentAccelFactor = accelFactor;
	currentBrakeFactor = brakeFactor;
	currentTurnFactor = turnFactor;


	if (_file_3ds != NULL && !LoadTextures(_file_Texture)) exit(1);
	if (_file_Texture != NULL && !model.LoadFile(_file_3ds)) exit(1);
}

SceneObject::~SceneObject(){

}

Coordinate3f SceneObject::getPosition(){

	return position;
}

GLfloat SceneObject::getPitchAngle(){

	return pitchAngle;
}

GLfloat SceneObject::getYawAngle(){

	return yawAngle;
}

GLfloat SceneObject::getRollAngle(){

	return rollAngle;
}

GLvoid SceneObject::calculatePhysics(){

	if (accelerating && speedZ < maxSpeed) speedZ += accelFactor;
	if (braking && speedZ-brakeFactor >= 0) speedZ -= brakeFactor;
	if (turningLeft) yawAngle += turnFactor;
	if (turningRight) yawAngle -= turnFactor;
	if (speedZ - frictionFactor > 0) speedZ -= frictionFactor;

	if (speedZ < frictionFactor) speedZ = 0;

	position.x -= (float)sin(yawAngle*piover180) * speedZ;
	position.z -= (float)cos(yawAngle*piover180) * speedZ;
}

GLvoid SceneObject::accelerate(bool _enable){
	accelerating = _enable;
/*	if (accelerating && accelFactor <= maxAccel) currentAccelFactor += 0.07f;
	else if (!accelerating) currentAccelFactor = accelFactor;*/
}

GLvoid SceneObject::brake(bool _enable){
	braking = _enable;
/*	if (braking && brakeFactor <= maxBrake) currentBrakeFactor += 0.05f;
	else if (!braking) currentBrakeFactor = brakeFactor;*/
}

GLvoid SceneObject::turnLeft(bool _enable){
	turningLeft = _enable;
/*	if (turningLeft && turnFactor <= maxTurn) currentTurnFactor += 0.05f;
	else if (!turningLeft) currentTurnFactor = turnFactor;*/
}

GLvoid SceneObject::turnRight(bool _enable){
	turningRight = _enable;
/*	if (turningRight && turnFactor <= maxTurn) currentTurnFactor += 0.05f;
	else if (!turningRight) currentTurnFactor = turnFactor;*/
}

GLvoid SceneObject::modifyYaw(GLfloat _angle){
	yawAngle -= _angle;
}

GLvoid SceneObject::modifyPitch(GLfloat _angle){
	pitchAngle += _angle;
}

GLvoid SceneObject::modifyRoll(GLfloat _angle){
	rollAngle += _angle;
}

GLvoid SceneObject::draw(bool _drawAxis, GLfloat _scale){

	GLfloat materialAmbient[4] =		{0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat materialDiffuse[4] =		{0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat materialSpecular[4] =		{0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat materialEmission[4] =		{0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat shininess =					50.0f;

	glPushMatrix();

		glTranslatef(position.x,position.y,position.z);			
		glRotatef(rollAngle, 0.0f, 0.0f, 1.0f);
		glRotatef(yawAngle, 0.0f, 1.0f, 0.0f);		

		if (_drawAxis) DrawAxis();						

		glScalef(_scale, _scale, _scale);
		glEnable(GL_NORMALIZE);

		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
		glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);		

		glColor3f(1.0,1.0,1.0);	  

		LFace face;
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		glBindTexture(GL_TEXTURE_2D, texture[0]);      

		int i,k;
		for (i=0; i<model.GetMeshCount(); i++)
		{
			for (k=0; k<model.GetMesh(i)->GetFaceCount(); k++)
			{
				face = model.GetMesh(i)->GetFace(k);
				glBegin(GL_TRIANGLES);
					if (zAxis == 0) {
						glTexCoord2f(face.uv[0].u,face.uv[0].v);
						glNormal3f(face.normals[0].z,	face.normals[0].y,	face.normals[0].x);
						glVertex3f(face.vertices[0].z,	face.vertices[0].y,	face.vertices[0].x);

						glTexCoord2f(face.uv[1].u,face.uv[1].v);
						glNormal3f(face.normals[1].z,	face.normals[1].y,	face.normals[1].x);
						glVertex3f(face.vertices[1].z,	face.vertices[1].y,	face.vertices[1].x);

						glTexCoord2f(face.uv[2].u,face.uv[2].v);
						glNormal3f(face.normals[2].z,	face.normals[2].y,	face.normals[2].x);
						glVertex3f(face.vertices[2].z,	face.vertices[2].y,	face.vertices[2].x);
					}
					else if (zAxis == 1) {
						glTexCoord2f(face.uv[0].u,face.uv[0].v);
						glNormal3f(face.normals[0].x,	face.normals[0].y,	face.normals[0].z);
						glVertex3f(face.vertices[0].x,	face.vertices[0].y,	face.vertices[0].z);

						glTexCoord2f(face.uv[1].u,face.uv[1].v);
						glNormal3f(face.normals[1].x,	face.normals[1].y,	face.normals[1].z);
						glVertex3f(face.vertices[1].x,	face.vertices[1].y,	face.vertices[1].z);

						glTexCoord2f(face.uv[2].u,face.uv[2].v);
						glNormal3f(face.normals[2].x,	face.normals[2].y,	face.normals[2].z);
						glVertex3f(face.vertices[2].x,	face.vertices[2].y,	face.vertices[2].z);
					}
				glEnd();
			}
		}	

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

	glPopMatrix();
}

GLvoid SceneObject::DrawAxis(){
	glDisable(GL_LIGHTING);

	glLineWidth(2.5f);

	glEnable(GL_LINE_STIPPLE);	

	glColor3f(0.0,1.0,1.0);

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(200,0,0);		
	glEnd();

	glColor3f(1.0,0.0,1.0);

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(0,200,0);		
	glEnd();

	glColor3f(1.0,1.0,0.0);

	glBegin(GL_LINE_STRIP);		
		glVertex3s(0,0,0); glVertex3s(0,0,200);		
	glEnd();

	glDisable(GL_LINE_STIPPLE);

	glLineWidth(0.2f);	

	glEnable(GL_LIGHTING);
}

bool SceneObject::LoadTextures(char* _file_texture){
	bool status = false;
	AUX_RGBImageRec *Texture[1];							
 
	memset(Texture,	0,sizeof(void *)*1);					

	if (Texture[0]=LoadBMP(_file_texture))
	{
		status = true;
		glGenTextures(1, &texture[0]);							
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Texture[0]->sizeX, Texture[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Texture[0]->data); 
	}
	if (Texture[0])										
	{
		if (Texture[0]->data)								
		{
			free(Texture[0]->data);						
		}
 
		free(Texture[0]);									
	}
    return status;												
}

AUX_RGBImageRec * SceneObject::LoadBMP(char *Filename){
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

GLvoid SceneObject::buildVBOs(){	

		// Check For VBOs Supported
	#ifndef NO_VBOS
		g_fVBOSupported = IsExtensionSupported( "GL_ARB_vertex_buffer_object" );
		if( g_fVBOSupported )
		{
			// Get Pointers To The GL Functions
			glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
			glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
			glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
			glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
			// Load Vertex Data Into The Graphics Card Memory
		}
	#else /* NO_VBOS */
		g_fVBOSupported = false;
	#endif

    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );                  // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );         // Bind The Buffer
    // Load The Data
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, model.GetMesh(0)->GetVertexCount()*3*sizeof(float), model.GetMesh(0)->getVertices(), GL_STATIC_DRAW_ARB );
 
    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB( 1, &m_nVBOTexCoords );                 // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );        // Bind The Buffer
    // Load The Data
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, model.GetMesh(0)->GetVertexCount()*2*sizeof(float), model.GetMesh(0)->getTextCoords(), GL_STATIC_DRAW_ARB );
 
    // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
	model.GetMesh(0)->freeVertices();
	model.GetMesh(0)->freeTextCoords();
}

GLvoid SceneObject::renderVBOs(){

	// Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays

	if( g_fVBOSupported )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
		glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
	} else
	{
		glVertexPointer( 3, GL_FLOAT, 0, model.GetMesh(0)->getVertices()); // Set The Vertex Pointer To Our Vertex Data
		glTexCoordPointer( 2, GL_FLOAT, 0, model.GetMesh(0)->getTextCoords() );  // Set The Vertex Pointer To Our TexCoord Data
	}

	glBindTexture(GL_TEXTURE_2D, texture[0]); 
	// Render
	glDrawArrays( GL_TRIANGLES, 0, model.GetMesh(0)->GetVertexCount() );	// Draw All Of The Triangles At Once


	// Disable Pointers
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
}

bool SceneObject::IsExtensionSupported( char* szTargetExtension )
{
    const unsigned char *pszExtensions = NULL;
    const unsigned char *pszStart;
    unsigned char *pszWhere, *pszTerminator;
 
    // Extension names should not have spaces
    pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
    if( pszWhere || *szTargetExtension == '\0' )
        return false;
 
    // Get Extensions String
    pszExtensions = glGetString( GL_EXTENSIONS );
 
    // Search The Extensions String For An Exact Copy
    pszStart = pszExtensions;
    for(;;)
    {
        pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
        if( !pszWhere )
            break;
        pszTerminator = pszWhere + strlen( szTargetExtension );
        if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
            if( *pszTerminator == ' ' || *pszTerminator == '\0' )
                return true;
        pszStart = pszTerminator;
    }
    return false;
}