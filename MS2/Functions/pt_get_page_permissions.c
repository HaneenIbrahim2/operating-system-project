inline int pt_get_page_permissions(uint32* page_directory, uint32 virtual_address )
{
	
	uint32* pageTable_ptr=NULL;
	int flag = get_page_table(page_directory,virtual_address,&pageTable_ptr);
	
	if(flag == TABLE_IN_MEMORY){
		return pageTable_ptr[PTX(virtual_address)] & (4095);
	}
	return -1;
}