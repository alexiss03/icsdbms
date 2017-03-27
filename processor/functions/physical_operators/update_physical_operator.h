/*
	Performs update operator
*/
bool  update_op(UPDATEQUERYPARSE query)
{
	
	TABLEDATALIST 	** table = get_table(query.tablename), 
					** rows = NULL;
					
	string 			opTemp,
					column_nameTemp,
					typeTemp,
					opTemp2 = "";
	bool			temp,
					toPerform = true;
	int 			i;
	DATA 			* dataTemp;
					
	clear_bpt_pool();
	
	if(table == NULL)
		return false;
	else
	{
		if(query.search_condition == NULL)
		{
			rows = select_all_rows(table);
			i = row_count(table[0]);

			dataTemp = query.column_value;
			do
			{
				//filters if the column to be updated is primary key
				if(is_primary_key(query.tablename.c_str(), get_string(dataTemp->data).c_str()) != 0 && i > 1)			
				{
					toPerform = false;
					cout << "\nQuery will result to duplicate primary keys\n";
					break;
				}
				//filters if the column to be updated is unique
				else if(is_unique_key(query.tablename.c_str(), get_string(dataTemp->data).c_str()) != 0 && i > 1 && dataTemp->down->data != NULL)
				{
					toPerform = false;
					cout << "\nQuery will result to duplicate unique keys\n";
					break;
				}


				dataTemp = dataTemp->next;
			}
			while(dataTemp != NULL);

			if(toPerform)
				temp = update_physical_operator(table, query.column_value, rows, query.tablename);
			else
			{
				temp = false;
				return false;
			}
			if(!isImport)
				cout << "\n" << i <<" row/s updated\n";	

			return false;
		}
		else
		{
			init = clock();

			rows = condlist_selector(table,query.search_condition);
			for(i=0; *(rows + i)!= NULL; i++);

			dataTemp = query.column_value;
			do
			{
				if(is_primary_key(query.tablename.c_str(), get_string(dataTemp->data).c_str()) != 0 && i > 1)			
				{
					toPerform = false;
					cout << "\nQuery will result to duplicate primary keys\n";
					break;
				}
				else if(is_unique_key(query.tablename.c_str(), get_string(dataTemp->data).c_str()) != 0 && i > 1 && dataTemp->down->data != NULL)
				{
					toPerform = false;
					cout << "\nQuery will result to duplicate unique keys\n";
					break;
				}


				dataTemp = dataTemp->next;
			}
			while(dataTemp != NULL);

			if(toPerform)
				temp = update_physical_operator(table, query.column_value, rows, query.tablename);
			else
				temp = false;
			
			final = clock()-init;
			time_processed = (float)final / ((float)CLOCKS_PER_SEC);

			if(temp)
			{	
				if(!isImport)
					cout << "\n" << i <<" row/s updated\n";	
				return true;
			}
			else
			{
				cout << "\n Update query not processed\n";
				return false;
			}
		}
	}	
		
}

bool update_physical_operator(TABLEDATALIST ** tabledatalist, DATA * column_value, TABLEDATALIST ** rows ,string tablename = "")
{

	TABLEDATALIST ** 	tabledataTemp1 = rows,
						* tabledataTemp2 = tabledatalist[0], 
						* firstRow = NULL,
						* temp3;
						
	DATA * 				dataTemp1 = column_value;
	
	string 				column_name, 
						stringTemp1;
						
	int 				i=0;

	int					is_primary,
						is_unique;

	bool 				is_eq;
	
	if(tabledatalist == NULL || rows == NULL)
	{
		return false;
	}
	
	for(int i = 0; *(rows + i) != NULL; i++)
	{
		//cout << "rows\n";
	}
	
	while(dataTemp1 != NULL)
	{	
		//locates the column to be updated
		do
		{
			stringTemp1 = *(static_cast<string *>(tabledataTemp2->data));
			if((*(static_cast<string *>(dataTemp1->data))).compare(stringTemp1) == 0)
				break;
			else
			{
				tabledataTemp2 = tabledataTemp2->next;
				for(i=0; *(tabledataTemp1+i) != NULL ; i++)
				{
					*(tabledataTemp1+i) = &(*(*(tabledataTemp1 + i))->next);
				}
			}
		}while(tabledataTemp2 != tabledatalist[0]);
		
		//locates the first data row
		firstRow = tabledataTemp2->down->down;
		is_primary = is_primary_key(tablename.c_str(), get_string(tabledataTemp2->data).c_str());
		is_unique = is_unique_key(tablename.c_str(), get_string(tabledataTemp2->data).c_str());

		do
		{
			for(i=0; *(tabledataTemp1+i) != NULL ; i++)
			{
				if(*(tabledataTemp1+i) == firstRow)
				{		
					temp3 = firstRow;
					is_eq = false;
					if(is_primary  != 0 ||  is_unique != 0)
					{
						//cout << "primary";
						do
						{
							if(temp3->data == NULL)
							{

							}
							else if(is_void_equal(dataTemp1->down->type, dataTemp1->down->data, temp3->data))
							{
								is_eq = true;
								break;
							}	
							temp3 = temp3->down;
						}
						while(temp3 != firstRow);
					}
					
					if(!is_eq)
					{	

						(*(tabledataTemp1+i))->data = copy_data(dataTemp1->down->data, dataTemp1->down->type, dataTemp1->down->type);
						(*(tabledataTemp1+i))->type = dataTemp1->down->type;
					}
					else
					{
						cout << "\nSemantic Error : Primary key or unique key duplicate\n";
						return false;
						//error("Primary key or unique key duplicate");
					}
				}	
				
			}
			firstRow = firstRow->down;		
		}while(firstRow != tabledataTemp2);
		
		dataTemp1 = dataTemp1->next;
	}
	return true;
}

