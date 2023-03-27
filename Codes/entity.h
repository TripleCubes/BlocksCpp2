#ifndef ENTITY_H
#define ENTITY_H

#include "vectormath.h"
#include "mesh.h"
#include "texture.h"

class Entity
{
    private:
        float internalVelocityCap = 0;
        float internalVelocityAddAmount = 0;
        float internalVelocitySlowDownAmount = 0;

        float externalVelocityCap = 0;
        float externalVelocitySlowDownAmount = 0;
        float gravityVelocityAddAmount = 0;
        float jumpVelocityAddAmount = 0;

        bool isOnGround = false;

        Mesh playerMesh;
        Texture playerTexture;
        Vec3 internalVelocity;
        Vec3 externalVelocity;

        void moveX(float moveAmount);
        void moveY(float moveAmount);
        void moveZ(float moveAmount);

        void slowDownVelocity(Vec3 &velocity, float velocitySlowDownAmount);
        void addVelocity(Vec3 &velocity, Vec3 velocityAddDir, float velocityAddAmount, float velocityCap);
        void addGravity();

    public:
        Vec3 pos;
        Vec3 frontDir;
        
        void init(Vec3 pos, Vec3 frontDir);
        void update();
        void draw();

        void move(Vec3 dir);
        void jump();

        void release();
};

#endif