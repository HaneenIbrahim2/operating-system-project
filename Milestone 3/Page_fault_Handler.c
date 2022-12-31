void page_fault_handler(struct Env * curenv, uint32 fault_va) {
	
	uint32 numofpages = 0;
	struct FrameInfo * ptr_frame_info = NULL;
	uint32 *ptr_page_table = NULL;
	numofpages = env_page_ws_get_size(curenv);
	uint32 lastIndex = curenv->page_last_WS_index;

	if (numofpages == curenv->page_WS_max_size) {
		while (lastIndex < curenv->page_WS_max_size) {
			uint32 va = curenv->ptr_pageWorkingSet[lastIndex].virtual_address;
			uint32 perms = pt_get_page_permissions(curenv->env_page_directory,
					va);
			if ((perms & PERM_USED) == 0) {
				if ((perms & PERM_MODIFIED) > 0) {
					ptr_frame_info = get_frame_info(curenv->env_page_directory,
							va, &ptr_page_table);
					pf_update_env_page(curenv, va, ptr_frame_info);
				}
				unmap_frame(curenv->env_page_directory, va);
				env_page_ws_invalidate(curenv, va);
				break;
			} else {
				pt_set_page_permissions(curenv->env_page_directory, va, 0,
						PERM_USED);
				lastIndex = (lastIndex + 1) % curenv->page_WS_max_size;
			}
		}
	}
	
	numofpages = env_page_ws_get_size(curenv);
	if (numofpages < curenv->page_WS_max_size) {
		ptr_frame_info = get_frame_info(curenv->env_page_directory, fault_va,
				&ptr_page_table);
		allocate_frame(&ptr_frame_info);
		map_frame(curenv->env_page_directory, ptr_frame_info, fault_va,
				PERM_WRITEABLE | PERM_USER);

		int ret = pf_read_env_page(curenv, (void*) fault_va);
		if (ret == E_PAGE_NOT_EXIST_IN_PF) {
			if (!((fault_va >= USER_HEAP_START && fault_va <= USER_HEAP_MAX)
					|| (fault_va >= USTACKBOTTOM && fault_va <= USTACKTOP))) {
				unmap_frame(curenv->env_page_directory, fault_va);
				panic("ILLEGAL MEMORY ACCESS");
			}
		}
		
		int i = 0;
		int res = 0;
		for (; i < curenv->page_WS_max_size; i++) {
			res = env_page_ws_is_entry_empty(curenv, i);
			if (res == 1)
				break;
		}
		env_page_ws_set_entry(curenv, i, ROUNDDOWN(fault_va, PAGE_SIZE));
		curenv->page_last_WS_index = (i + 1) % curenv->page_WS_max_size;
	}

	//refer to the project presentation and documentation for details
}
void __page_fault_handler_with_buffering(struct Env * curenv, uint32 fault_va) {
	// Write your code here, remove the panic and write your code
	panic("__page_fault_handler_with_buffering() is not implemented yet...!!");

}
