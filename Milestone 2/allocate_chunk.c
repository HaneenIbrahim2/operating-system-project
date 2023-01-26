int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms) {
	
	uint32 eva = ROUNDUP(va + size, PAGE_SIZE);
	uint32 new_size = eva - ROUNDDOWN(va, PAGE_SIZE);
	uint32 no_frames = new_size / PAGE_SIZE;

	uint32* pageTable_ptr = NULL;
	struct FrameInfo * frame_ptr = NULL;

	// check if the whole chunk is free
	// if not return -1;
	for (int i = 0; i < no_frames; i++) {
		frame_ptr = get_frame_info(page_directory, va + (i * PAGE_SIZE),
				&pageTable_ptr);
		if (frame_ptr != NULL)
			return -1;
	}
	// allocate the chunk and create necessary non existing tables
	for (int i = 0; i < no_frames; i++) {
		frame_ptr = get_frame_info(page_directory, va + (i * PAGE_SIZE),
				&pageTable_ptr);
		if (pageTable_ptr == NULL)
			create_page_table(page_directory, va + (i * PAGE_SIZE));
		allocate_frame(&frame_ptr);
		map_frame(page_directory, frame_ptr, va + (i * PAGE_SIZE), perms);
	}

	return 0;
}
