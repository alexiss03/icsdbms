void write_tabledatalist(TABLEDATALIST ** tabledatalist, const char* table_name)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST 		* temp1,
						* temp2;
						
	int count = 0;
	vector<string> data;	
	char command[1000];
	command[0]='\0';
	strcat(command,"processor/database/data/tmp/");
	strcat(command,table_name);

	strcat(command,".dat");
	//ofstream myfile (command);
	//ofstream ofs;
	//ofs.open(command,ios::binary);	

	if (tabledatalist == NULL || tabledatalist[0] == NULL)
	{
		cout << "TABLE EMPTY";
		
	}
	temp2 = temp1 = tabledatalist[1]->down;

	//cout << "\n";

	if(row_count(tabledatalist[0]) != 0)
	{
		do{
			if(temp1->up != tabledatalist[0])
			{
				do
				{
					if(temp1->data == NULL)
					{
						data.push_back("NULL");
						data.push_back("|");
					}
					else if(temp1->type.compare("varchar")==0 || temp1->type.compare("char")==0)
					{
						data.push_back(convert_to_string(temp1));
						data.push_back("|");
						
					}
					else if(temp1->type.compare("int")==0)
					{
						data.push_back(convert_to_string(temp1));
						data.push_back("|");				
					}
					else if(temp1->type.compare("bool")==0)
					{
						data.push_back(convert_to_string(temp1));
						data.push_back("|");
					}
					else if(temp1->type.compare("float")==0)
					{
						
						data.push_back(convert_to_string(temp1));
						data.push_back("|");
					}
					temp1 = temp1->next;	
				}while(temp1!=temp2);
				//cout << "\n";	
			}
			temp2 = temp2->down;
			temp1 = temp2;		
		
		}while(temp2 != tabledatalist[0]);
		
	}
	
	ofstream ofs;
	ofs.open(command,ios::binary);
		
	for(int i=0;i<data.size();i++){
	long length =strlen(data[i].c_str());	
	length=(length/8)+1;
	ofs.write(data[i].c_str(),sizeof(string)*2*(length));
	}
	ofs.flush();	
	ofs.close();
	
}//end
