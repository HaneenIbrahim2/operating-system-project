void *krealloc(void *virtual_address, uint32 new_size)
{
	
	struct MemBlock * currentBlock = NULL;
	struct MemBlock * newBlock = NULL;
	uint32 va = (uint32)virtual_address;

	currentBlock = find_block(&AllocMemBlocksList,va);
	if(currentBlock == NULL)
		return kmalloc(new_size);

	uint32 current_size = currentBlock->size;
	int diff_size = new_size - currentBlock->size;
	if(new_size == 0){
		kfree(virtual_address);
		return virtual_address;
	}
	else{
		if(diff_size > 0){
			int res = allocate_chunk(ptr_page_directory,currentBlock->size+currentBlock->sva,diff_size,PERM_WRITEABLE);
			// there is allocated pages within the requested page so will reallocate
			if(res == -1){
				new_size = ROUNDUP(new_size,PAGE_SIZE);
				newBlock = alloc_block_BF(new_size);
			// NO suitable block
			if(newBlock == NULL)
				return NULL;

			cut_paste_pages(ptr_page_directory,va,newBlock->sva,current_size/PAGE_SIZE);
			res = allocate_chunk(ptr_page_directory,ROUNDDOWN(newBlock->sva+current_size,PAGE_SIZE),ROUNDUP(diff_size,PAGE_SIZE),PERM_WRITEABLE);
			LIST_REMOVE(&AllocMemBlocksList,currentBlock);
			insert_sorted_allocList(newBlock);
			insert_sorted_with_merge_freeList(currentBlock);
			return (void *) newBlock->sva;
		}
		// the range is free and allocation is done just need to insert ablock
		else if(res == 0){
			currentBlock->size = new_size;
			return virtual_address;
		}
	}
	else if(diff_size <= 0)
		return virtual_address;
	}
	return NULL;
}