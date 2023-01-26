void* malloc(uint32 size) {
	
	InitializeUHeap();
	if (size == 0)
		return NULL;
	
	struct MemBlock *memblock = NULL;
	size = ROUNDUP(size, PAGE_SIZE);
	//Implement FF strategy to search the heap for suitable space
	if (sys_isUHeapPlacementStrategyFIRSTFIT()) {
		memblock = alloc_block_FF(size);
	} else {
		return NULL;
	}

	if (memblock == NULL) {
		return NULL;
	} else {
		//Return pointer containing the virtual address of allocated space
		insert_sorted_allocList(memblock);
		return (void*) memblock->sva;
	}
}
