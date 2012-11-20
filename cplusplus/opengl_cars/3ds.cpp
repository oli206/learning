#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "3ds.hpp"

//-------------------------------------------------------
// generic stuff
//-------------------------------------------------------

#define SEEK_START           1900
#define SEEK_CURSOR          1901
             
#define MAIN3DS             0x4D4D
#define EDIT3DS             0x3D3D  // this is the start of the editor config
#define EDIT_MATERIAL       0xAFFF
#define EDIT_OBJECT         0x4000
#define OBJ_TRIMESH         0x4100
#define OBJ_LIGHT           0x4600
#define OBJ_CAMERA          0x4700
#define LIT_OFF             0x4620
#define LIT_SPOT            0x4610
#define TRI_VERTEXLIST      0x4110
#define TRI_VERTEXOPTIONS   0x4111
#define TRI_FACELIST        0x4120
#define TRI_FACEMAPPING     0x4140
#define TRI_MATRIX          0x4160
#define RGB_COLOR           0x0010
#define COLOR24             0x0011
#define SPOTLIGHT           0x4610


struct LChunk
{
    short id;
    int start;
    int end;
};

LVector3 AddVectors(const LVector3 a, const LVector3 b)
{
    LVector3 t;
    t.x = a.x+b.x;
    t.y = a.y+b.y;
    t.z = a.z+b.z;
    return t;
}

LVector3 SubtractVectors(const LVector3 a, const LVector3 b)
{
    LVector3 t;
    t.x = a.x-b.x;
    t.y = a.y-b.y;
    t.z = a.z-b.z;
    return t;
}

float VectorLength(const LVector3 vec)
{
    return (float)sqrt(vec.x*vec.x + vec.y*vec.y+vec.z*vec.z);
}

LVector3 NormalizeVector(const LVector3 vec)
{
    float a = VectorLength(vec);
    if (a == 0)
        return vec;
    LVector3 v;
    v.x = vec.x/a;
    v.y = vec.y/a;
    v.z = vec.z/a;
    return v;
}

LVector3 CrossProduct(const LVector3 a, const LVector3 b)
{
    LVector3 v;
    v.x = a.y*b.z - a.z*b.y;
    v.y = a.z*b.x - a.x*b.z;
    v.z = a.x*b.y - a.y*b.x;
    return v;
}

void LoadIdentityMatrix(LMatrix &m)
{
    m.xAxis.x = 1.0f;
    m.xAxis.y = 0.0f;
    m.xAxis.z = 0.0f;

    m.yAxis.x = 0.0f;
    m.yAxis.y = 1.0f;
    m.yAxis.z = 0.0f;

    m.zAxis.x = 0.0f;
    m.zAxis.y = 0.0f;
    m.zAxis.z = 1.0f;

    m.pos.x = 0.0f;
    m.pos.y = 0.0f;
    m.pos.z = 0.0f;
}

LVector3 VectorByMatrix(const LMatrix m, const LVector3 vec)
{
    LVector3 res;
    res.x = m.xAxis.x*vec.x + m.xAxis.y*vec.y + m.xAxis.z*vec.z;
    res.y = m.yAxis.x*vec.x + m.yAxis.y*vec.y + m.yAxis.z*vec.z;
    res.z = m.zAxis.x*vec.x + m.zAxis.y*vec.y + m.zAxis.z*vec.z;
    res = AddVectors(vec, m.pos);
    return res;
}

//-------------------------------------------------------
// LObject implementation
//-------------------------------------------------------

LObject::LObject()
{
    m_name = 0;
    m_nameSize = 0;
}

LObject::~LObject()
{
    if (m_name != 0)
        free(m_name);
}

void LObject::SetName(const char *value)
{
    m_nameSize = strlen(value)+1;
    m_name = (char*)malloc(m_nameSize);
    strcpy(m_name, value);
}

