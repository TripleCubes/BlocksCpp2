#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player: public Entity
{
    private:

    public:
        using Entity::Entity;
        
        void init(Vec3 pos, Vec3 frontDir);

        void release();
};

#endif