#pragma once

// ====== Terrain ====== //
//Mesh *mesh_TerrainTest;
TextureImage tex_Terrain;

// ====== Objects ====== //

#include "OTemplate.h"

#include "ODragon.h"
ODragon LocalPlayer;				// There can be only one! No array needed.
//Mesh *mesh_Dragon;
TextureImage tex_Dragon;
float time_taken = 0;

#include "ORock.h"
int MAXROCKS = 25;				// Increases as more are created, decreases as instances are destroyed
int iter_obj_Rocks = 0;							// Array iterator
ORock* obj_Rocks = new ORock[MAXROCKS];					// Declare the object's instancing array and initialise to NULL
TextureImage tex_Rock;
int rocks_left;

#include "Projectile.h"
int m_iProjectileIndex = 0;						// Array iterator
const static int MAXPROJECTILES = 25;		// Optional: Limit the number of instances
CProjectile* theProjectiles = new CProjectile[MAXPROJECTILES];			// Declare the object's instancing array and initialise
