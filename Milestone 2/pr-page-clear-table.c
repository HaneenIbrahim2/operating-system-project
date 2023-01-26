inline void pt_clear_page_table_entry(uint32* page_directory, uint32 virtual_address)
{
	

	uint32* ptr_page_table=NULL;

	int ret=get_page_table(page_directory, virtual_address, &ptr_page_table);
	if(ret==TABLE_IN_MEMORY){
		unmap_frame(page_directory,virtual_address);
		tlb_invalidate((void *)NULL, (void *)virtual_address);
	}
	else{
		panic("invalid va");
	}
}