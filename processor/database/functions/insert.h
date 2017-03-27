using namespace std;
vector<string> get_column_details(const char* table_name, const char* column_name){
	vector<string> values;
	vector<string> details;
	cout << "insert" << endl;
	values = show_table_contents(table_name);
	
	for(int i=0;i<values.size();i++){
		if(strcmp(values[i].c_str(),column_name)==0){
			for(int j=i+1;j<values.size();j++){
				if(strcmp(values[j].c_str(),"|")==0) {i=j; break;}
				else{				
				details.push_back(values[j]);
				}
			}	
		}
	}
	
	for(int i=0;i<details.size();i++){
		cout << details[i] << endl;
	}	
		
	return details;

}

string get_column_type(const char* table_name,const char* column_name){
	vector<string> values;
	vector<string> values1;
	vector<string> values2;
	string type;
	values = show_table_contents(table_name);	
	values1=show_datatypes(values);
	values2=show_table_names(values);
	
	for(int i=0;i<values1.size();i++){
		cout << "" << values1[i] << "," << values2[i];
	}

	for(int i=0;i<values2.size();i++){
		if(strcmp(values2[i].c_str(),column_name)==0){
			return values1[i].c_str();
		}
	}	
	

	
}

vector<string> get_tables_with_same_column_name (vector<string> tables, const char* column_name){
	vector<string> values;
	vector<string> tables_with_column_name;	
	for(int i=0;i<tables.size();i++){
		const char* table_name = tables[i].c_str();
		values=show_table_contents(table_name);
		if(check_column_name(tables[i].c_str(),column_name)){
			tables_with_column_name.push_back(tables[i]);
		}
	}
	return tables_with_column_name;
}

