int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	//panic("copy_paste_chunk() is not implemented yet...!!");
		uint32 old = source_va+size;
		uint32 eva = ROUNDUP(dest_va+size, PAGE_SIZE);
		uint32 new_size = eva - ROUNDDOWN(dest_va, PAGE_SIZE);
		uint32 no_frames = new_size / PAGE_SIZE;

		struct FrameInfo* dst_frame_ptr = NULL;
		struct FrameInfo* src_frame_ptr = NULL;
		uint32* dst_page_table = NULL;
		uint32* src_page_table = NULL;
		int ret;
		uint32 entry_dst;
		uint32 entry_src;
		for(int i=0; i<no_frames; i++){
			ret = get_page_table(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
			if(ret == TABLE_IN_MEMORY){
				entry_dst = dst_page_table[PTX(dest_va+(i*PAGE_SIZE))];
				if((entry_dst & PERM_PRESENT) > 0  && (entry_dst & PERM_WRITEABLE) <=0 )
					return -1;
			}
		}

		for(int i=0; i<no_frames; i++){
			ret = get_page_table(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
			if(ret == TABLE_NOT_EXIST)
				dst_page_table = create_page_table(page_directory, dest_va+(i*PAGE_SIZE));
			dst_frame_ptr = get_frame_info(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
			src_frame_ptr = get_frame_info(page_directory, source_va+(i*PAGE_SIZE), &src_page_table);
			entry_src = src_page_table[PTX(source_va+(i*PAGE_SIZE))];
			if(dst_frame_ptr == NULL){
				allocate_frame(&dst_frame_ptr);
				map_frame(page_directory, dst_frame_ptr, dest_va+(i*PAGE_SIZE), PERM_WRITEABLE | (PERM_USER & entry_src));
			}
			// loop to copy each byte
			int z = 0;
			for(z ; z<PAGE_SIZE; z++){
				if(source_va+(i*PAGE_SIZE) >= old)
					break;
				char *k = (char*)source_va+(i*PAGE_SIZE);
				char *j = (char*)dest_va+(i*PAGE_SIZE);
				*j = *k;
				source_va = source_va+1;
				dest_va = dest_va+1;
			}
			source_va = source_va-z;
			dest_va = dest_va-z;
		}
		return 0;
}