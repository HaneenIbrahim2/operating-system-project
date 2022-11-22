inline int pt_get_page_permissions(uint32* page_directory, uint32 virtual_address )
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_get_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_get_page_permissions() is not implemented yet...!!");
	uint32* pageTable_ptr=NULL;
	int flag = get_page_table(page_directory,virtual_address,&pageTable_ptr);
	if(flag == TABLE_IN_MEMORY){
		// 4095 is equivalent to 00000000000000000000111111111111
		return pageTable_ptr[PTX(virtual_address)] & (4095);
	}
	return -1;
}