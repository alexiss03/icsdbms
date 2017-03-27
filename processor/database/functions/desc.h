#include <iomanip>
using namespace std;
std::vector<std::string> show_datatypes(vector<string> values){					//returns a vector of the datatypes of the table	
	std::vector<std::string> datatypes;
	
	int no_of_tokens = atoi(values[0].c_str());
	for(int i=0;i<values.size();i++){
		if((strcmp(values[i].c_str(),"int")==0) || (strcmp(values[i].c_str(),"float")==0) || (strcmp(values[i].c_str(),"char")==0)|| (strcmp(values[i].c_str(),"varchar")==0) || (strcmp(values[i].c_str(),"boolean")==0)|| (strcmp(values[i].c_str(),"date")==0)){
		datatypes.push_back(values[i]);
		
		}
		
	}

	return datatypes;
	
}//end of show_datatypes

std::vector<string> show_table_names(vector<string> values){				//returns all the table names in the data			
	std::vector<string> col_names;

		for(int i=1;i<values.size();i++){
		if((strcmp(values[i].c_str(),"int")==0) || (strcmp(values[i].c_str(),"float")==0) || (strcmp(values[i].c_str(),"char")==0)|| (strcmp(values[i].c_str(),"boolean")==0)|| (strcmp(values[i].c_str(),"date")==0)){
		
		
		}
		else if((strcmp(values[i].c_str(),"_pk")==0) || (strcmp(values[i].c_str(),"_uk")==0) || (strcmp(values[i].c_str(),"_nn")==0)){
		if(strcmp(values[i].c_str(),"_pk")==0){		
		
			if((i+1) < values.size()){
			if(strcmp(values[i+1].c_str(),"_uk")==0 || strcmp(values[i+1].c_str(),"_nn")==0 ){		
			}
			else {i++;}
			}		
		}//end of else of _pk	
	
		else if(strcmp(values[i].c_str(),"_uk")==0){
		
		if((i+1) < values.size()){
		
		if(strcmp(values[i+1].c_str(),"_nn")==0 || strcmp(values[i+1].c_str(),"_pk")==0 ){		
			}
		else {i++;}
		}
		
		}//end of else of _uk

		else if(strcmp(values[i].c_str(),"_nn")==0){
				
		}//end of else of _nn		
		}//end of consraints
		//varchar special case
		else if(strcmp(values[i].c_str(),"varchar")==0){
			i++;
		}
		else if(strcmp(values[i].c_str(),"|")==0){
		
		}
		else{
			col_names.push_back(values[i]);
		
		}
	}

	return col_names;
}//end of show_table_names

