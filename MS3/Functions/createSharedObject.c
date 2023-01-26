int createSharedObject(int32 ownerID, char* shareName, uint32 size,
		uint8 isWritable, void* virtual_address) {
	
	struct Env* myenv = curenv; //The calling environment

	struct Share *shares = NULL;
	struct FrameInfo* ptr_frame_info = NULL;

	uint32 va = ROUNDDOWN((uint32 )virtual_address, PAGE_SIZE);
	size = ROUNDUP(size, PAGE_SIZE);

	uint32 no_frames = size / PAGE_SIZE;

	int ret = get_share_object_ID(ownerID, shareName);
	if (ret != E_SHARED_MEM_NOT_EXISTS) {
		return E_SHARED_MEM_EXISTS;
	}

	int shared_obj_id = allocate_share_object(&shares);

	if (shared_obj_id != E_NO_SHARE) {

		strcpy(shares->name, shareName);
		shares->isWritable = isWritable;
		shares->size = size;
		shares->ownerID = ownerID;
		shares->references = 1;
		for (int i = 0; i < no_frames; i++) {

			ptr_frame_info = NULL;
			allocate_frame(&ptr_frame_info);
			map_frame(myenv->env_page_directory, ptr_frame_info, va,
			PERM_WRITEABLE | PERM_USER);
			add_frame_to_storage(shares->framesStorage, ptr_frame_info, i);
			va = va + PAGE_SIZE;
		}
		return shared_obj_id;
	} else {
		return E_NO_SHARE;
	}

	return 0;

}