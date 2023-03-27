#ifndef ENTITY_H
#define ENTITY_H

#include "vectormath.h"
#include "mesh.h"
#include "texture.h"

class Entity
{
    private:
        Mesh playerMesh;
        Texture playerTexture;
        void moveX(float moveAmount);
        void moveY(float moveAmount);
        void moveZ(float moveAmount);

    public:
        Vec3 pos;
        Vec3 frontDir;
        
        void init(Vec3 pos, Vec3 frontDir);
        void draw();

        void move(Vec3 moveVec);

        void release();
};

#endif