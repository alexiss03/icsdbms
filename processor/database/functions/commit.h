using namespace std;
bool check_in_main(const char* tmp_file_name){					//checks if a certain table has been altered in the transaction
	vector<string> main_files;	
	main_files=listFiles("processor/database/data/main/",main_files);

	for(int i=0; i<main_files.size();i++){
		if(strcmp(tmp_file_name,main_files[i].c_str())==0){
		return true;
		}
	}
	return false;
}

void move_to_main(vector<string> match_data){				//moves the files in the main
	//copy_to_tmp(const char* name)
	//remove_table(const char* name)

	for(int i=0;i<match_data.size();i++){
		remove_table_main(match_data[i].c_str());
		copy_to_main(match_data[i].c_str());
		remove_all_in_tmp(match_data[i].c_str());
	}	

}

bool check_main_files_exist(){
	vector<string> main_files;
	main_files=listFiles("processor/database/data/main/",main_files);
	
	if(main_files.size() > 0)
		return true;
	else 
		return false;

}

void compare_data_bank(){								//compares the data from the tmp and to the main
	vector<string> match_data;
	vector<string> main_files;
	vector<string> tmp_files;
	tmp_files=listFiles("processor/database/data/tmp/",tmp_files);
	main_files=listFiles("processor/database/data/main/",main_files);	
	
	
	for(int i=0;i<tmp_files.size();i++){
	if(check_in_main(tmp_files[i].c_str())){
		match_data.push_back(tmp_files[i].c_str());	
		}	
	}
	

	move_to_main(match_data);
	end_logs();
}//end of compare_data_bank


