
void queryprocessing(std::string line){
	std::vector<std::string> words;	
	std::vector<std::string> values;
	std::vector<std::string> status;
	std::vector<std::string> d_types;
	std::vector<std::string> col_names;
	bool constraints;	
	std::string dir = string("processor/database/data/main/.");						//path of the main data files
    std::vector<std::string> files;										//vector that contains the table names of the database
	files=listFiles("processor/database/data/main/",files);							//function that inserts the table names of the the database

	std::stringstream buffer(line);

	std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
	
	const char* buf = words[0].c_str();
	if(strcmp(buf,"quit")==0){
	compare_data_bank();
	showexit();
	end_logs();
	exit(1);
	}	
		
	if(strcmp(buf,"create")==0){
	const char* table_name = words[1].c_str();
	const char* no_tokens = words[2].c_str();
	int num_tokens = atoi(no_tokens);
	if(!check_exist(table_name)){	
			create_table(table_name);
			table_data(table_name,num_tokens,words,1);	
			success_msg(100);
			}
		else{error_msg(200);}
	words.clear();	
	}//end of create

	else if(strcmp(buf, "drop")==0){
		const char* type = words[1].c_str();
		const char* table_name = words[2].c_str();
		if(strcmp(type,"table")==0){
			if(check_exist(table_name)!=0){
				if(check_exist_tmp(table_name)!=0){
					remove_table(table_name);
					success_msg(105);
					}
					else{
					copy_to_tmp(table_name);
					remove_table(table_name);
					success_msg(105);
					}
				status.push_back(table_name);
			}
			//else{error_msg(206);}
			
		}else if(strcmp(type,"all") == 0){
			if(files.size() != 0){
				compare_data_bank();
				system("processor/database/scripts/remove_all_tables.sh");
			}
		}	
		words.clear();
	}//end of drop

	else if(strcmp(buf,"desc")==0){
		const char* table_name = words[1].c_str();
		
		if(check_exist(table_name)!=0){
			if(check_exist_tmp(table_name)!=0){
			values=show_table_contents(table_name);
			d_types=show_datatypes(values);
			print_table_properties2(values);
			//cout << "\n";
			//print_table_properties(values);
			//constraints=is_primary_key("pep","id");
			//cout << "primary: "<< constraints << endl;			
			//constraints=is_unique_key("a","id");
			//cout << "unique: "<< constraints << endl;	
			//constraints=is_not_null("a","id");
			//cout << "not null: " << constraints<< endl;				
			}
			else{
			copy_to_tmp(table_name);
			values=show_table_contents(table_name);
			d_types=show_datatypes(values);
			print_table_properties2(values);
			//cout << "\n";
			//print_table_properties(values);
			//constraints=is_primary_key("pep","id");
			//cout << "primary: "<< constraints << endl;			
			//constraints=is_unique_key("a","id");
			//cout << "unique: "<< constraints << endl;	
			//constraints=is_not_null("a","id");
			//cout << "not null: " << constraints<< endl;				
			}
		}
		else {error_msg(207);}
			
	
	words.clear();	
	}//end of desc
	else if(strcmp(buf,"alter")==0){
		//rename,add,delete
		const char* table_name = words[1].c_str();		
		const char* operation = words[2].c_str();	
		
		if(strcmp(operation,"add")==0){		
		if(check_exist(table_name)!=0){
			if(check_exist_tmp(table_name)!=0){
				values = show_table_contents(table_name);				
				values=change_values(words,values);
				int total=atoi(values[0].c_str());				
				table_data(table_name,total,values,2);	
			}
		else{
		copy_to_tmp(table_name);
		values = show_table_contents(table_name);		
		values=change_values(words,values);
		int total=atoi(values[0].c_str());				
		table_data(table_name,total,values,2);	
		}
		}
		
	}//end of add

		else if(strcmp(operation,"rename")==0){
			const char * table_name = words[1].c_str();
			const char * old_name = words[3].c_str();
			const char * new_name = words[4].c_str();
			if(check_exist(table_name)!=0){
				if(check_exist_tmp(table_name)!=0){
				values = show_table_contents(table_name);				
				values = rename_columnname(values,old_name,new_name);	
				int total=atoi(values[0].c_str());		
				table_data(table_name,total,values,2);
				}
				else{
				copy_to_tmp(table_name);		
				values = show_table_contents(table_name);				
				values = rename_columnname(values,old_name,new_name);	
				int total=atoi(values[0].c_str());		
				table_data(table_name,total,values,2);
				}
			}
		}//end of rename
		//alter tablename drop column_name
		else if(strcmp(operation,"drop")==0){
			const char* colname = words[3].c_str();
			const char* table_name = words[1].c_str();
			if(check_exist(table_name)!=0){
				if(check_exist_tmp(table_name)!=0){
				values = show_table_contents(table_name);
				values = drop_columnname(values,colname);
				int total=atoi(values[0].c_str());		
				table_data(table_name,total,values,2);
				}			
				else{
				copy_to_tmp(table_name);
				values = show_table_contents(table_name);
				values = drop_columnname(values,colname);				
				int total=atoi(values[0].c_str());		
				table_data(table_name,total,values,2);				
				}

			}	

		}
	
	words.clear();
	}//end of alter

	if(strcmp(words[0].c_str(),"showdbs")==0){
		std::vector<string> files;	
		files=listFiles("processor/database/data/main/",files);
		print_table_names(files);
		bool stats;
		//stats=table_data_exist("adhvbnk");
		//cout <<"stats " << stats << endl;
		words.clear();
	}
	//else error_msg(208);	
	
	words.clear();	//clears the buffer
 	

}

 
