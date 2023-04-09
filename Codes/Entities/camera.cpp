#include "camera.h"
#include "../globals.h"

void Camera::init(Entity attachedEntity)
{
    pos = attachedEntity.pos;
    lookDir = attachedEntity.frontDir;
}

void Camera::update(Entity attachedEntity)
{
    pos = attachedEntity.pos;
}