inline void pt_set_page_permissions(uint32* page_directory, uint32 virtual_address,
		uint32 permissions_to_set, uint32 permissions_to_clear)
{
	
	uint32 *ptr_page_table = NULL;
	int ret = get_page_table(page_directory, virtual_address, &ptr_page_table);
	if(ret==TABLE_NOT_EXIST){
		panic("invalid va");
	}
    ptr_page_table [PTX(virtual_address)]= ptr_page_table [PTX(virtual_address)]|(permissions_to_set);
    ptr_page_table [PTX(virtual_address)]=ptr_page_table [PTX(virtual_address)]&(~permissions_to_clear);


}