int LObject::GetName(char *buffer, int max_size)
{
    int i;
    if (m_nameSize >= max_size)
        i = max_size;
    else
        i = m_nameSize;
    strncpy(buffer, m_name, i);
    return i;
}

//-------------------------------------------------------
// LMesh implementation
//-------------------------------------------------------

LMesh::LMesh()
:   LObject()
{
    m_vertices = 0;
    m_vertexCount = 0;
    m_normals = 0;
    m_normalCount = 0;
    m_uv = 0;
    m_uvCount = 0;
    m_triangles = 0;
    m_triangleCount = 0;
    LoadIdentityMatrix(m_matrix);
    m_transform = false;
}

LMesh::~LMesh()
{
    Clear();
}

LVector3 LMesh::GetVertex(int index)
{
    int i;
    LVector3 vec;
    vec.x = vec.y = vec.z = 0.0f;
    if (index>=m_vertexCount)
        return vec;
    if (index < 0) 
        i = 0;
    else
        i = index;
    
    if (m_transform)
        vec = VectorByMatrix(m_matrix, m_vertices[i]);
    else
        vec = m_vertices[i];
    return vec;
}

LUV LMesh::GetUV(int index)
{
    int i;
    LUV uv;
    uv.u = uv.v = 0.0f;
    if (index >= m_uvCount)
        return uv;
    if ( index < 0)
        i = 0;
    else
        i = index;
    return m_uv[i];
}

void LMesh::AddVertex(LVector3 vec)
{
    m_vertexCount ++;
    m_vertices = (LVector3*) realloc(m_vertices, m_vertexCount*sizeof(LVector3));
    m_vertices[m_vertexCount-1].x = vec.x;
    m_vertices[m_vertexCount-1].y = vec.y;
    m_vertices[m_vertexCount-1].z = vec.z;
}

void LMesh::AddUV(LUV uv)
{
    m_uvCount++;
    m_uv = (LUV*) realloc(m_uv, m_uvCount*sizeof(LUV));
    m_uv[m_uvCount-1].u = uv.u;
    m_uv[m_uvCount-1].v = uv.v;
}

void LMesh::Clear()
{
    if ((m_vertexCount > 0) && (m_vertices != 0))
    {
        free(m_vertices);
        m_vertices = 0;
        m_vertexCount = 0;
    }
    if ((m_uvCount > 0) && (m_uv != 0))
    {
        free(m_uv);
        m_uv = 0;
        m_uvCount = 0;
    }
    if ((m_triangleCount >0) && (m_triangles != 0))
    {
        free(m_triangles);
        m_triangles = 0;
        m_triangleCount = 0;
    }
    if ((m_normalCount >0) && (m_normals != 0))
    {
        free(m_normals);
        m_normals = 0;
        m_normalCount = 0;
    }
    LoadIdentityMatrix(m_matrix);
    m_transform = false;
}

void LMesh::AddTriangle(int a, int b, int c)
{
    m_triangleCount++;
    m_triangles = (LTriangle*) realloc(m_triangles, m_triangleCount*sizeof(LTriangle));
    m_triangles[m_triangleCount-1].a = a;
    m_triangles[m_triangleCount-1].b = b;
    m_triangles[m_triangleCount-1].c = c;
}

LTriangle LMesh::GetTriangle(int index)
{
    int i;
    LTriangle tri;
    tri.a = tri.b = tri.c = 0;
    if (index>= m_triangleCount)
        return tri;
    if ( index < 0)
        i = 0;
    else
        i = index;
    return m_triangles[i];
}

int LMesh::GetVertexCount()
{
    return m_vertexCount;
}

LVector3 * LMesh::getVertices()
{
	return m_vertices;
}

LUV * LMesh::getTextCoords()
{
	return m_uv;
}

void LMesh::freeVertices()
{
	delete[] m_vertices;
	m_vertices = NULL;
}

void LMesh::freeTextCoords(){
	delete [] m_uv;
	m_uv = NULL;
}

