#include "player.h"

void Player::init(Vec3 pos, Vec3 frontDir)
{
    Entity::init(pos, frontDir);
}

void Player::release()
{
    Entity::release();
}