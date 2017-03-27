int create_log(){											//creates the log.txt in the data/tmp folder
	int i;
	char command[1000];
	command[0]='\0';
	strcpy(command,"processor/database/scripts/create_logs.sh ");
	strcat(command,"logs");
	strcat(command," data");
	i=system(command);
	return i;
}

string get_date_and_time(){									//gets the time and date from the system
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
	std::string str(buffer);
	

	return str;
}

void add_to_log(const char* query, const char* msg){			//general function in logging the transactions
	string str =get_date_and_time();
	int i;
	char command[1000];
	command[0]='\0';
	strcpy(command,"processor/database/scripts/logs_create.sh ");
	strcat(command,str.c_str());
	strcat(command,msg);
	//strcat(command," data");
	strcat(command,query);
	i=system(command);
}

void start_logs(){							//signals the start of the program
	create_log();
	string str =get_date_and_time();
	//std::cout << str;
	add_to_log(" ICSDBMS_START"," OK");
}


void end_logs(){							//signals the commits to main
	add_to_log(" ICSDBMS_EXIT_COMMITS_TO_MAIN_DONE"," OK");
	//read_log_file();
}