int LMesh::GetUVCount()
{
    return m_uvCount;
}

int LMesh::GetTriangleCount()
{
    return m_triangleCount;
}

int LMesh::GetFaceCount()
{
    return m_triangleCount;
}

LFace LMesh::GetFace(int index)
{
    LFace f;
    LTriangle t = GetTriangle(index);
    f.vertices[0] = GetVertex(t.a);
    f.vertices[1] = GetVertex(t.b);
    f.vertices[2] = GetVertex(t.c);

    f.normals[0] = GetNormal(t.a);
    f.normals[1] = GetNormal(t.b);
    f.normals[2] = GetNormal(t.c);
    
    f.uv[0] = GetUV(t.a);
    f.uv[1] = GetUV(t.b);
    f.uv[2] = GetUV(t.c);
    
    LVector3 a, b;

    a = SubtractVectors(f.vertices[1], f.vertices[0]);
    b = SubtractVectors(f.vertices[1], f.vertices[2]);

    f.normal = CrossProduct(b, a);

    f.normal = NormalizeVector(f.normal);

    return f;
}

LMatrix LMesh::GetMatrix()
{
    return m_matrix;
}

void LMesh::TransformVertices(bool value)
{
    m_transform = value;
}

void LMesh::SetMatrix(LMatrix m)
{
    m_matrix = m;
}

int LMesh::GetNormalCount()
{
    return m_normalCount;
}

LVector3 LMesh::GetNormal(int index)
{
    int i;
    LVector3 vec;
    vec.x = vec.y = vec.z = 0.0f;
    if (index>=m_normalCount)
        return vec;
    if (index < 0) 
        i = 0;
    else
        i = index;
    
    if (m_transform)
        vec = VectorByMatrix(m_matrix, m_normals[i]);
    else
        vec = m_normals[i];
    return vec;
}

void LMesh::AddNormal(LVector3 vec)
{
    m_normalCount ++;
    m_normals = (LVector3*) realloc(m_normals, m_normalCount*sizeof(LVector3));
    m_normals[m_normalCount-1].x = vec.x;
    m_normals[m_normalCount-1].y = vec.y;
    m_normals[m_normalCount-1].z = vec.z;
}

void LMesh::CalcNormals()
{
    LVector3 vertex;
    LVector3 normal;
    int i, k, j;
    if (m_vertexCount <= 0)
        return;
    m_normalCount = m_vertexCount;
    m_normals = (LVector3*) malloc(m_vertexCount*sizeof(LVector3));

    for (i=0; i<m_vertexCount; i++)
    {
        normal.x = 0.0;
        normal.y = 0.0;
        normal.z = 0.0;
        vertex = m_vertices[i];
        // find all vertices with the same coords
        for (k=0; k<m_vertexCount; k++)
        {
            if ((fabs(vertex.x - m_vertices[k].x) < 0.0001) &&
                (fabs(vertex.y - m_vertices[k].y) < 0.0001) &&
                (fabs(vertex.z - m_vertices[k].z) < 0.0001))
            {
                for (j=0; j<m_triangleCount; j++)
                {
                    if ((m_triangles[j].a == (unsigned int)k) ||
                        (m_triangles[j].b == (unsigned int)k) ||
                        (m_triangles[j].c == (unsigned int)k))
                    {
                        LVector3 a, b, n;
                        a = SubtractVectors(m_vertices[m_triangles[j].b], m_vertices[m_triangles[j].a]);
                        b = SubtractVectors(m_vertices[m_triangles[j].b], m_vertices[m_triangles[j].c]);
                        n = CrossProduct(b, a);
                        n = NormalizeVector(n);
                        normal = AddVectors(normal, n);
                    }
                }
            }
        }
        m_normals[i] = NormalizeVector(normal);

    }
}




//-------------------------------------------------------
// LLight implementation
//-------------------------------------------------------

