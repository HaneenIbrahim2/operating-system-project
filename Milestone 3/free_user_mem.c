void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size) {
	
	
	for (int i = virtual_address; i < virtual_address + size; i += PAGE_SIZE) {
		pf_remove_env_page(e, i);
		env_page_ws_invalidate(e, i);
		unmap_frame(e->env_page_directory, i);
	}
	uint32 * table_ptr = NULL;
	for (int i = virtual_address; i < virtual_address + size;
			i += PAGE_SIZE * 1024) {
		int ret = get_page_table(e->env_page_directory, i, &table_ptr);
		if (ret == TABLE_IN_MEMORY) {
			for (int j = 0; j < 1024; j++) {
				uint32 table_entry = table_ptr[j];
				if ((table_entry & PERM_PRESENT) > 0) {
					break;
				}
				if (j == 1023) {
					kfree((void *) table_ptr);
					uint32 dirIndex = PDX(i);
					e->env_page_directory[dirIndex] = 0;
				}
			}
		}
	}
}
