void copy_to_tmp(const char* name){						//copy the table to the tmp folder
	char command[1000];	
	command[0]='\0';
	strcat(command,"processor/database/scripts/copy_files.sh ");
	strcat(command,name);
	system(command);	
}


void copy_to_main(const char* name){					//copy the table to the main folder
	char command[1000];	
	command[0]='\0';
	strcat(command,"processor/database/scripts/copy_files_main.sh ");
	strcat(command,name);
	system(command);	
}

void remove_table_main(const char* name){				//removes the table in main folder
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/scripts/remove_tab_main.sh ");
	strcat(command,name);
	strcat(command," 2");
	system(command);	
}

void remove_table(const char* name){					//removes the table in the tmp folder
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/scripts/remove_files.sh ");
	strcat(command,name);
	strcat(command," 1");
	system(command);
	remove_table_main(name);	
}

void remove_all_in_tmp(const char *name){			//removes all the tables in the tmp folder
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/scripts/remove_files.sh ");
	strcat(command,name);
	strcat(command," 1");
	system(command);
	
}

int check_exist_tmp(const char* name){				//checks if a data table consists in the tmp folder
	int i;	
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/scripts/check_exist_tmp.sh ");
	strcat(command,name);
	strcat(command,".dat");
	i=system(command);
	return i;
}
