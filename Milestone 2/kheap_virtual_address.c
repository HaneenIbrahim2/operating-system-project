unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//return the virtual address corresponding to given physical_address
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED
	int f_num = physical_address >> 12;
	if(ph_arr[f_num] == -1)
		return 0;
	return ph_arr[f_num];
	
}