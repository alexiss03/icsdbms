using namespace std;
std::vector<std::string> change_values(vector<string>& words, vector<string>& values){
		const char* num_f = words[3].c_str();
		int num = atoi(num_f);
		const char *val_i = values[0].c_str();
		int val = atoi(val_i);
		int total = val+num;
		stringstream ss;
		ss << total;
		string str = ss.str();
		values[0]=str;
		for(int i=4;i<(num+4);i++){
			values.push_back(words[i].c_str());
		}
		return values;
}

std::vector<std::string> rename_columnname(vector<string>& values, const char* old_name, const char* new_name){

	for(int i=0;i<values.size();i++){
		if(strcmp(values[i].c_str(),old_name)==0){
			stringstream ss;		
			ss << new_name;					
			string str = ss.str();			
			values[i]=str;
			return values;				
			
		}
	}
}

std::vector<std::string> drop_columnname(vector<string>& values, const char *colname){
	
	std::vector<std::string> new_values;
	int counter=0;
	int iterator=0;
	int j;
	/**for(int i=0;i<values.size();i++){
		if(strcmp(values[i].c_str(),colname)==0){
			//printf("i=%d\n",i);
			counter++;
		 for(j=i; j<values.size();j++){
				
				//supported data types
				if((strcmp(values[j].c_str(),"int")==0 )|| (strcmp(values[j].c_str(),"float")==0 ) || (strcmp(values[j].c_str(),"char")==0) || (strcmp(values[j].c_str(),"date")==0) || (strcmp(values[j].c_str(),"boolean")==0 )){
				counter++;
				}
				//varchar special case
				if((strcmp(values[j].c_str(),"varchar")==0 )){
				counter=counter+2;
				j++;
				}
			
				//constraints
				if((strcmp(values[j].c_str(),"_pk")==0 )||(strcmp(values[j].c_str(),"_uk")==0) || (strcmp(values[j].c_str(),"_nn")==0 )){
				counter++;
				}
				else {
				i=i+counter; 
				break;
				}
			}//end of for		
		}
		else{
		stringstream ss;
		ss << values[i].c_str();					
		string str = ss.str();
		new_values.push_back(str);
		}
		
	}	
	const char* val=values[0].c_str();
	int num = atoi(val);
	int total = num-counter;
	
	stringstream ss;
	ss << total;
	string str = ss.str();
	new_values[0] = str; 

	for(int i=0;i<new_values.size();i++){
	std::cout << new_values[i] << endl;
	}**/
	//cout << "DROP" << endl;
	int total = atoi(values[0].c_str());
	new_values.push_back(values[0]);		
	for(int i=1;i<values.size();i++){
		if(strcmp(values[i].c_str(),colname)==0){
			for(int j=i;j<values.size();j++){
				if(strcmp(values[j].c_str(),"|")==0) { i=j; break;}
				else {
					//cout << "val of j " << values[j] << endl; 
					counter++;
				}
			}
		}
		else{
		new_values.push_back(values[i]);		
		}
	}
	total = total-counter;
	new_values[0] = total;	
	/*for(int i=0;i<new_values.size();i++){
		cout << new_values[i] << endl;
	}	*/
	return new_values;
}