std::vector<std::string> show_table_contents(const char* table_name){			//functions that reads the contents of the .stdat file 
	char filename[1000];														// of the specific table
	int c=0;																	//only reads at the data/tmp
	filename[0]='\0';
	
	if(check_exist_tmp(table_name)==0){
		copy_to_tmp(table_name);	
	}																	

	strcat(filename,"processor/database/data/tmp/");	
	strcat(filename,table_name);
	strcat(filename,".stdat");
	vector<string> values;
	vector<string> new_values;
	
	ifstream fp1;
	fp1.open(filename,ios::binary);
	fp1.seekg (0, fp1.end);
    int length = fp1.tellg();
    fp1.seekg (0, fp1.beg);
	int i,j;
	int num=1000;
	char buffer[length];
	char buffer2[1000];
	
	length = (length/8)+1;
	buffer[8] = '\0';
	while(fp1.read((char*)buffer,sizeof(string)*2)){
			//cout << buffer << " ";
			values.push_back(buffer);
			buffer[8]='\0';		
          }
          fp1.close();	

	new_values.push_back(values[0]);
	
	for(i=1;i<values.size();i++){
		//datatypes
		buffer2[0] = '\0';
		if((strcmp(values[i].c_str(),"int")==0) || (strcmp(values[i].c_str(),"float")==0) || (strcmp(values[i].c_str(),"boolean")==0) || (strcmp(values[i].c_str(),"date")==0) || (strcmp(values[i].c_str(),"char")==0)){
		new_values.push_back(values[i]);		
		}
		else if(strcmp(values[i].c_str(),"varchar")==0){
		new_values.push_back(values[i]);
		new_values.push_back(values[i+1]);
		i++;
		}
		else if(strcmp(values[i].c_str(),"_nn")==0){ 
			new_values.push_back(values[i]);
		}
		else if((strcmp(values[i].c_str(),"_pk")==0) || (strcmp(values[i].c_str(),"_uk")==0)){		
		new_values.push_back(values[i]);
		//new_values.push_back(values[i+1]);
		//i++;
		}
		else if(strcmp(values[i].c_str(),"|")==0){
		new_values.push_back(values[i]);
		//cout << "terminator" << endl;
		}
		else{
				
			for(j=i;j<values.size();j++){
				//cout << "this is values at first j " << j <<  values[j] << endl;
				if((j+1)<values.size()){
										
					if((strcmp(values[j+1].c_str(),"int")==0) || (strcmp(values[j+1].c_str(),"float")==0) || (strcmp(values[j+1].c_str(),"boolean")==0) || (strcmp(values[j+1].c_str(),"date")==0) || (strcmp(values[j+1].c_str(),"char")==0)){
					strcat(buffer2,values[j].c_str());
					new_values.push_back(buffer2);	
					buffer2[0] = '\0';	
					//cout << "datatypes1" << endl;			
					i=j;
					break;	
					}
		
					else if(strcmp(values[j+1].c_str(),"varchar")==0){
					strcat(buffer2,values[j].c_str());
					new_values.push_back(buffer2);
					i=j;
					buffer2[0] = '\0';	
					//cout << "datatypes2" << endl;
					break;
					}
					else if((strcmp(values[j+1].c_str(),"_pk")==0) || (strcmp(values[j+1].c_str(),"_uk")==0) || (strcmp(values[j+1].c_str(),"_nn")==0)){
						
					strcat(buffer2,values[j].c_str());
					new_values.push_back(buffer2);					
					buffer2[0] = '\0';	
					i=j;
					break;
					}
					else if((strcmp(values[j+1].c_str(),"|")==0)){
					strcat(buffer2,values[j].c_str());
					new_values.push_back(buffer2);					
					buffer2[0] = '\0';	
					i=j;
					break;
					}		
					
					else{
					num=strlen(values[j].c_str());
					strcat(buffer2,values[j].c_str());
					i=j;
						}
				}
				
			}//end of for			
		}
	}
			
	return new_values;
}

vector < vector <string> > print_table_properties2(vector<string> &values){
	std::vector < vector <string> > allData;
	std::vector<string> colName;
	std::vector<string> dataType;
	std::vector<string> null;
	std::vector<string> primaryKey;
	std::vector<string> pkName;
	std::vector<string> uniqueKey;
	std::vector<string> ukName;
	/*cout << "\nPrint: ";
	for(int i=1;i<values.size();i++)
		cout << values[i] << " ";
	cout << "\n\n";*/
	for(int i=1;i<values.size();i++){
		
		if(values[i] == "int" || values[i] == "float" || values[i] == "char" || values[i] == "boolean" || values[i] == "date"){
			dataType.push_back(values[i]);
			//cout << "Data Type\n";
		}else if(values[i] == "varchar"){
			dataType.push_back(values[i] + "(" + values[i+1] + ")");
			i++;
			//cout << "Data Type var\n";
		}else if(values[i] == "_nn"){
			null.push_back("True");
			//cout << "Not Null\n";
		}else if(values[i] == "_pk"){
			primaryKey.push_back("True");
			//cout << "Primary Key\n";
			if(values[i+1] != "_uk" && values[i+1] != "|"){
				i++;
				pkName.push_back(values[i]);
				//cout << "PK Name\n";
			}else{
				pkName.push_back("NULL");
				//cout << "PK No Name\n";
			}
		}else if(values[i] == "_uk"){
			uniqueKey.push_back("True");
			//cout << "Unique Key\n";
			if(values[i+1] != "_pk" && values[i+1] != "|"){
				i++;
				ukName.push_back(values[i]);
				//cout << "UK Name\n";
			}else{
				ukName.push_back("NULL");
				//cout << "UK No Name\n";
			}
		}else if(values[i] == "|"){
			if(null.size() != dataType.size()){
				null.push_back("False");
				//cout << "Null\n";
			}

			if(primaryKey.size() != dataType.size()){
				pkName.push_back("NULL");
				primaryKey.push_back("False");
				/*cout << "Not Primary Key\n";
				cout << "PK No Name\n";*/
			}

			if(uniqueKey.size() != dataType.size()){
				ukName.push_back("NULL");
				uniqueKey.push_back("False");
				/*cout << "Not Unique Key\n";
				cout << "UK No Name\n";*/
			}

			//cout << "\n";

		}else if(values[i] != "|"){
			colName.push_back(values[i]);
			//cout << "Col Name\n";
		}
	}
	/*cout << "\n";
	cout << "Col Name: " << colName.size() << "\n";
	cout << "Data Type: " << dataType.size() << "\n";
	cout << "Null: " << null.size() << "\n" ;
	cout << "PK: " << primaryKey.size() << "\n";
	cout << "pkName: " << pkName.size() << "\n";
	cout << "UK: " << uniqueKey.size() << "\n";
	cout << "ukName: " << ukName.size() << "\n";*/
	
	if(!isExport){
		const char separator = ' ';
	    const int nameWidth = 14;
	    const int boolWidth = 7;
	    cout << endl;
	    cout << left << setw(nameWidth) << setfill(separator) << "Column Name";
	    cout << left << setw(nameWidth) << setfill(separator) << "Data Type";
	   	cout << left << setw(boolWidth) << setfill(separator) << "isNN";
	   	cout << left << setw(boolWidth) << setfill(separator) << "isPK";
	   	cout << left << setw(nameWidth) << setfill(separator) << "PK_Name";
	   	cout << left << setw(boolWidth) << setfill(separator) << "isUK";
	   	cout << left << setw(nameWidth) << setfill(separator) << "UK_Name";
		printf("\n--------------------------------------------------------------------------------\n");
		for(int i=0;i<colName.size();i++){
			cout << left << setw(nameWidth) << setfill(separator) << colName[i] << " ";
		    cout << left << setw(nameWidth) << setfill(separator) << dataType[i];
		   	cout << left << setw(boolWidth) << setfill(separator) << null[i];
		   	cout << left << setw(boolWidth) << setfill(separator) << primaryKey[i];
		   	cout << left << setw(nameWidth) << setfill(separator) << pkName[i] << " ";
		   	cout << left << setw(boolWidth) << setfill(separator) << uniqueKey[i];
		   	cout << left << setw(nameWidth) << setfill(separator) << ukName[i] << " ";
		   	cout << "\n";
		}
		return allData;
	}else{
		allData.push_back(colName);
		allData.push_back(dataType);
		allData.push_back(null);
		allData.push_back(primaryKey);
		allData.push_back(pkName);
		allData.push_back(uniqueKey);
		allData.push_back(ukName);

		return allData;
	}

		
}

