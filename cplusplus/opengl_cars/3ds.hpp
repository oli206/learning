#ifndef L3DS_H
#define L3DS_H

struct LVector3
{
    float x;
    float y;
    float z;
};

struct LColor3
{   
    float r;
    float g;
    float b;
};

struct LUV
{
    float u;
    float v;
};

struct LTriangle
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

struct LFace
{
    LVector3 vertices[3];
    LVector3 normals[3];
    LUV uv[3];
    LVector3 normal;
};

struct LMatrix
{
    LVector3 xAxis;
    LVector3 yAxis;
    LVector3 zAxis;
    LVector3 pos;
};

class LObject
{
public:
    LObject();
    virtual ~LObject();
    virtual void SetName(const char *value);
    virtual int GetName(char *buffer, int max_size);
protected:
    char *m_name;
    int m_nameSize;
};

class LMesh : public LObject
{
public:
    LMesh();
    virtual ~LMesh();
    virtual int GetVertexCount();
    virtual int GetUVCount();
    virtual int GetNormalCount();
    virtual int GetTriangleCount();
    virtual int GetFaceCount();
    virtual LVector3 GetVertex(int index);
    virtual LVector3 GetNormal(int index);
    virtual LUV GetUV(int index);
    virtual void AddVertex(LVector3 vec);
    virtual void AddNormal(LVector3 vec);
    virtual void AddUV(LUV uv);
    virtual void Clear();
    virtual void AddTriangle(int a, int b, int c);
    virtual LTriangle GetTriangle(int index);
    virtual LFace GetFace(int index);
    virtual LMatrix GetMatrix();
    virtual void TransformVertices(bool value);
    virtual void SetMatrix(LMatrix m);
    virtual void CalcNormals();
	virtual LVector3 * getVertices();
	virtual LUV * getTextCoords();
	virtual void freeVertices();
	virtual void freeTextCoords();
protected:
    bool m_transform;
    LVector3 *m_vertices;
    int m_vertexCount;
    LVector3 *m_normals;
    int m_normalCount;
    LUV *m_uv;
    int m_uvCount;
    LTriangle *m_triangles;
    int m_triangleCount;
    LMatrix m_matrix;
};

class LLight : public LObject
{
public:
    LLight();
    virtual ~LLight();
    virtual void Clear();
    virtual void SetPosition(LVector3 vec);
    virtual LVector3 GetPosition();
    virtual void SetColor(LColor3 color);
    virtual LColor3 GetColor();
    virtual void SetSpotlight(bool value);
    virtual bool GetSpotlight();
    virtual void SetTarget(LVector3 target);
    virtual LVector3 GetTarget();
    virtual void SetHotspot(float value);
    virtual float GetHotspot();
    virtual void SetFalloff(float value);
    virtual float GetFalloff();
protected:
    LVector3 m_pos;
    LColor3 m_color;
    bool m_spotlight;
    LVector3 m_target;
    float m_hotspot;
    float m_falloff;
};

struct LChunk;

class L3DS
{
public:
    // the default contructor
    L3DS();
    // constructs the object and loads the file
    L3DS(const char *filename);
    // destructor
    virtual ~L3DS();
    // load 3ds file 
    virtual bool LoadFile(const char *filename);
    // returns the number of meshes in the scene
    virtual int GetMeshCount();
    // returns the number of lights in the scene
    virtual int GetLightCount();
    // returns a pointer to a mesh
    virtual LMesh *GetMesh(int index);
    // returns a pointer to a light at a given index
    virtual LLight *GetLight(int index);
protected:
    // used internally for reading
    char m_objName[100];
    // the lights found in 3ds scene
    LLight **m_lights;
    // number of lights
    int m_lightCount;
    // triangular meshes
    LMesh **m_meshes;
    // number of meshes
    int m_meshCount;
    // true if end of file is reached
    bool m_eof;
    // this is true when the file is loaded
    bool m_loaded;
    // buffer for loading, used for speedup
    unsigned char *m_buffer;
    // the size of the buffer
    int m_bufferSize;
    // the current cursor position in the buffer
    int m_pos;
    // read a light chunk 
    virtual void ReadLight(LChunk parent);
    // read a trimesh chunk
    virtual void ReadMesh(LChunk parent);
    // read the chunk and return it.
    virtual LChunk ReadChunk();
    // read untip given chunk is found
    virtual bool FindChunk(LChunk &target, LChunk parent);
    // skip to the end of chunk "chunk"
    virtual void SkipChunk(LChunk chunk);
    // this is where 3ds file is being read
    virtual bool Read3DS();
    // add a mesh to a scene
    virtual void AddMesh();
    // add a light to a scene
    virtual void AddLight();
    // clears all data.
    virtual void Clear();
    // reads a short value from the buffer
    virtual short ReadShort();
    // reads an int value from the buffer
    virtual int ReadInt();
    // reads a char from the buffer
    virtual char ReadChar();
    //reada a floatvalue from the buffer
    virtual float ReadFloat();
    //reads an asciiz string 
    virtual void ReadASCIIZ(char *buf, int &count, int max_count);
    // seek wihtin the buffer
    virtual void Seek(int offset, int origin);
    // returns the position of the cursor
    virtual int Pos();
};
#endif