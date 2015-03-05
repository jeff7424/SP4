#include "loadobj.h"
#include "freeglut.h"
#include <fstream>

Mesh * LoadObj(char *filename)
{
	
	int vc = 0, nc = 0, tc = 0, fc = 0;
	char buf [256];
	FILE * fp = fopen (filename, "r");
	Mesh * pMesh = (Mesh *) malloc (sizeof (Mesh));
	pMesh->noNormals = pMesh->noTexCoords = pMesh->noVertices = pMesh->noFaces = 
	0;
	// initialise values in (*pMesh)
	// Count elements
	while (!feof(fp)) {
		if (!fgets (buf, 256, fp)) break; // read one line
		if (!strncmp ("vn ", buf, 3)) pMesh->noNormals ++;
		else if (!strncmp ("vt ", buf, 3)) pMesh->noTexCoords ++;
		else if (!strncmp ("v ", buf, 2)) pMesh->noVertices ++;
		else if (!strncmp ("f ", buf, 2)) pMesh->noFaces ++;
	}

	pMesh->vertices = (Vertex *) malloc (pMesh->noVertices * sizeof (Vertex));
	pMesh->normals = (Normal *) malloc (pMesh->noNormals * sizeof (Normal));
	pMesh->texCoords = (TexCoord *) malloc (pMesh->noTexCoords * sizeof (TexCoord));
	pMesh->faces = (Face *) malloc (pMesh->noFaces * sizeof (Face));

	fseek (fp, 0, SEEK_SET); // reset file ptr to beginning
	while (!feof (fp)) {
	fgets (buf, 256, fp);
	if (!strncmp ("vn ", buf, 3))
		{
			sscanf ((buf + 2), "%f%f%f",
			&pMesh->normals[nc].x,
			&pMesh->normals[nc].y,
			&pMesh->normals[nc].z);
			nc++;	
		}// process vn
	else if (!strncmp ("vt ", buf, 3))
		{
			sscanf ((buf + 2), "%f%f",
			&pMesh->normals[tc].x,
			&pMesh->normals[tc].y);
			tc++;	
		}// process vt
	else if (!strncmp ("v ", buf, 2))
		{
			sscanf ((buf + 2), "%f%f%f",
			&pMesh->vertices[vc].x,
			&pMesh->vertices[vc].y,
			&pMesh->vertices[vc].z);
			vc++;	
		}// process v
	else if (!strncmp ("f ", buf, 2))
		{
			char * pSp = NULL;
			int ii = 0;
			Face * pf = &pMesh->faces[fc];

			for (int i = 0; i < strlen (buf); i ++)
				if (buf[i] == '/') ii ++; // no of '/'
			pf->vCount = ii / 2; // no of vertices
			pf->vIndices = (int *) malloc (pf->vCount * sizeof (int));
			pf->nIndices = (int *) malloc (pf->vCount * sizeof (int));
			pf->tcIndices = (int *) malloc (pf->vCount * sizeof (int));
			pSp = strtok ((buf + 2), " \t\n");
			for (int i = 0; pSp; pSp = strtok (NULL, " \t\n"), i ++) {
				sscanf (pSp, "%d/%d/%d",
					&pf->vIndices[i],
					&pf->tcIndices[i],
					&pf->nIndices[i]);
				pf->vIndices[i] --;
				pf->tcIndices[i] --;
				pf->nIndices[i] --;
			}
			fc ++;
		}// process f
	}
	fclose (fp); // close file

	

	return pMesh;

}