void print_table_properties(vector<string> &values){				//prints the credentials of the table
	int counter=1;	
	printf("column name \t data type \n");
	printf("========================================================\n");
	for(int i=1;i<values.size();i++){	
	
	if((strcmp(values[i].c_str(),"int")==0) || (strcmp(values[i].c_str(),"float")==0) || (strcmp(values[i].c_str(),"char")==0) || (strcmp(values[i].c_str(),"boolean")==0) || (strcmp(values[i].c_str(),"date")==0)){
		cout << values[i] << " ";
		}


		//constraints
		else if((strcmp(values[i].c_str(),"_pk")==0) || (strcmp(values[i].c_str(),"_uk")==0) || (strcmp(values[i].c_str(),"_nn")==0)){
		if(strcmp(values[i].c_str(),"_pk")==0){		
		cout << "primary key ";
			if((i+1) < values.size()){
			if(strcmp(values[i+1].c_str(),"_uk")==0 || strcmp(values[i+1].c_str(),"_nn")==0 || strcmp(values[i+1].c_str(),"|")==0 ){		
			}
			else {cout << values[i+1] << " ";i++;}
			}		
		}//end of else of _pk	
	
		else if(strcmp(values[i].c_str(),"_uk")==0){
		cout << "unique key ";
		if((i+1) < values.size()){
		//cout << "hello!" << endl;
		if(strcmp(values[i+1].c_str(),"_nn")==0 || strcmp(values[i+1].c_str(),"_pk")==0 || strcmp(values[i+1].c_str(),"|")==0 ){		
			}
		else {cout << values[i+1] << " "; i++;}
		}
		
		}//end of else of _uk

		else if(strcmp(values[i].c_str(),"_nn")==0){
		cout << "not null ";		
		}//end of else of _nn		
		}//end of consraints
		
		//varchar special case
		else if(strcmp(values[i].c_str(),"varchar")==0){
			cout << values[i] << " ";
			cout << values[i+1] << " ";
			i++;
			
		}
		else if(strcmp(values[i].c_str(),"|")==0){
			//cout << endl;
		}
		//for column names 
		else{
			if(counter==1){	cout << values[i] << "\t\t"; counter++;}
			else{ cout << endl; cout << values[i] << "\t\t"; }	
		}

	//cout << "hehe" << endl;
	}//end of for
	cout << endl;
	//cout << values.size() << " ";
	return;
}

