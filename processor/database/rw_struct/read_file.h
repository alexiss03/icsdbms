#include "concat_data.h"
using namespace std;

TABLEDATALIST** read_file(const char * table_name, vector<string> datatypes, vector<string> col_names){

	std::vector<string> data_contents1;	
	std::vector<string> data_contents;
	char filename[1000];	
	int c=0;	
	filename[0]='\0';
	strcat(filename,"processor/database/data/tmp/");	
	strcat(filename,table_name);
	strcat(filename,".dat");
	std::vector<std::string> values;


	
	ifstream fp1;
	fp1.open(filename,ios::binary);
	fp1.seekg (0, fp1.end);
    long length = fp1.tellg();
    fp1.seekg (0, fp1.beg);
	char buffer[length];
	//cout << length;
	length = (length/8)+1;
	buffer[8] = '\0';
	//cout << "length " << length << endl;
	
	while(fp1.read((char*)buffer,sizeof(string)*2)){
			//cout << buffer << endl;
			data_contents1.push_back(buffer);
			buffer[8]='\0';		
          }
          fp1.close();	
	
	data_contents = clean_data(data_contents1);
		
	TABLE * table =  new TABLE();
	//colname
	TABLEDATALIST  ** head, * temp1, * temp2, * temp3, * temp4, *p,*q;
	head = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *)*100);
	
	p=head[0];
	q=head[1];
	
	for(int i=0;i<col_names.size();i++){
		if(i==0){	
			head[0] = new TABLEDATALIST(new string(col_names[i].c_str()),"varchar");
			head[1] = new TABLEDATALIST(new string(datatypes[i].c_str()), "varchar");
			head[0]->next = head[0];		
			head[1]->next = head[1];		
			head[0]->prev = head[0];
			head[1]->prev = head[1];
			p=head[0];
			q=head[1];		
		}	
		else{
			temp1 = new TABLEDATALIST(new string(col_names[i].c_str()),"varchar");
			temp1->prev = p;		
			temp1->next = head[0];
			p->next = temp1;		
			p=p->next;
		
			temp2 = new TABLEDATALIST(new string(datatypes[i].c_str()),"varchar");
			temp2->prev = q;		
			temp2->next = head[1];
			q->next = temp2;		
			q=q->next;	
		}
	}//end of for
	int no_of_rows = data_contents.size()/datatypes.size();
	int no_of_datatypes = datatypes.size();	
	
	if(col_names.size() != 1)
	{
		head[0]->prev = temp1;
		head[1]->prev = temp2;
		connect_two_rows_vertical(head[0],head[1]);
		connect_two_rows_vertical(head[1],head[0]);
	
	//populate data
	int j=2;
	int counter=0;	
	for(int i=0; i<data_contents.size(); i++){
		if((i%no_of_datatypes==0 || i==0 ) && counter<no_of_datatypes){
			if(i==0){			
				if(data_contents[i].substr(0,4).compare("NULL") != 0)
				{
					if(strcmp(datatypes[counter].c_str(),"int")==0){
					int num = atoi(data_contents[i].c_str());
					head[2]= new TABLEDATALIST(new int(num), "int");
							
					}
			
					if(strcmp(datatypes[counter].c_str(),"varchar")==0){
										
						head[2]= new TABLEDATALIST(new string(data_contents[i].c_str()), "varchar");
						}	 

					if(strcmp(datatypes[counter].c_str(),"float")==0){
						float num = atof(data_contents[i].c_str());
						head[2]= new TABLEDATALIST(new float(num), "float");

					}

					if(strcmp(datatypes[counter].c_str(),"char")==0){
						head[2]= new TABLEDATALIST(new string(data_contents[i].c_str()), "char");
					}
				}
				else
				{
					head[2] = new TABLEDATALIST(NULL, "");
				}
			}//end of if
			else{
				head[j]->prev = temp1;
				j++;
				if(data_contents[i].substr(0,4).compare("NULL") != 0)
				{
					if(strcmp(datatypes[counter].c_str(),"int")==0){
						int num = atoi(data_contents[i].c_str());
						head[j]= new TABLEDATALIST(new int(num), "int");
					}
					if(strcmp(datatypes[counter].c_str(),"varchar")==0){
						head[j]= new TABLEDATALIST(new string(data_contents[i].c_str()), "varchar");
					} 
					if(strcmp(datatypes[counter].c_str(),"float")==0){
						float num = atof(data_contents[i].c_str());
						head[j] = new TABLEDATALIST(new float(num), "float");	
					}
					if(strcmp(datatypes[counter].c_str(),"char")==0){
						head[j] = new TABLEDATALIST(new string(data_contents[i].c_str()), "char");
					}
				}
				else
				{
					head[j] = new TABLEDATALIST(NULL, "");
				}
			}
			head[j]->next = head[j];		
			head[j]->prev = head[j];
			p=head[j];
			counter++;

					
		}//big if
		
		else if(counter<no_of_datatypes){
			if(data_contents[i].substr(0,4).compare("NULL") != 0)
			{
				if(strcmp(datatypes[counter].c_str(),"int")==0){
					int num = atoi(data_contents[i].c_str());
					temp1= new TABLEDATALIST(new int(num), "int");
				}
				
				if(strcmp(datatypes[counter].c_str(),"varchar")==0){
					temp1 = new TABLEDATALIST(new string(data_contents[i].c_str()), "varchar");
				} 
				if(strcmp(datatypes[counter].c_str(),"float")==0){
					float num = atof(data_contents[i].c_str());
					temp1 = new TABLEDATALIST(new float(num), "float");
				}
				if(strcmp(datatypes[counter].c_str(),"char")==0){
					temp1 = new TABLEDATALIST(new string(data_contents[i].c_str()), "char");
				}
			}
			else
			{
				temp1 = new TABLEDATALIST(NULL, "");
			}

			temp1->prev = p;		
			temp1->next = head[j];
			p->next = temp1;		
			p=p->next;		
			counter++;

				
		}	
		if(counter==no_of_datatypes){
			counter=0;
		}
				
	}//end of for
		if(no_of_rows > 0)
			head[j]->prev = temp1;
	
		for(int i=0;i<(no_of_rows+2);i++){
			if(i==no_of_rows+1){
			connect_two_rows_vertical(head[i],head[0]);
			}
			else{
			connect_two_rows_vertical(head[i],head[i+1]);		
			}	
		}
	
	
		table->table = head;
		table->tablename = table_name;
		//circular_test(head);
		return head;	
	}//end of id
	else{
		head[0]->prev = head[0];
		head[1]->prev = head[1];		
		connect_two_rows_vertical(head[0],head[1]);
		connect_two_rows_vertical(head[1],head[0]);
		
	//populate data
	int j=2;
	
	
	
		for(int i=0; i<data_contents.size(); i++){					
					if(data_contents[0].substr(0,4).compare("NULL") != 0)
					{
					if(strcmp(datatypes[0].c_str(),"int")==0){
					int num = atoi(data_contents[i].c_str());
					head[j]= new TABLEDATALIST(new int(num), "int");			
					}
			
					if(strcmp(datatypes[0].c_str(),"varchar")==0){	
					head[j]= new TABLEDATALIST(new string(data_contents[i].c_str()), "varchar");
					}	 
					if(strcmp(datatypes[0].c_str(),"float")==0){
						float num = atof(data_contents[i].c_str());
						//num = data_contents[i].c_str();						
						head[j] = new TABLEDATALIST(new float(num), "float");
						//return head;	
					}

					if(strcmp(datatypes[0].c_str(),"char")==0){
						head[j]= new TABLEDATALIST(new string(data_contents[i].c_str()), "char");
					}
					
				}//end of if
				else
					{
					head[j] = new TABLEDATALIST(NULL, "");
					}
				head[j]->next = head[j];
				head[j]->prev = head[j];
				j++;
						
		}//end of for
		
		
	
	
	
		for(int i=0;i<(no_of_rows+2);i++){
			if(i==no_of_rows+1){
			connect_two_rows_vertical(head[i],head[0]);
			}
			else{
			connect_two_rows_vertical(head[i],head[i+1]);		
			}	
		}
	
	
		table->table = head;
		table->tablename = table_name;	


	
	/**print_row(head[2]);
	print_row(head[3]);
	print_row(head[4]);
	print_row(head[5]);	
	print_row(head[6]);**/
	//print_row(head[7]);	
	//circular_test(head);
	return head;
		
	}//end of else
	if(no_of_rows == 0)
		return head;
	
}//end of read_file


TABLEDATALIST **read_file_start(const char* table_name){
	vector<string> values;
	vector<string> datatypes;
	vector<string> col_names;
	values=show_table_contents(table_name);				//gets the values of the table
	datatypes=show_datatypes(values);
	col_names=show_table_names(values);						
	//return NULL;	
	return (read_file(table_name,datatypes,col_names));
}

