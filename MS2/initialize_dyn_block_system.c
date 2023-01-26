void initialize_dyn_block_system()
{

	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
	uint32 array_max = 0;
	uint32 heap_size = 0;
#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	/*[2] Dynamically allocate the array of MemBlockNodes
	 * 	remember to:
	 * 		1. set MAX_MEM_BLOCK_CNT with the chosen size of the array
	 * 		2. allocation should be aligned on PAGE boundary
	 * 	HINT: can use alloc_chunk(...) function
	 */
	heap_size = KERNEL_HEAP_MAX - KERNEL_HEAP_START;
	MAX_MEM_BLOCK_CNT = NUM_OF_KHEAP_PAGES;
	MemBlockNodes = (struct MemBlock *)KERNEL_HEAP_START;
	array_max = ROUNDUP(MAX_MEM_BLOCK_CNT*sizeof(struct MemBlock),PAGE_SIZE);
	allocate_chunk(ptr_page_directory, KERNEL_HEAP_START, array_max, PERM_WRITEABLE);

#endif
	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	//[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList

	struct MemBlock * newBlock=LIST_FIRST(&AvailableMemBlocksList);
	newBlock->size=heap_size-array_max;
	newBlock->sva=KERNEL_HEAP_START+array_max;
	LIST_REMOVE(&AvailableMemBlocksList, newBlock);
	LIST_INSERT_HEAD(&FreeMemBlocksList,newBlock);

}