LLight::LLight()
:   LObject()
{
    Clear();
}

LLight::~LLight()
{

}

void LLight::Clear()
{
    m_pos.x = m_pos.y = m_pos.z = 0.0f;
    m_color.r = m_color.g = m_color.b = 0.0f;
    m_spotlight = false;
}

void LLight::SetPosition(LVector3 vec)
{
    m_pos = vec;
}

LVector3 LLight::GetPosition()
{
    return m_pos;
}

void LLight::SetColor(LColor3 color)
{
    m_color = color;
}

LColor3 LLight::GetColor()
{
    return m_color;
}

void LLight::SetSpotlight(bool value)
{
    m_spotlight = value;
}

bool LLight::GetSpotlight()
{
    return m_spotlight;
}

void LLight::SetTarget(LVector3 target)
{
    m_target = target;
}

LVector3 LLight::GetTarget()
{
    return m_target;
}

void LLight::SetHotspot(float value)
{
    m_hotspot = value;
}

float LLight::GetHotspot()
{
    return m_hotspot;
}

void LLight::SetFalloff(float value)
{
    m_falloff = value;
}
    
float LLight::GetFalloff()
{
    return m_falloff;
}

//-------------------------------------------------------
// L3DS implementation
//-------------------------------------------------------

L3DS::L3DS()
{
    m_loaded = false;
    m_buffer = 0;
    m_bufferSize = 0;
    m_pos = 0;
    m_eof = false;
    m_lights = 0;
    m_lightCount = 0;
    m_meshes = 0;
    m_meshCount = 0;
} 

L3DS::L3DS(const char *filename)
{
    m_loaded = false;
    m_buffer = 0;
    m_bufferSize = 0;
    m_pos = 0;
    m_eof = false;
    m_lights = 0;
    m_lightCount = 0;
    m_meshes = 0;
    m_meshCount = 0;
    LoadFile(filename);
}

L3DS::~L3DS()
{
    Clear();
}

bool L3DS::LoadFile(const char *filename)
{
    if (m_loaded)
        Clear();
    FILE *f;
    f = fopen(filename, "rb");
    if (f == 0)
        return false;
    fseek(f, 0, SEEK_END);
    m_bufferSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    m_buffer = (unsigned char*) malloc(m_bufferSize);
    if (fread(m_buffer, m_bufferSize, 1, f) != 1)
    {
        fclose(f);
        free(m_buffer);
        m_bufferSize = 0;
        return false;
    }
    fclose(f);
    m_loaded = true;
    bool res = Read3DS();
    free(m_buffer);
    m_buffer = 0;
    m_bufferSize = 0;
    return res;
}

void L3DS::Clear()
{
    int i;
    m_loaded = false;
    if ((m_bufferSize > 0) && (m_buffer != 0))
    {
        free(m_buffer);
        m_buffer = 0;
    }
    m_bufferSize = 0;
    m_pos = 0;
    m_eof = false;
    if ((m_lights != 0) && (m_lightCount > 0))
    {
        for (i=0; i<m_lightCount; i++)
            delete m_lights[i];
        free(m_lights);
        m_lights = 0;
        m_lightCount = 0;
    }
    if ((m_meshes != 0) && (m_meshCount > 0))
    {
        for (i=0; i<m_meshCount; i++)
            delete m_meshes[i];
        free(m_meshes);
        m_meshes = 0;
        m_meshCount = 0;
    }
}

short L3DS::ReadShort()
{
    if ((m_buffer!=0) && (m_bufferSize != 0) && ((m_pos+2)<m_bufferSize))
    {
        short *w = (short*)(m_buffer+m_pos);
        short s = *w;//(short)*(m_buffer+m_pos);
        m_pos += 2;
        return s;
    }
    m_eof = true;
    return 0;
}
 
