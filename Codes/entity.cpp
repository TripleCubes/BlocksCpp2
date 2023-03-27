#include <vector>

#include "entity.h"
#include "chunkLoader.h"
#include "globals.h"
#include "graphics.h"

void Entity::init(Vec3 pos, Vec3 frontDir)
{
    this->pos = pos;
    this->frontDir = frontDir;
    
    std::vector<float> verticies = {
        -0.5,    2, -0.5,    0,    1,    0,    0,    1, // A
         0.5,    2,  0.5,    0,    1,    0,    1,    0, // C
        -0.5,    2,  0.5,    0,    1,    0,    0,    0, // D

        -0.5,    2, -0.5,    0,    1,    0,    0,    1, // A
         0.5,    2, -0.5,    0,    1,    0,    1,    1, // B
         0.5,    2,  0.5,    0,    1,    0,    1,    0, // C

        -0.5,    0, -0.5,    0,   -1,    0,    1,    1, // E
        -0.5,    0,  0.5,    0,   -1,    0,    1,    0, // H
         0.5,    0,  0.5,    0,   -1,    0,    0,    0, // G

        -0.5,    0, -0.5,    0,   -1,    0,    1,    1, // E
         0.5,    0,  0.5,    0,   -1,    0,    0,    0, // G
         0.5,    0, -0.5,    0,   -1,    0,    0,    1, // F

        -0.5,    2, -0.5,   -1,    0,    0,    0,    1, // A
        -0.5,    2,  0.5,   -1,    0,    0,    1,    1, // D
        -0.5,    0,  0.5,   -1,    0,    0,    1,    0, // H

        -0.5,    2, -0.5,   -1,    0,    0,    0,    1, // A
        -0.5,    0,  0.5,   -1,    0,    0,    1,    0, // H
        -0.5,    0, -0.5,   -1,    0,    0,    0,    0, // E

         0.5,    2, -0.5,    1,    0,    0,    1,    1, // B
         0.5,    0,  0.5,    1,    0,    0,    0,    0, // G
         0.5,    2,  0.5,    1,    0,    0,    0,    1, // C

         0.5,    2, -0.5,    1,    0,    0,    1,    1, // B
         0.5,    0, -0.5,    1,    0,    0,    1,    0, // F
         0.5,    0,  0.5,    1,    0,    0,    0,    0, // G

        -0.5,    2,  0.5,    0,    0,    1,    0,    1, // D
         0.5,    2,  0.5,    0,    0,    1,    1,    1, // C
        -0.5,    0,  0.5,    0,    0,    1,    0,    0, // H

         0.5,    2,  0.5,    0,    0,    1,    1,    1, // C
         0.5,    0,  0.5,    0,    0,    1,    1,    0, // G
        -0.5,    0,  0.5,    0,    0,    1,    0,    0, // H

        -0.5,    2, -0.5,    0,    0,   -1,    1,    1, // A
        -0.5,    0, -0.5,    0,    0,   -1,    1,    0, // E
         0.5,    2, -0.5,    0,    0,   -1,    0,    1, // B

         0.5,    2, -0.5,    0,    0,   -1,    0,    1, // B
        -0.5,    0, -0.5,    0,    0,   -1,    1,    0, // E
         0.5,    0, -0.5,    0,    0,   -1,    0,    0, // F
    };

    playerMesh.set(verticies);

    playerTexture.load("./Textures/player.png", NEAREST);
}

void Entity::draw()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(pos.x, pos.y, pos.z));
    Graphics::getViewShader().useProgram();
    Graphics::getViewShader().setUniform("modelMat", modelMat);
    Graphics::getViewShader().setUniform("testTexture", playerTexture, 0);
    playerMesh.draw();
}

