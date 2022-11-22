inline void pt_set_page_permissions(uint32* page_directory, uint32 virtual_address,
		uint32 permissions_to_set, uint32 permissions_to_clear)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_set_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_set_page_permissions() is not implemented yet...!!");
	uint32 *ptr_page_table = NULL;
	int ret = get_page_table(page_directory, virtual_address, &ptr_page_table);
    ptr_page_table [PTX(virtual_address)]= ptr_page_table [PTX(virtual_address)]|(permissions_to_set);
    ptr_page_table [PTX(virtual_address)]=ptr_page_table [PTX(virtual_address)]&(~permissions_to_clear);


}