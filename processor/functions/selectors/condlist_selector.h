/*
	selects the rows given search_condition
*/
TABLEDATALIST ** condlist_selector(	TABLEDATALIST ** table, 
									CONDLIST * search_condition,
									TABLEDATALIST * column_search = NULL,
									TABLEDATALIST * row_search = NULL
									)
{
	TABLEDATALIST 		** rows = NULL,
						** test;
	
	CONDLIST 			* temp = search_condition;
	
	string 				opTemp,
						column_nameTemp,
						typeTemp,
						opTemp2 = "";

	DATA				* dataTemp;

	if(temp == NULL)
	{
		rows = select_all_rows(table);
		return rows;
		//return NULL;
	}
	else
	{
		do
		{
			opTemp = temp->op;
			column_nameTemp = temp->column_name;
			typeTemp =  temp->type;
			
			if(opTemp.compare("<") == 0)
			{
				rows = selector(table, column_nameTemp, typeTemp, NULL, false, temp->value, false, rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare(">") == 0)
			{
				rows = selector(table, column_nameTemp, typeTemp, temp->value, false, NULL, false, rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare("<=") == 0)
			{
				rows = selector(table, column_nameTemp, typeTemp, NULL, false, temp->value, true, rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare(">=") == 0)
			{
				rows = selector(table, column_nameTemp, typeTemp, temp->value, true, NULL, false, rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare("=") == 0) 
			{
				rows = selector(table, column_nameTemp, typeTemp, temp->value, true, temp->value, true, rows, opTemp2, column_search, row_search);
				
			}
			else if(opTemp.compare("#") == 0) 
			{
				rows = selector(table, column_nameTemp, typeTemp, temp->value, false, temp->value, false, rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare("> any") == 0) 
			{
				dataTemp = search_condition->column1;
				while(dataTemp != NULL)
				{
					rows = selector(table, column_nameTemp, typeTemp, dataTemp->data, false, NULL, false, rows, opTemp2, column_search, row_search);
					dataTemp = dataTemp->next;
					opTemp2 = "or";
				}	
			}
			else if(opTemp.compare("< any") == 0) 
			{
				dataTemp = search_condition->column1;

				while(dataTemp != NULL)
				{
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, dataTemp->data, false, rows, opTemp2, column_search, row_search);
					dataTemp = dataTemp->next;
					opTemp2 = "or";
				}
			}
			else if(opTemp.compare("> all") == 0) 
			{
				dataTemp = search_condition->column1;

				while(dataTemp != NULL)
				{
					rows = selector(table, column_nameTemp, typeTemp, dataTemp->data, false, NULL, false, rows, opTemp2, column_search, row_search);
					dataTemp = dataTemp->next;
					opTemp2 = "and";
				}
			}
			else if(opTemp.compare("< all") == 0) 
			{
				dataTemp = search_condition->column1;

				while(dataTemp != NULL)
				{
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, dataTemp->data, false, rows, opTemp2, column_search, row_search);
					dataTemp = dataTemp->next;
					opTemp2 = "and";
				}
			}
			else if(opTemp.compare("between") == 0)
			{
				if(get_int(temp->column1->data) < get_int(temp->column1->next->data))
				{
					rows = selector(table, column_nameTemp, typeTemp, temp->column1->data, false, NULL, false, rows, opTemp2, column_search, row_search);
					opTemp2 = "and";
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, temp->column1->next->data, false, rows, opTemp2, column_search, row_search);
				}
				else
				{
					rows = selector(table, column_nameTemp, typeTemp, temp->column1->next->data, false, NULL, false, rows, opTemp2, column_search, row_search);
					opTemp2 = "and";
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, temp->column1->data, false, rows, opTemp2, column_search, row_search);	
				}
			}
			else if(opTemp.compare("not between") == 0)
			{
				if(get_int(search_condition->column1->data) < get_int(search_condition->column1->next->data))
				{
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, search_condition->column1->data, true, rows, opTemp2, column_search, row_search);
					opTemp2 = "or";
					rows = selector(table, column_nameTemp, typeTemp, search_condition->column1->next->data, true, NULL, true, rows, opTemp2, column_search, row_search);
				}
				else
				{
					rows = selector(table, column_nameTemp, typeTemp, NULL, false, search_condition->column1->next->data, true, rows, opTemp2, column_search, row_search);
					opTemp2 = "or";
					rows = selector(table, column_nameTemp, typeTemp, search_condition->column1->data, true, search_condition->column1->data, true, rows, opTemp2, column_search, row_search);	
				}
			}
			else if (opTemp.compare("like") == 0)
			{ 
				rows = selector(table, column_nameTemp, "like", static_cast<string *>(temp->value), rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare("is null") == 0)
			{
				rows = selector(table, column_nameTemp, "is null", rows, opTemp2, column_search, row_search);
			}
			else if(opTemp.compare("is not null") == 0)
			{
				rows = selector(table, column_nameTemp, "is not null", rows, opTemp2, column_search, row_search);
			}
			else if (opTemp.compare("and") == 0 || opTemp.compare("or") == 0)
 			{
				opTemp2 = opTemp;
			}
			else
			{
				opTemp2 = "";
			}
			
			temp = temp->next;
		}
		while(temp != NULL);
		
		return rows;
		
	}
	return rows;
}

/*
	counts the rows satisfying the condition 'search_condition'
*/
int cond_stat(	TABLEDATALIST ** table,
				CONDLIST * search_condition,
				TABLEDATALIST * column_search = NULL,
				TABLEDATALIST * row_search = NULL
			)
{
	
	CONDLIST 			* temp = search_condition;
	
	string 				opTemp,
						column_nameTemp,
						typeTemp,
						opTemp2 = "";
						
	int 				count;
						
	
	if(table == NULL)
		return -1;
	else
	{
	
		
		opTemp = temp->op;
		column_nameTemp = temp->column_name;
		typeTemp =  temp->type;
		
		if(opTemp.compare("<") == 0)
		{
			return row_count(table, column_nameTemp, typeTemp, NULL, false, temp->value, false, column_search, row_search);
		}
		else if(opTemp.compare(">") == 0)
		{
			return row_count(table, column_nameTemp, typeTemp, temp->value, false, NULL, false, column_search, row_search);
		}
		else if(opTemp.compare("<=") == 0)
		{
			return count = row_count(table, column_nameTemp, typeTemp, NULL, false, temp->value, true, column_search, row_search);
		}
		else if(opTemp.compare(">=") == 0)
		{
			return count = row_count(table, column_nameTemp, typeTemp, temp->value, true, NULL, false, column_search, row_search);
		}
		else if(opTemp.compare("=") == 0) 
		{
			return count = row_count(table, column_nameTemp, typeTemp, temp->value, true, temp->value, true, column_search, row_search);
		}
		else if(opTemp.compare("=") == 0 && typeTemp.compare("varchar") == 0) 
		{
			return count = row_count(table, column_nameTemp, "=", static_cast<string *>(temp->value), column_search, row_search);
		}
		else if (opTemp.compare("like") == 0)
		{ 
			return count = row_count(table, column_nameTemp, "like", static_cast<string *>(temp->value), column_search, row_search);
		}
	}
}