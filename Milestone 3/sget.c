void* sget(int32 ownerEnvID, char *sharedVarName) {
	InitializeUHeap();
	uint32 size = sys_getSizeOfSharedObject(ownerEnvID,sharedVarName);
	if(size == E_SHARED_MEM_NOT_EXISTS)
		return NULL;
	
	size = ROUNDUP(size,PAGE_SIZE);
	if(sys_isUHeapPlacementStrategyFIRSTFIT()){
		struct MemBlock * newBlock = alloc_block_FF(size);
		if(newBlock != NULL){
			uint32 res = sys_getSharedObject(ownerEnvID,sharedVarName,(void*)newBlock->sva);
			if(res < 0){
				return NULL;
			}
			return (void*) newBlock->sva;

		}
		return NULL;
	}
	return NULL;
	
}