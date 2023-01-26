unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//return the physical address corresponding to given virtual_address
	virtual_address = ROUNDDOWN(virtual_address,PAGE_SIZE);
	uint32* pageTable_ptr = NULL;
	struct FrameInfo * frame_ptr = get_frame_info(ptr_page_directory,virtual_address,&pageTable_ptr);
	if(frame_ptr != NULL)
		return to_physical_address(frame_ptr);
	else
			return 0;

}