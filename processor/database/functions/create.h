int create_table(const char* name){							//function that calls the script function to make a file name
	int i;										
	char command[1000];
	command[0]='\0';
	strcpy(command,"processor/database/scripts/create_file.sh ");
	strcat(command,name);
	strcat(command," processor/database/data/main");
	i=system(command);
	return i;
}


int check_exist(const char* name){						//checks if a certain table is existing
	int i;	
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/scripts/check_exist.sh ");
	strcat(command,name);
	strcat(command,".dat");
	i=system(command);
	return i;
}

bool table_data_exist(const char* table_name){		//checks if a table has a content already
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/data/main/");
	strcat(command,table_name);	
	strcat(command,".dat");	
	std::cout << command;
	
	std::ifstream fp1;
	fp1.open(command);
	fp1.seekg (0, fp1.end);
    int length = fp1.tellg();
    
	std::cout << length;
	if(length==-1){return false;}
	return true;
	
}
