int share_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va,
		uint32 size, uint32 perms) {
	
	uint32 Source = ROUNDUP(source_va + size, PAGE_SIZE);
	uint32 new_size1 = Source - ROUNDDOWN(source_va, PAGE_SIZE);

	uint32 dest = ROUNDUP(dest_va + size, PAGE_SIZE);
	uint32 new_size2 = dest - ROUNDDOWN(dest_va, PAGE_SIZE);
	uint32 no_frames = new_size2 / PAGE_SIZE;

	uint32 *ptr_page_table_source = NULL;
	uint32 *ptr_page_table_dest = NULL;

	struct FrameInfo* ptr_Frame_Info = NULL;

	uint32 new_add_des = dest;
	uint32 new_add_source = Source;

	get_page_table(page_directory, source_va, &ptr_page_table_source);
	get_page_table(page_directory, dest_va, &ptr_page_table_dest);

	if (ptr_page_table_dest == NULL) {
		ptr_page_table_dest = create_page_table(page_directory, dest_va);
	}
	for (int i = 0; i < no_frames; i++) {
		ptr_Frame_Info = get_frame_info(page_directory, new_add_des - PAGE_SIZE,
				&ptr_page_table_dest);
		if (ptr_Frame_Info == NULL) {

			new_add_des = new_add_des - PAGE_SIZE;

		} else
			return -1;
	}
	new_add_source = Source - PAGE_SIZE;
	new_add_des = dest - PAGE_SIZE;

	for (int i = 0; i < no_frames; i++) {
		ptr_Frame_Info = get_frame_info(page_directory, new_add_source,
				&ptr_page_table_source);
		map_frame(page_directory, ptr_Frame_Info, new_add_des, perms);
		new_add_des = new_add_des - PAGE_SIZE;
		new_add_source = new_add_source - PAGE_SIZE;
	}
	return 0;
}