int L3DS::ReadInt()
{
    if ((m_buffer!=0) && (m_bufferSize != 0) && ((m_pos+4)<m_bufferSize))
    {
        int *w = (int*)(m_buffer+m_pos);
        int s = *w;//(int)*(m_buffer+m_pos);
        m_pos += 4;
        return s;
    }
    m_eof = true;
    return 0;
}

char L3DS::ReadChar()
{
    if ((m_buffer!=0) && (m_bufferSize != 0) && ((m_pos+1)<m_bufferSize))
    {
        char s = (char)*(m_buffer+m_pos);
        m_pos += 1;
        return s;
    }
    m_eof = true;
    return 0;
}

float L3DS::ReadFloat()
{
    if ((m_buffer!=0) && (m_bufferSize != 0) && ((m_pos+4)<m_bufferSize))
    {
        float *w = (float*)(m_buffer+m_pos);
        float s = *w;//(float)*(m_buffer+m_pos);
        m_pos += 4;
        return s;
    }
    m_eof = true;
    return 0.0;
}

void L3DS::ReadASCIIZ(char *buf, int &count, int max_count)
{
    if ((m_buffer==0) || (m_bufferSize == 0) || (m_pos>=m_bufferSize))
    {
        count = 0;
        m_eof = true;
        return;
    }
    count = 0;
    char c = ReadChar();
    while ((c!=0) && (count<max_count-1))
    {
        buf[count] = c;
        count ++;
        c = ReadChar();
    }
    buf[count] = 0;
}

void L3DS::Seek(int offset, int origin)
{
    if (origin == SEEK_START)
        m_pos = offset;
    if (origin == SEEK_CURSOR)
        m_pos += offset;
    if (m_pos < 0)
        m_pos = 0;
    if (m_pos >= m_bufferSize)
        m_pos = m_bufferSize-1;
    m_eof = false;
}

int L3DS::Pos()
{
    return m_pos;
}

void L3DS::AddMesh()
{
    m_meshCount++;
    m_meshes = (LMesh**) realloc(m_meshes, m_meshCount*sizeof(LMesh*));
    m_meshes[m_meshCount-1] = new LMesh();
}

void L3DS::AddLight()
{
    m_lightCount++;
    m_lights = (LLight**) realloc(m_lights, m_lightCount*sizeof(LLight*));
    m_lights[m_lightCount-1] = new LLight();
}

int L3DS::GetMeshCount()
{
    return m_meshCount;
}
    
int L3DS::GetLightCount()
{
    return m_lightCount;
}

LMesh* L3DS::GetMesh(int index)
{
    if ((index<0) ||(index>= m_meshCount))
        return 0;
    return m_meshes[index];
}

LLight* L3DS::GetLight(int index)
{
    if ((index<0) ||(index>= m_lightCount))
        return 0;
    return m_lights[index];
}

bool L3DS::Read3DS()
{
    LChunk mainchunk;
    LChunk edit;
    edit.id = EDIT3DS;
    mainchunk = ReadChunk();
    if (mainchunk.id != MAIN3DS)
        return false;
    if (!FindChunk(edit, mainchunk))
        return false;
    LChunk obj;
    LChunk ml;
    int d;
    obj.id = EDIT_OBJECT;
    {
        while (FindChunk(obj, edit))
        {
            ReadASCIIZ(m_objName, d, 99);
            ml = ReadChunk();
            if (ml.id == OBJ_TRIMESH)
                ReadMesh(obj);
            if (ml.id == OBJ_LIGHT)
                ReadLight(obj);
            SkipChunk(obj);
        }
    }

    return true;
}

LChunk L3DS::ReadChunk()
{
    LChunk chunk;
    chunk.id = ReadShort();
    int a = ReadInt();
    chunk.start = Pos();
    chunk.end = chunk.start+a-6;
    return chunk;
}

