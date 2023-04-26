#include "blockTextures.h"

Texture GameTextures::BlockTextures::texture;
const int GameTextures::BlockTextures::blockPixelSize = 8;

void GameTextures::BlockTextures::init()
{
    texture.load("./Textures/blocks.png", FilterType::NEAREST);
}

Texture GameTextures::BlockTextures::getTexture()
{
    return texture;
}

Vec2 GameTextures::BlockTextures::getUV(BlockType blockType, BlockDir blockDir)
{
    Vec2 oneBlockTextureSize(1.0f/(float)texture.getWidth(), 1.0f/(float)texture.getHeight());
    oneBlockTextureSize = oneBlockTextureSize * (float)blockPixelSize;

    switch (blockType)
    {
    case BlockType::STONE:
        return Vec2(0, 0) * oneBlockTextureSize;
        break;

    case BlockType::DIRT:
        return Vec2(1, 0) * oneBlockTextureSize;
        break;

    case BlockType::GRASS:
        if (blockDir == BlockDir::TOP)
        {
            return Vec2(3, 0) * oneBlockTextureSize;
        }
        if (blockDir == BlockDir::BOTTOM)
        {
            return Vec2(1, 0) * oneBlockTextureSize;
        }
        return Vec2(2, 0) * oneBlockTextureSize;
        break;
    
    default:
        return Vec2(0, 0);
        break;
    }
}

void GameTextures::BlockTextures::release()
{
    texture.release();
}
