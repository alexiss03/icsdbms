bool insert_op(INSERTQUERYPARSE query)
{
	
	TABLEDATALIST 			** table = get_table(query.tablename),
							* datalist = NULL;
							
	DATA					* data = query.datalist;
	bool 					temp;	
	
	clear_bpt_pool();
	
	if(table == NULL)
	{
		//error("\nTable " + query.tablename + " does not exist\n");
		return false;
	}
	else
	{
		init = clock();
		
		//builds the row to inserted, note that insert is append
		datalist = build_datalist(table, query.datalist, query.tablename);
		if(datalist == NULL)
			return true;

		//inserts 'datalist' into 'table'
		temp  = insert_physical_operator(table, datalist, query.inserttype, query.tablename);
		final = clock()-init;
		time_processed = (float)final / ((float)CLOCKS_PER_SEC);\

		if(temp)
		{
			
			if(!isImport)
				cout << "\n1 row inserted\n";
			return true;
		}
		else
		{
			cout << "\nInsert query not processed\n";
			return false;
		}
	}
	return false;
}

bool insert_physical_operator(TABLEDATALIST ** table, TABLEDATALIST * querydata, string inserttype, string tablename)
{
	if(inserttype.compare("manual") == 0)
	{
		TABLEDATALIST * temp1 = table[0]->up,
				* temp2 = querydata,
				* temp3 = table[0], 
				* temp4 = querydata, 
				* temp5 = NULL;
		
		string strTemp1;
		
		/*checking of CONSTRAintS UNIQUE and PRIMARY*/
		do
		{
			if(is_primary_key(tablename.c_str(), get_string(temp3->data).c_str()) != 0  || is_unique_key(tablename.c_str(), get_string(temp3->data).c_str()) != 0)
			{
				temp5 = temp3->down->down;
				do
				{
					if(temp4->type.compare("")==0 && is_unique_key(tablename.c_str(), get_string(temp3->data).c_str()) != 0)
					{
						//cout << "NULL";
						break;
					}
					else if(is_equal(temp4, temp5) && temp4->data != NULL)
					{
						cout << "Duplicate in primary key or unique key";
						return NULL;
					}
					
					temp5 = temp5->down;
				}while(temp5 != temp3);
			}

			temp3 = temp3->next;
			temp4 = temp4->next;

		}while(temp3 != table[0]);
		
		temp5 = temp2;
		
		do
		{
			temp2->down = temp1->down;
			temp1->down = temp2;
			temp2->up = temp1;
			temp2->down->up = temp2;
			temp2 = temp2->next;
			temp1 = temp1->next;
			temp2->prev = temp1->prev->down;
			
		}while(temp2 != temp5);
		
		return true;
	}
	else
		return false;
}

TABLEDATALIST * build_datalist(TABLEDATALIST ** header, DATA * datalist, string tablename)
{
	TABLEDATALIST 	* temp = header[0],
					* temp3 = NULL,
					* newdata = NULL,
					* previousdata = NULL;
	
	DATA			* temp2 = NULL;
	
	bool boolTemp = false;
		
	do
	{
		boolTemp = false;
		temp2 = datalist;

		while(temp2 !=NULL)
		{
			//return NULL;
			if(get_string(temp2->data).compare(get_string(temp->data)) == 0)
			{
			
				boolTemp = true;
				break;
			}
			temp2 = temp2->next;			
		}

		//return NULL;
		if(boolTemp)
		{
			if(previousdata == NULL)
			{
				newdata = new TABLEDATALIST(temp2->down->data, temp2->down->type);
				previousdata = newdata;
			}
			else
			{
				previousdata->next = new TABLEDATALIST(temp2->down->data, temp2->down->type);
				previousdata = previousdata->next;
			}
		}
		else
		{
			if(is_primary_key(tablename.c_str(), get_string(temp->data).c_str())  == 0 && is_not_null(tablename, get_string(temp->data).c_str()) == 0)
			{			
				if(previousdata == NULL)
				{
					newdata = new TABLEDATALIST(NULL, "");
					previousdata = newdata;
				}
				else
				{
					previousdata->next = new TABLEDATALIST(NULL,"");
					previousdata = previousdata->next;
				}
			}
			else
			{
				//error("Column: " + get_string(temp->DATA) + " cannot be null");
				cout << "Column '" << get_string(temp->data) << "' cannot be null\n";
				return NULL;
			}
		}

		
		temp = temp->next;
	}while(temp != header[0]);
	
	previousdata->next = newdata;
	//print_data(newdata->next->next->next);	
	return newdata;

}



