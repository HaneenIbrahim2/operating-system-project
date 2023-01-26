int createSemaphore(int32 ownerEnvID, char* semaphoreName, uint32 initialValue) {
	
	struct Semaphore * s = NULL;
	int Flag = get_semaphore_object_ID(ownerEnvID, semaphoreName);
	if (Flag == E_SEMAPHORE_NOT_EXISTS) {
		int index = allocate_semaphore_object(&s);
		if (index == E_NO_SEMAPHORE)
			return E_NO_SEMAPHORE;
		s->ownerID = ownerEnvID;
		strcpy(s->name, semaphoreName);
		s->value = initialValue;
		return index;
	}
	return E_SEMAPHORE_EXISTS;
}