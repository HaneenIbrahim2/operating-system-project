void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
	
	InitializeUHeap();
	if (size == 0)
		return NULL;
	
	size = ROUNDUP(size, PAGE_SIZE);
	struct MemBlock* new_block = NULL;

	if (sys_isUHeapPlacementStrategyFIRSTFIT()) {
		
		new_block = alloc_block_FF(size);
		if (new_block == NULL) {
			return NULL;
		} 
		else {
			int shared_var_id = sys_createSharedObject(sharedVarName, size,
					isWritable, (void*) new_block->sva);	
				if (shared_var_id >= 0) {
					insert_sorted_allocList(new_block);
					return (void*) new_block->sva;
				} else 
		    		    return NULL;	
		  }

	}

	return NULL;
}