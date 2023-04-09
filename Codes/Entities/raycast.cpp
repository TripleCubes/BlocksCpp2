#include "raycast.h"
#include "../math.h"
#include "../ChunkLoader/chunkLoader.h"

BlockRaycast::BlockRaycast(Vec3 from, Vec3 dir, float range)
{
	IntPos previousBlockPos = {(int)floor(from.x), (int)floor(from.y), (int)floor(from.z)};

	Vec3 currentPos = from;
	Vec3 dirSign = {0, 0, 0};
	if (dir.x > 0)
    {
		dirSign.x = 1;
    }
	else if (dir.x < 0)
    {
		dirSign.x = -1;	
    }
	if (dir.y > 0)
    {
		dirSign.y = 1;
    }
	else if (dir.y < 0)
    {
		dirSign.y = -1;	
    }
	if (dir.z > 0)
    {
		dirSign.z = 1;
    }
	else if (dir.z < 0)
    {
		dirSign.z = -1;	
    }

	int count = 0;
	while (distance(from, currentPos) <= range)
    {
		count += 1;
		Vec3 distanceToNextBlock = {-1, -1, -1};

		if (dir.x > 0)
        {
			distanceToNextBlock.x = ceil(currentPos.x) - currentPos.x + 0.001;
        }
		else if (dir.x < 0)
        {
			distanceToNextBlock.x = currentPos.x - floor(currentPos.x) + 0.001;
        }
		if (dir.y > 0)
        {
			distanceToNextBlock.y = ceil(currentPos.y) - currentPos.y + 0.001;
        }
		else if (dir.y < 0)
        {
			distanceToNextBlock.y = currentPos.y - floor(currentPos.y) + 0.001;
        }
		if (dir.z > 0)
        {
			distanceToNextBlock.z = ceil(currentPos.z) - currentPos.z + 0.001;
        }
		else if (dir.z < 0)
        {
			distanceToNextBlock.z = currentPos.z - floor(currentPos.z) + 0.001;
        }

		Vec3 distanceDivDir = {0, 0, 0};
		if (dir.x != 0)
        {
			distanceDivDir.x = abs(distanceToNextBlock.x / dir.x);
        }
		if (dir.y != 0)
        {
			distanceDivDir.y = abs(distanceToNextBlock.y / dir.y);
        }
		if (dir.z != 0)
        {
			distanceDivDir.z = abs(distanceToNextBlock.z / dir.z);
        }

		if (isMin(distanceDivDir.x, distanceDivDir.y, distanceDivDir.z))
        {
			currentPos.x += distanceToNextBlock.x * dirSign.x;
			if (dir.x != 0)
            {
				currentPos.y += distanceToNextBlock.x / abs(dir.x) * abs(dir.y) * dirSign.y;
				currentPos.z += distanceToNextBlock.x / abs(dir.x) * abs(dir.z) * dirSign.z;
            }
        }
		else if (isMin(distanceDivDir.y, distanceDivDir.x, distanceDivDir.z))
        {
			currentPos.y += distanceToNextBlock.y * dirSign.y;
			if (dir.y != 0)
            {
				currentPos.x += distanceToNextBlock.y / abs(dir.y) * abs(dir.x) * dirSign.x;
				currentPos.z += distanceToNextBlock.y / abs(dir.y) * abs(dir.z) * dirSign.z;
            }
        }
		else
        {
			currentPos.z += distanceToNextBlock.z * dirSign.z;
			if (dir.z != 0)
            {
				currentPos.x += distanceToNextBlock.z / abs(dir.z) * abs(dir.x) * dirSign.x;
				currentPos.y += distanceToNextBlock.z / abs(dir.z) * abs(dir.y) * dirSign.y;
            }
        }
		
        IntPos currentBlockPos((int)floor(currentPos.x), (int)floor(currentPos.y), (int)floor(currentPos.z));
        Block currentBlock = ChunkLoader::getBlock(currentBlockPos);
		if (currentBlock.blockType != EMPTY)
        {
			found = true;
			selectedBlock = currentBlock;
			selectedBlockPos = currentBlockPos;
			blockPlacingPos = previousBlockPos;
			break;
        }
		
		previousBlockPos = currentBlockPos;
    }
}

BlockRaycast::BlockRaycast() {}