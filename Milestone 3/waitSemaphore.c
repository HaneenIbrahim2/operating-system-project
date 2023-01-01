void waitSemaphore(int32 ownerEnvID, char* semaphoreName) {
	
	struct Env* myenv = curenv; //The calling environment
	int index = get_semaphore_object_ID(ownerEnvID, semaphoreName);
	if (index != E_SEMAPHORE_NOT_EXISTS) {
		semaphores[index].value--;
		if (semaphores[index].value < 0) {
			enqueue(&semaphores[index].env_queue, myenv);
			myenv->env_status = ENV_BLOCKED;
			curenv = NULL;
		}
		fos_scheduler();
	}
	
}