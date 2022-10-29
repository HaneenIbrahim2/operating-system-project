int execute_command(char *command_string)
{
	// Split the command string into whitespace-separated arguments
	int number_of_arguments;
	//allocate array of char * of size MAX_ARGUMENTS = 16 found in string.h
	char *arguments[MAX_ARGUMENTS];
	// array of indices 
	int similar[NUM_OF_COMMANDS];

	strsplit(command_string, WHITESPACE, arguments, &number_of_arguments) ;
	if (number_of_arguments == 0)
		return 0;

	// Lookup in the commands array and execute the command
	int command_found = 0;
	int i ;
	int j=0;
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (strcmp(arguments[0], commands[i].name) == 0)
		{
			command_found = 1;
			break;
		} // compare entered command with the command list item
		else if(strncmp(arguments[0], commands[i].name,strlen(arguments[0]))==0)
		{
			similar[j]=i;
			j++;
		}
	}

	if(command_found)
	{
		int return_value;
		return_value = commands[i].function_to_execute(number_of_arguments, arguments);
		return return_value;
	}
	//check for similar commands
	else if(j>0){
		for(int i=0;i<j;i++){
			cprintf("%s\n",commands[similar[i]].name);
		}
		return 0;
	}
	else
	{
		//if not found, then it's unknown command
		cprintf("Unknown command '%s'\n", arguments[0]);
		return 0;
	}
}