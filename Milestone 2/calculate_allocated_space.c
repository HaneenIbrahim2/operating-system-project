void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	//panic("calculate_allocated_space() is not implemented yet...!!");
	int tcount = 0;
	int pcount = 0;

	uint32 new_size = ROUNDUP(eva,PAGE_SIZE) - ROUNDDOWN(sva,PAGE_SIZE);
	uint32 no_frames = new_size / PAGE_SIZE;

	uint32* pageTable_ptr = NULL;
	uint32* tmp = NULL;
	for(int i=0;i<no_frames;i++){
		struct FrameInfo * frame_ptr = get_frame_info(page_directory,sva+(i*PAGE_SIZE),&pageTable_ptr);
		if(frame_ptr != NULL)
			pcount++;
		if(tmp != pageTable_ptr && pageTable_ptr != NULL )
			tcount++;

		tmp = pageTable_ptr;
	}

	*num_tables = tcount;
	*num_pages = pcount;
}