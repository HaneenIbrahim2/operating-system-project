struct MemBlock* find_block(struct MemBlock_List* blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code

	struct MemBlock* block;
	int found = 0;
	LIST_FOREACH(block, blockList) {
		if (va == block->sva) {
			found = 1;
			return block;
		}
	}
	if (found == 0) {
		return NULL;
	}
	return 0;

}