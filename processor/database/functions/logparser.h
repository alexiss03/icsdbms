vector<string> read_log_file(){					//returns a vector of the logs read from the text file
	vector<string> log_data;
	char filename[1000];	
	filename[0]='\0';
	string line;
	strcat(filename,"processor/database/data/");	
	strcat(filename,"logs.txt");
	
	
  ifstream myfile (filename);
  if (myfile.is_open()){
    while ( getline (myfile,line) )
    {
      
	  stringstream s (line);
	  while(s>> line)
	  log_data.push_back(line);
    }
    myfile.close();
  }

  return log_data;
	
}

bool check_log_file(){						//checks the last commit
	vector<string> data;
	data=read_log_file();

	if(strcmp(data[(data.size()-1)].c_str(),"ICSDBMS_EXIT_COMMITS_TO_MAIN_DONE")==0){
	return false;
	}
	return true;
}


vector<string> get_all_unsuccessful_queries(){		//gets all the unsuccessful transactions
	vector<string> data;	
	data=read_log_file();
	
}
