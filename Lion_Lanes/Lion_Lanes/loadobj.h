#pragma once

typedef struct {
float x, y, z;
} Vertex, Normal;

typedef struct {
float u, v;
} TexCoord;

typedef struct {
int * vIndices, * nIndices, * tcIndices;
int vCount;
} Face;




typedef struct _Mesh {
Vertex * vertices;
Normal * normals;
TexCoord * texCoords;
Face * faces;
int noVertices, noNormals, noTexCoords, noFaces;
} Mesh;



Mesh * LoadObj(char *filename);