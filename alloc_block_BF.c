struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
    //panic("alloc_block_BF() is not implemented yet...!!");

	struct MemBlock *freeMemBlock;
	struct MemBlock *selectedMemBlock;

	int minMemBlockSize = 0;
	int isFirstElement = 0;

	LIST_FOREACH(freeMemBlock, &FreeMemBlocksList)
	{

		if(freeMemBlock->size == size) //if there a block with EXACT size
		{
			LIST_REMOVE(&FreeMemBlocksList, freeMemBlock);
			return freeMemBlock;
		}
		else if(freeMemBlock->size > size)
		{
			if(isFirstElement == 0) //when first iteration
			{
				minMemBlockSize = freeMemBlock->size - size;
				selectedMemBlock = freeMemBlock;
				isFirstElement++;
			}
			else if(isFirstElement != 0 && (freeMemBlock->size - size) < minMemBlockSize) //when it isn't a first iteration
			{
				minMemBlockSize = freeMemBlock->size - size;
				selectedMemBlock = freeMemBlock;
			}
		}
	}

	if(selectedMemBlock != NULL) // if we found block
	{ 
		struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
		newBlock->size = size;
		newBlock->sva = selectedMemBlock->sva;
		selectedMemBlock->size = selectedMemBlock->size-size;
		selectedMemBlock->sva = selectedMemBlock->sva+size;
		LIST_REMOVE(&AvailableMemBlocksList, newBlock);
		
		return newBlock;
	}
	return NULL;
}