void Entity::moveX(float moveAmount)
{
    if (moveAmount == 0)
    {
        return;
    }

    Vec3 nextPos = Vec3(pos.x + moveAmount, pos.y, pos.z);
    int xCheckPos = 0;

    if (moveAmount > 0)
    {
        xCheckPos = floor(nextPos.x + 0.5);
    }
    else if (moveAmount < 0)
    {
        xCheckPos = floor(nextPos.x - 0.5);
    }

    std::vector<IntPos> checkBlockPositions = {
        IntPos(xCheckPos, floor(nextPos.y + 0.001), floor(nextPos.z - 0.5 + 0.001)),
        IntPos(xCheckPos, floor(nextPos.y + 0.001), floor(nextPos.z + 0.5 - 0.001)),
        IntPos(xCheckPos, floor(nextPos.y + 1    ), floor(nextPos.z - 0.5 + 0.001)),
        IntPos(xCheckPos, floor(nextPos.y + 1    ), floor(nextPos.z + 0.5 - 0.001)),
        IntPos(xCheckPos, floor(nextPos.y + 1.999), floor(nextPos.z - 0.5 + 0.001)),
        IntPos(xCheckPos, floor(nextPos.y + 1.999), floor(nextPos.z + 0.5 - 0.001)),
    };

    bool blockFound = false;
    for (int i = 0; i < checkBlockPositions.size(); i++)
    {
        Block checkBlock = ChunkLoader::getBlock(checkBlockPositions[i]);
        if (checkBlock.blockType != EMPTY)
        {
            pos.x = floor(nextPos.x) + 0.5;
            blockFound = true;
            break;
        }
    }
    if (!blockFound)
    {
        pos.x += moveAmount;
    }
}

void Entity::moveY(float moveAmount)
{
    if (moveAmount == 0)
    {
        return;
    }

    Vec3 nextPos = Vec3(pos.x, pos.y + moveAmount, pos.z);
    int yCheckPos = 0;

    if (moveAmount > 0)
    {
        yCheckPos = floor(nextPos.y + 2);
    }
    else if (moveAmount < 0)
    {
        yCheckPos = floor(nextPos.y);
    }

    std::vector<IntPos> checkBlockPositions = {
        IntPos(floor(pos.x - 0.5 + 0.01), yCheckPos, floor(pos.z + 0.5 - 0.01)),
        IntPos(floor(pos.x - 0.5 + 0.01), yCheckPos, floor(pos.z - 0.5 + 0.01)),
        IntPos(floor(pos.x + 0.5 - 0.01), yCheckPos, floor(pos.z + 0.5 - 0.01)),
        IntPos(floor(pos.x + 0.5 - 0.01), yCheckPos, floor(pos.z - 0.5 + 0.01)),
    };

    bool blockFound = false;
    for (int i = 0; i < checkBlockPositions.size(); i++)
    {
        Block checkBlock = ChunkLoader::getBlock(checkBlockPositions[i]);
        if (checkBlock.blockType != EMPTY)
        {
            pos.y = floor(nextPos.y + 0.5);
            blockFound = true;
            break;
        }
    }
    if (!blockFound)
    {
        pos.y += moveAmount;
    }
}

void Entity::moveZ(float moveAmount)
{
    if (moveAmount == 0)
    {
        return;
    }

    Vec3 nextPos = Vec3(pos.x, pos.y, pos.z + moveAmount);
    int zCheckPos = 0;

    if (moveAmount > 0)
    {
        zCheckPos = floor(nextPos.z + 0.5);
    }
    else if (moveAmount < 0)
    {
        zCheckPos = floor(nextPos.z - 0.5);
    }

    std::vector<IntPos> checkBlockPositions = {
        IntPos(floor(pos.x - 0.5 + 0.001), floor(pos.y + 0.001), zCheckPos),
        IntPos(floor(pos.x + 0.5 - 0.001), floor(pos.y + 0.001), zCheckPos),
        IntPos(floor(pos.x - 0.5 + 0.001), floor(pos.y + 1    ), zCheckPos),
        IntPos(floor(pos.x + 0.5 - 0.001), floor(pos.y + 1    ), zCheckPos),
        IntPos(floor(pos.x - 0.5 + 0.001), floor(pos.y + 1.999), zCheckPos),
        IntPos(floor(pos.x + 0.5 - 0.001), floor(pos.y + 1.999), zCheckPos),
    };

    bool blockFound = false;
    for (int i = 0; i < checkBlockPositions.size(); i++)
    {
        Block checkBlock = ChunkLoader::getBlock(checkBlockPositions[i]);
        if (checkBlock.blockType != EMPTY)
        {
            pos.z = floor(nextPos.z) + 0.5;
            blockFound = true;
            break;
        }
    }
    if (!blockFound)
    {
        pos.z += moveAmount;
    }
}

void Entity::move(Vec3 moveVec)
{
    moveY(moveVec.y);
    moveX(moveVec.x);
    moveZ(moveVec.z);
}

void Entity::release()
{
    playerMesh.release();
    playerTexture.release();
}
