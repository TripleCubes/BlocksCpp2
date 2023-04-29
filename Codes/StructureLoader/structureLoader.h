#ifndef STRUCTURELOADER_H
#define STRUCTURELOADER_H

#include "tree.h"
#include <vector>

class StructureLoader
{
    private:
        static std::vector<Structure::Tree> structureList;

    public:
        static void add(Structure::Tree structure);
        static void checkLoad(IntPos chunkPos);
        static void removeCheckedMarks();
        static void placeStructures(Chunk &chunk);
};

#endif
