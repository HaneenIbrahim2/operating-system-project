struct MemBlock* alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");

	struct MemBlock* iterator;

	LIST_FOREACH(iterator, &FreeMemBlocksList) {
		if (size == iterator->size) {
			LIST_REMOVE(&FreeMemBlocksList, iterator);
			return iterator;
		}
		else if (size < iterator->size) {
			struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
			newBlock->size = size;
			newBlock->sva = iterator->sva;
			iterator->size = iterator->size - size;
			iterator->sva = iterator->sva + size;
			LIST_REMOVE(&AvailableMemBlocksList, newBlock);
			return newBlock;
		}
	}
	return NULL;
}