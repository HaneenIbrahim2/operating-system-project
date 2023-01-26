void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");
	LIST_INIT(&AvailableMemBlocksList);
	for(int i=0;i<numOfBlocks;i++){
		LIST_INSERT_TAIL(&AvailableMemBlocksList,&MemBlockNodes[i]);
	}
}