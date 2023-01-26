int getSharedObject(int32 ownerID, char* shareName, void* virtual_address) {
	struct Env* myenv = curenv; //The calling environment

	struct FrameInfo* ptr_frame_info = NULL;
	uint32 va = ROUNDDOWN((uint32)virtual_address, PAGE_SIZE);
	int sharedObjectIndex = get_share_object_ID(ownerID, shareName);

	if (sharedObjectIndex != E_SHARED_MEM_NOT_EXISTS) {
		int no_frames = ROUNDUP(shares[sharedObjectIndex].size,PAGE_SIZE)/PAGE_SIZE;
		
		for (int i = 0; i < no_frames; i++) {
			ptr_frame_info = get_frame_from_storage(shares[sharedObjectIndex].framesStorage, i);
			if (shares[sharedObjectIndex].isWritable == 0)
				map_frame(myenv->env_page_directory, ptr_frame_info, va,PERM_USER);
			else
				map_frame(myenv->env_page_directory, ptr_frame_info, va,(PERM_WRITEABLE| PERM_USER));
			va = va + PAGE_SIZE;
		}
		
		shares[sharedObjectIndex].references++;
		return sharedObjectIndex;

	} else
		return E_SHARED_MEM_NOT_EXISTS;

}