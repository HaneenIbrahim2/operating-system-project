void signalSemaphore(int ownerEnvID, char* semaphoreName) {
	
	int index = get_semaphore_object_ID(ownerEnvID, semaphoreName);
	if (index != E_SEMAPHORE_NOT_EXISTS) {
		semaphores[index].value++;
		if (semaphores[index].value <= 0) {
			struct Env* myenv = dequeue(&semaphores[index].env_queue);
			if (myenv != NULL) {
				sched_insert_ready(myenv);
				myenv->env_status = ENV_READY;
			}
		}
	}
	
}