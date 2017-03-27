void print_cond(CONDLIST  * cond)
{
	CONDLIST * temp1 = cond;	
	do
	{
		if(temp1->value != NULL)
		{
			cout << temp1->column_name << " ";
			cout << temp1->op << " ";
			if(temp1->value == NULL)
			{
				cout << "NULL \t";
			}
			else if(temp1->type.compare("varchar")==0)
			{
				cout << *(static_cast<string *>(temp1->value));
			}
			else if(temp1->type.compare("int")==0)
			{
				cout << *(static_cast<int *>(temp1->value));
			}
			else if(temp1->type.compare("bool")==0)
			{
				if(*(static_cast<bool *>(temp1->value)) == false)
					cout << "0";
				else
					cout << "1";
			}
			else if(temp1->type.compare("float")==0)
			{
				printf("%4f", *(static_cast<float *>(temp1->value)));
			}
			else if(temp1->type.compare("date")==0)
			{
				printf("%02d", (*(static_cast<DATE *>(temp1->value))).month);
				printf("%02d", (*(static_cast<DATE *>(temp1->value))).day);
				printf("%02d", (*(static_cast<DATE *>(temp1->value))).year);
			}
		}
		else
		{
			cout << temp1->column_name << " ";
			cout << temp1->op << " ";
		}
		temp1 = temp1->next;
	}while(temp1 != cond);
	cout << "\n";
}

void print_row(TABLEDATALIST * row)
{
	TABLEDATALIST * temp1 = row;
	cout << "\n";
	do
	{
		if(temp1->data == NULL)
		{
			cout << "NULL \t";
		}
		else if(temp1->type.compare("varchar")==0)
		{
			cout << *(static_cast<string *>(temp1->data)) << "\t";
		}
		else if(temp1->type.compare("int")==0)
		{
			cout << *(static_cast<int *>(temp1->data)) << "\t";
		}
		else if(temp1->type.compare("bool")==0)
		{
			if(*(static_cast<bool *>(temp1->data)) == false)
				cout << "0\t";
			else
				cout << "1\t";
		}
		else if(temp1->type.compare("float")==0)
		{
			printf("%4f \t", *(static_cast<float *>(temp1->data)));
		}
		else if(temp1->type.compare("date")==0)
		{
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).month);
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).day);
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).year);
		}
		
		temp1 = temp1->next;	
		cout << "\n";
	}while(temp1 != row);
}

void print_column(TABLEDATALIST * column)
{
	TABLEDATALIST * temp1 = column;
	cout << "\n";
	do
	{
		if(temp1->data == NULL)
		{
			cout << "NULL \t";
		}
		else if(temp1->type.compare("varchar")==0)
		{
			cout << *(static_cast<string *>(temp1->data)) << "\t";
		}
		else if(temp1->type.compare("int")==0)
		{
			cout << *(static_cast<int *>(temp1->data)) << "\t";
		}
		else if(temp1->type.compare("bool")==0)
		{
			if(*(static_cast<bool *>(temp1->data)) == false)
				cout << "0\t";
			else
				cout << "1\t";
		}
		else if(temp1->type.compare("float")==0)
		{
			printf("%f \t", *(static_cast<float *>(temp1->data)));
		}
		else if(temp1->type.compare("date")==0)
		{
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).month);
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).day);
			printf("%02d \t", (*(static_cast<DATE *>(temp1->data))).year);
		}
		
		temp1 = temp1->down;	
		cout << "\n";
		
	}while(temp1 != column);
}

void print_data(TABLEDATALIST * data)
{
	if(data->type.compare("varchar") == 0)
		cout << "Print data: \t"<<*(static_cast<string *>(data->data)) << "\n";
	else if(data->type.compare("int") == 0)
		cout << "Print data: \t"<<*(static_cast<int *>(data->data)) << "\n";
}

void print_data(DATA * data)
{
	if(data->type.compare("varchar") == 0)
		cout << "Print data: \t"<<*(static_cast<string *>(data->data)) << "\n";
	else if(data->type.compare("int") == 0)
		cout << "Print data: \t"<<*(static_cast<int *>(data->data)) << "\n";
}


vector< pair <string,string> > print_tabledatalist(TABLEDATALIST ** tabledatalist)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST 		* temp1,
						* temp2;
						
	int 				count = 0;

	std::vector< pair<string,string> > tableValues;
	
	//cout << convert_to_string(tabledatalist[0]) << "varchar";
	if (tabledatalist == NULL)
	{
		cout << "Error in data\n";
		return tableValues;
	}
	else if(tabledatalist[0] == NULL)
	{
		cout << "Error in data\n";
		return tableValues;	
	}
	/*else if(tabledatalist[0]->down->down == tabledatalist[0])
	{
		cout << "Table is empty.\n";
		return;
	}*/
	temp2 = temp1 = tabledatalist[0];
	cout << "\n";
	do{

		if(temp1->up != tabledatalist[0])
		{
			do
			{
				if(temp1->data == NULL)
				{
					//cout << "NULL \t|";
					tableValues.push_back(make_pair("number",convert_to_string(temp1)));
				}
				else if(temp1->type.compare("varchar")==0 || temp1->type.compare("CHAR")==0)
				{
					//cout << *(static_cast<string *>(temp1->data)) << "\t|";
					tableValues.push_back(make_pair("string",convert_to_string(temp1)));
				}
				else if(temp1->type.compare("int")==0)
				{
					//cout << *(static_cast<int *>(temp1->data)) << "\t|";
					tableValues.push_back(make_pair("number",convert_to_string(temp1)));
				}
				else if(temp1->type.compare("bool")==0)
				{
					//cout << *(static_cast<bool *>(temp1->data)) << "\t|";
					tableValues.push_back(make_pair("number",convert_to_string(temp1)));
				}	
				else if(temp1->type.compare("float")==0)
				{
					//printf("%.2f \t|", *(static_cast<float *>(temp1->data)));
					tableValues.push_back(make_pair("float",convert_to_string(temp1)));
				}
				temp1 = temp1->next;	
			}while(temp1!=temp2);
			//cout << "\n";	
		}
		temp2 = temp2->down;
		temp1 = temp2;		
	//}while(temp2 != NULL);
	}while(temp2 != tabledatalist[0]);

	if(!isExport){
		int noCol = no_of_columns(tabledatalist);
		const char separator = ' ';
	    const int stringWidth = 25;
	    const int numberWidth = 25;
		
		for(int i=0;i<noCol;i++){
			if(tableValues[i].first == "string")
				cout << left << setw(stringWidth) << setfill(separator) << tableValues[i].second << " ";
			else
				cout << left << setw(numberWidth) << setfill(separator) << tableValues[i].second << " ";
		}
		
		printf("\n--------------------------------------------------------------------------------\n");

		for(int i=noCol;i<(int)tableValues.size();i++){
			if(tableValues[i].first == "string")
				cout << left << setw(stringWidth) << setfill(separator) << tableValues[i].second << " ";
			else
				cout << left << setw(numberWidth) << setfill(separator) << tableValues[i].second << " ";
			
			if(i%noCol == noCol-1)
				cout << "\n";
		}

		//printf("\n");
		count = row_count(temp2);
		if(count > 1)
			cout << "\n" << count << " rows in set";
		else if(count == 0)
		{
			cout << "\n" << count << " rows in set";
		}
		else
			cout << "\n" << count << " row in set";	
		cout << " <" << (float)time_processed	<< " sec>\n\n";
		//circular_test(tabledatalist);
		return tableValues;
	}else{
		return tableValues;
	}

	
}
