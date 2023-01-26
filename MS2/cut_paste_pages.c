int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va,
		uint32 num_of_pages) {
	
	uint32 *ptr_page_table1 = NULL;
	uint32 *ptr_page_table2 = NULL;
	struct FrameInfo *ptr_frame_info;

	uint32 sourceva = ROUNDDOWN(source_va, PAGE_SIZE);
	uint32 destva = ROUNDDOWN(dest_va, PAGE_SIZE);

	uint32 size_chunk = num_of_pages * PAGE_SIZE;
	uint32 size = ROUNDUP(size_chunk + dest_va, PAGE_SIZE);

	for (int va = destva; va < size; va += PAGE_SIZE) {
		ptr_frame_info = get_frame_info(page_directory, va, &ptr_page_table2);
		if (ptr_frame_info != NULL)
			return -1;
	}
	for (int va2 = destva; va2 < size; va2 += PAGE_SIZE) {
		int ret = get_page_table(page_directory, va2, &ptr_page_table2);
		
		if (ret == TABLE_NOT_EXIST)
			ptr_page_table2 = create_page_table(page_directory, va2);
		
		ptr_frame_info = get_frame_info(page_directory, sourceva,
				&ptr_page_table1);
		
		uint32 entry = ptr_page_table1[PTX(sourceva)];
		map_frame(page_directory, ptr_frame_info, va2, 4095 & entry);
		unmap_frame(page_directory, sourceva);
		sourceva += PAGE_SIZE;

	}
	return 0;
}
