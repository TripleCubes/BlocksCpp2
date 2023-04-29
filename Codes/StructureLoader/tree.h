#ifndef TREE_H
#define TREE_H

#include <vector>
#include "../ChunkLoader/block.h"
#include "../ChunkLoader/chunk.h"
#include "../Types/intPos.h"
#include <unordered_map>
#include <string>

namespace Structure
{
    class Tree 
    {
        private:
            static int checkRadius;

            static std::unordered_map<std::string, IntPos> checkedChunks;

            static std::string convertToKey(IntPos chunk2dPos);



            std::vector<IntPos> belongToChunks;
            std::vector<std::vector<Block>> blockList;

            bool blockExist(IntPos pos);
            int ringAdd(int a, int b);
            int ringMiddle(int a, int b);
            void addBlock(Block block);

        public:
            static void checkLoad(IntPos chunk2dPos);
            static void removeCheckedMarks();
            void init(IntPos pos);
            void place();
            bool place(Chunk &chunk);
    };
}

#endif
