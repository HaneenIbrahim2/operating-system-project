void initialize_dyn_block_system() {
	uint32 array_max = 0;
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
	
	// Dynamically allocate the array of MemBlockNodes at VA USER_DYN_BLKS_ARRAY
	MAX_MEM_BLOCK_CNT = NUM_OF_UHEAP_PAGES;
	MemBlockNodes = (struct MemBlock *) USER_DYN_BLKS_ARRAY;
	array_max = ROUNDUP(MAX_MEM_BLOCK_CNT * sizeof(struct MemBlock), PAGE_SIZE);
	sys_allocate_chunk(USER_DYN_BLKS_ARRAY, array_max,
			PERM_WRITEABLE | PERM_USER);
	
	
	// Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	
	//Insert a new MemBlock with the heap size into the FreeMemBlocksList
	struct MemBlock * newBlock = LIST_FIRST(&AvailableMemBlocksList);
	newBlock->size = ROUNDUP(USER_HEAP_MAX-USER_HEAP_START, PAGE_SIZE);
	newBlock->sva = USER_HEAP_START;
	LIST_REMOVE(&AvailableMemBlocksList, newBlock);
	LIST_INSERT_HEAD(&FreeMemBlocksList, newBlock);
}