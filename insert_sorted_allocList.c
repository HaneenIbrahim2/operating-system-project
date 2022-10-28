void insert_sorted_allocList(struct MemBlock* blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code

	if (LIST_EMPTY(&AllocMemBlocksList)) {     //if empty -> true(1)
		LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
	}
	else {
		struct MemBlock* block;
		bool found = 0;
		LIST_FOREACH(block, &AllocMemBlocksList) {
			if (blockToInsert->sva < block->sva) {
				LIST_INSERT_BEFORE(&AllocMemBlocksList, block, blockToInsert);
				found = 1;
				break;
			}

		}
		if (found == 0) {
			LIST_INSERT_TAIL(&AllocMemBlocksList, blockToInsert);
		}
	}
}