void kfree(void* virtual_address)
{
	
	struct MemBlock * newBlock = NULL;
	struct FrameInfo * frame_ptr = NULL;
	uint32* pageTable_ptr = NULL;
	
	if((uint32)virtual_address >= KERNEL_HEAP_START &&(uint32)virtual_address < KERNEL_HEAP_MAX ){
		newBlock = find_block(&AllocMemBlocksList,(uint32)virtual_address);
		if(newBlock != NULL){
			for(uint32 i = ROUNDDOWN(newBlock->sva,PAGE_SIZE) ; i<ROUNDUP(newBlock->sva+newBlock->size,PAGE_SIZE) ;i+=PAGE_SIZE){
				frame_ptr = get_frame_info(ptr_page_directory,i,&pageTable_ptr);
				ph_arr[to_frame_number(frame_ptr)] = -1;
				unmap_frame(ptr_page_directory,i);
			}
			LIST_REMOVE(&AllocMemBlocksList,newBlock);
			insert_sorted_with_merge_freeList(newBlock);


		}
	}
}
