#ifndef CAMERA_H
#define CAMERA_H

#include "../Types/vectormath.h"
#include "entity.h"

struct Camera
{
    Vec3 pos;
    Vec3 lookDir;

    void init(Entity attachedEntity);
    void update(Entity attachedEntity);
};

#endif