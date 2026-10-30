// retruxx: the game's height-map terrain geom, with y up (Benoit CHAPEROT's dTerrainY, as changed
// for the shipped build). It is a built-in class, dTerrainYClass, collided against spheres, boxes,
// capsules and rays.

#ifndef _ODE_COLLISION_TERRAIN_Y_H_
#define _ODE_COLLISION_TERRAIN_Y_H_

#include <ode/common.h>
#include "collision_kernel.h"

struct dxRay;

struct dxTerrainY : public dxGeom {
    dReal m_vLength;
    dReal *m_pHeights;
    dReal m_vMinHeight;
    dReal m_vMaxHeight;
    dReal m_vNodeLength;
    dReal m_vNodeLengthInv;
    int m_nNumNodesPerSide;
    int m_nNumNodesPerSideShift;
    int m_nNumNodesPerSideMask;
    int m_bFinite;
    dxRay *m_subdivisionRay;

    dxTerrainY(dSpaceID space, dReal *pHeights, dReal vLength, int nNumNodesPerSide, int bFinite, int bPlaceable);
    ~dxTerrainY();
    void computeAABB();
    dReal GetHeight(dReal x, dReal z);
    dReal GetHeight(int x, int z);
    int dCollideTerrainUnit(int x, int z, dxGeom *o2, int numMaxContacts, int flags, dContactGeom *contact, int skip);
    bool IsOnTerrain(int nx, int nz, int w, dReal *pos);
};

int dCollideTerrainY(dxGeom *o1, dxGeom *o2, int flags, dContactGeom *contact, int skip);

#endif
