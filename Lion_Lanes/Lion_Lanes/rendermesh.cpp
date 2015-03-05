/*

Nominally place in main.cpp

Instructions:

- Declare Mesh Objects to hold models
- In init function, load obj model files into Mesh Objects
- In MyDisplay function, call RenderMesh(meshname) to render models, after appropriate glPushMatrix, glTranslatef, glScalef and glRotatef, glPopMatrix and etc

*/

#include <math.h>
#include "freeglut.h"
#include "loadTGA.h"
#include "loadobj.h"

void RenderMesh(Mesh *cur)
{
	for (int i = 0; i < cur->noFaces; i ++)
	{
		Face * pf = &cur->faces[i];
		glBegin (GL_POLYGON);
		for (int j = 0; j < pf->vCount; j ++)
		{
			glTexCoord2f (cur->texCoords[pf->tcIndices[j]].u, cur->texCoords[pf->tcIndices[j]].v);
			glNormal3f (cur->normals[pf->nIndices[j]].x, cur->normals[pf->nIndices[j]].y, cur->normals[pf->nIndices[j]].z);
			glVertex3f (cur->vertices[pf->vIndices[j]].x, cur->vertices[pf->vIndices[j]].y, cur->vertices[pf->vIndices[j]].z);
		}
		glEnd ();
	}
}