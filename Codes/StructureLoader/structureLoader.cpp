#include "structureLoader.h"

std::vector<Structure::Tree> StructureLoader::structureList;

void StructureLoader::add(Structure::Tree structure)
{
    structureList.push_back(structure);
}

void StructureLoader::checkLoad(IntPos chunkPos)
{
    Structure::Tree::checkLoad(chunkPos);
}

void StructureLoader::removeCheckedMarks()
{
    Structure::Tree::removeCheckedMarks();
}

void StructureLoader::placeStructures(Chunk &chunk)
{
    for (int i = structureList.size() - 1; i >= 0; i--)
    {
        if (structureList[i].place(chunk))
        {
            structureList.erase(structureList.begin() + i);
        }
    }

    chunk.markStructuresAdded();
}