bool show_constraints(vector<string> values, const char *constraint_name){			//checks if a constraint is present in a table
	vector<string> constraints;
	int counter,i,j;
			
	for(i=0;i<values.size();i++){
		if((strcmp(values[i].c_str(),"_pk")==0)|| (strcmp(values[i].c_str(),"_nn")==0) || (strcmp(values[i].c_str(),"_uk")==0)){
			if(i+1 < values.size()){			
				if(strcmp(values[i+1].c_str(),"|")==0 || strcmp(values[i+1].c_str(),"_pk")==0 || strcmp(values[i+1].c_str(),"_uk")==0){			
				}
				else { if(strcmp(values[i+1].c_str(),constraint_name)==0) return true;}			
			}			
			}	
	}//end of for
	return false;
}//end of show_constraints

bool check_constraint_name(const char *table_name, const char* constraint_name){			//check a constraint name is present
	vector<string> values;

	if(!check_exist_tmp(table_name)){
	copy_to_tmp(table_name);
	}
	values=show_table_contents(table_name);
	return (show_constraints(values,constraint_name));	
}

bool check_column_name(const char *table_name, const char* column_name){					//check if a column name is present
	vector<string> values;
	
	if(check_exist_tmp(table_name)==0){
	copy_to_tmp(table_name);	
	}	
	values=show_table_contents(table_name);
	values=show_table_names(values);
	for(int i=0;i<values.size();i++){
		if(strcmp(column_name,values[i].c_str())==0){
			return true;
		}
	}
	return false;
}

int get_varchar_size(const char *table_name, std::string column_name){
	vector<string> values;
	if(!check_exist_tmp(table_name))
		copy_to_tmp(table_name);
	values=show_table_contents(table_name);
	
	for(int i=0;i<values.size();i++){
		if(values[i] == column_name){
			if(values[i+1] == "varchar"){
				return atoi(values[i+2].c_str());
			}
		}
	}

	return -1;
}

bool check_data_type(const char *table_name, std::string column_name, std::string data_type){
	vector<string> values;
	if(!check_exist_tmp(table_name))
		copy_to_tmp(table_name);
	values=show_table_contents(table_name);
	
	for(int i=0;i<values.size();i++){
		if(values[i] == column_name)
			if(values[i+1] == data_type)
				return true;
	}

	return false;
}

bool check_item(vector<string> values1, const char* column_name, const char* constraint){	//check is a column name has a constraint
	vector<string> data;																	//if primary key, unique key or not null
	
	for(int i=0; i<values1.size();i++){
		if(strcmp(values1[i].c_str(),column_name)==0){
			int j=i+1;			
			while(strcmp(values1[j].c_str(),"|")!=0){
			if(strcmp(values1[j].c_str(),constraint)==0) return true;
			else j++;
			}
		}
	}
	return false;
}//end of check_item


bool is_primary_key(const char* table_name, const char * column_name){
	vector<string> data;
	if(check_exist_tmp(table_name)==0){
	copy_to_tmp(table_name);	
	}
	data=show_table_contents(table_name);
	/*for(int i=0; i<data.size();i++){
		cout << data[i] << endl;
	}	*/
	return check_item(data,column_name,"_pk");	
	
}//end of is_primary_key

bool is_unique_key(const char* table_name, const char * column_name){
	vector<string> data;
	if(check_exist_tmp(table_name)==0){
	copy_to_tmp(table_name);	
	}
	data=show_table_contents(table_name);	
	return check_item(data,column_name,"_uk");	
}//end of unique_key

bool is_not_null(const char* table_name, const char* column_name){
	vector<string> data;
	if(check_exist_tmp(table_name)==0){
	copy_to_tmp(table_name);	
	}
	data=show_table_contents(table_name);	
	return check_item(data,column_name,"_nn");		
}//end of is_not_null

vector<string> get_not_null_columns(const char *table_name){
	vector<string> values;
	vector<string> colNames;

	if(!check_exist_tmp(table_name))
		copy_to_tmp(table_name);
	values=show_table_contents(table_name);
	values=show_table_names(values);
	
	for(int i=0;i<values.size();i++){
		if(is_not_null(table_name, values[i].c_str()))
			colNames.push_back(values[i]);
	}

	return colNames;
}
