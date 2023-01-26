int ph_arr[NUM_OF_KHEAP_PAGES];
void* kmalloc(unsigned int size)
{
	

	
	struct FrameInfo * frame_ptr = NULL;
	struct MemBlock * newBlock = NULL;
	uint32* pageTable_ptr = NULL;
	int perm = PERM_WRITEABLE;
	size = ROUNDUP(size,PAGE_SIZE);
	
	if(isKHeapPlacementStrategyFIRSTFIT())
		newBlock = alloc_block_FF(size);
	else if (isKHeapPlacementStrategyBESTFIT())
		newBlock = alloc_block_BF(size);
	else if(isKHeapPlacementStrategyNEXTFIT())
		newBlock = alloc_block_NF(size);
	else
		return NULL;

	if(newBlock == NULL)
		return NULL;
	
	
	int ret = allocate_chunk(ptr_page_directory,newBlock->sva,size,perm);
	if(ret == 0){
		for(int i = newBlock->sva; i <newBlock->sva+newBlock->size ; i+=PAGE_SIZE ){
		frame_ptr = get_frame_info(ptr_page_directory,i,&pageTable_ptr);
		ph_arr[to_frame_number(frame_ptr)] = i;
		}
		insert_sorted_allocList(newBlock);
		return (void*)newBlock->sva;
	}
	else
		return NULL;
}