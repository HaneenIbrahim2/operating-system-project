void free(void* virtual_address) {

	struct MemBlock * newBlock = NULL;
	newBlock = find_block(&AllocMemBlocksList, (uint32) virtual_address);
	
	if (newBlock != NULL) {
		sys_free_user_mem(ROUNDDOWN((uint32 )virtual_address, PAGE_SIZE),
				newBlock->size);
		LIST_REMOVE(&AllocMemBlocksList, newBlock);
		insert_sorted_with_merge_freeList(newBlock);
	}
	
}