bool L3DS::FindChunk(LChunk &target, LChunk parent)
{
    if (Pos() >= parent.end)
        return false;
    LChunk chunk;
    chunk = ReadChunk();
    while (( chunk.id != target.id) && (chunk.end <= parent.end))
    {
        SkipChunk(chunk);
        if (chunk.end >= parent.end)
            break;
        chunk = ReadChunk();
    }
    if (chunk.id == target.id)
    {
        target.start = chunk.start;
        target.end = chunk.end;
        return true;
    }
    return false;
}

void L3DS::SkipChunk(LChunk chunk)
{
    Seek(chunk.end, SEEK_START);
}

void L3DS::ReadLight(LChunk parent)
{
    LColor3 c;
    float t;
    LVector3 v;
    AddLight();
    m_lights[m_lightCount-1]->SetName(m_objName);
    LChunk chunk = ReadChunk();
    if (chunk.id == OBJ_LIGHT)
    {
        v.x = ReadFloat();
        v.y = ReadFloat();
        v.z = ReadFloat();
        if (chunk.end < parent.end)
            chunk = ReadChunk();
        while (chunk.end <= parent.end)
        {
            switch (chunk.id)
            {
            case RGB_COLOR:
                c.r = ReadFloat();
                c.g = ReadFloat();
                c.b = ReadFloat();
                m_lights[m_lightCount-1]->SetColor(c);
                break;
            case SPOTLIGHT:
                v.x = ReadFloat();
                v.y = ReadFloat();
                v.z = ReadFloat();
                m_lights[m_lightCount-1]->SetTarget(v);
                t = ReadFloat();
                m_lights[m_lightCount-1]->SetHotspot(t);
                t = ReadFloat();
                m_lights[m_lightCount-1]->SetFalloff(t);
                break;
            default:
                break;
            }
            SkipChunk(chunk);
            if (chunk.end >= parent.end)
                break;
            chunk = ReadChunk();

        }
    }
}

void L3DS::ReadMesh(LChunk parent)
{
    unsigned short count, i, a, b, c;
    LVector3 p;
    LMatrix m;
    LUV t;
    AddMesh();
    m_meshes[m_meshCount-1]->SetName(m_objName);
    LChunk chunk = ReadChunk();
    while (chunk.end <= parent.end)
    {
        switch (chunk.id)
        {
        case TRI_VERTEXLIST:
            count = ReadShort();
            for (i=0; i < count; i++)
            {
                p.x = ReadFloat();
                p.y = ReadFloat();
                p.z = ReadFloat();
                m_meshes[m_meshCount-1]->AddVertex(p);
            }
            break;
        case TRI_FACEMAPPING:
            count = ReadShort();
            for (i=0; i < count; i++)
            {
                t.u = ReadFloat();
                t.v = ReadFloat();
                m_meshes[m_meshCount-1]->AddUV(t);
            }
            break;
        case TRI_FACELIST:
            count = ReadShort();
            for (i=0; i < count; i++)
            {
                a = ReadShort();
                b = ReadShort();
                c = ReadShort();
                ReadShort(); // we actually dont need this variable
                m_meshes[m_meshCount-1]->AddTriangle(a, b, c);
            }
            break;
        case TRI_MATRIX:
            m.xAxis.x = ReadShort();
            m.xAxis.y = ReadShort();
            m.xAxis.z = ReadShort();

            m.yAxis.x = ReadShort();
            m.yAxis.y = ReadShort();
            m.yAxis.z = ReadShort();

            m.zAxis.x = ReadShort();
            m.zAxis.y = ReadShort();
            m.zAxis.z = ReadShort();

            m.pos.y = ReadShort();
            m.pos.x = ReadShort();
            m.pos.z = ReadShort();

            m_meshes[m_meshCount-1]->SetMatrix(m);
            break;
        default:
            break;
        }
        SkipChunk(chunk);
        if (chunk.end >= parent.end)
            break;
        chunk = ReadChunk();
    }
    m_meshes[m_meshCount-1]->CalcNormals();
}
