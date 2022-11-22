uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");
	int count = 0;

	uint32 eva = ROUNDUP(sva + size,PAGE_SIZE);
	uint32 new_size = eva - ROUNDDOWN(sva,PAGE_SIZE);
	uint32 no_frames = new_size / PAGE_SIZE;

	uint32* pageTable_ptr = NULL;
	uint32 tmp = -1;

	for(int i=0;i<no_frames;i++){
		struct FrameInfo * frame_ptr = get_frame_info(page_directory,sva+(i*PAGE_SIZE),&pageTable_ptr);
		// not allocated page
		if(frame_ptr == NULL){
			count++;
			// if the page is not allocated then maybe its table is not allocated too
			// But I first check if I counted it already
			if(pageTable_ptr == NULL && tmp != PDX(sva+(i*PAGE_SIZE))){
				count++;
			}
		}
		tmp=PDX(sva+(i*PAGE_SIZE));
	}
	return count;
}