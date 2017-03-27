bool select_op(SELECTQUERYPARSE query)
{

	TABLE 		 	* table1, 
					* table2;

	TABLEDATALIST	** table,
					** rows = NULL,
					** select_table = NULL;
					
	CONDLIST 		* condlist = query.search_condition;
	
	string 			opTemp,
					column_nameTemp,
					typeTemp,
					opTemp2 = "";	

	DATA 			* dataTemp;
	init = clock();
	if(query.table->next == NULL)
	{
		select_table = get_table(get_string(query.table->data));
	}	
	else
	{	
		get_table(get_string(query.table->data));
		get_table(get_string(query.table->next->next->data));

		table1  = get_table_from_table_pool(get_string(query.table->data));
		table2 = get_table_from_table_pool(get_string(query.table->next->next->data));

		select_table = join_op(*table1, *table2, query.join_condition, get_string(query.table->next->data));

		dataTemp = query.table->next->next->next;
		
		while(dataTemp != NULL)
		{
			table2 = get_table_from_table_pool(get_string(dataTemp->next->data));
			select_table = join_op(*(new TABLE(select_table, "join")), *table2, query.join_condition, get_string(dataTemp->data));
			dataTemp = dataTemp->next->next;
		}

	}

	
	if(table == NULL)
	{
		return false;
	}
	else
	{
		//memcpy(select_table, table, sizeof(TABLEDATALIST**));	
		select_table = create_new_tabledatalist(select_table, select_all_rows(select_table), NULL);	
		

		//the table must not be empty
		if(select_table[0] != select_table[0]->down->down)
		{
			//selects the rows using 'query.search_condition'
		 	rows = condlist_selector(select_table, query.search_condition);		
		 	select_table = create_new_tabledatalist(select_table, rows, NULL);
		}

		select_table =  group_by(select_table, query.group_grouping, query.columnlist, query.group_condition);

		rows = group_by_operator(select_table, query.group_condition, query.group_grouping, query.columnlist);
		
		
		if(query.order_grouping != NULL)
		{
			rows = order_by(select_table, get_string(query.table->data), query.order_grouping);
			select_table = create_new_tabledatalist(select_table, rows, query.columnlist);		
		}
		else
		{
			if(rows != NULL)
				select_table = create_new_tabledatalist(select_table, rows, query.columnlist);
			else
				select_table = create_new_tabledatalist(select_table, select_all_rows(select_table), query.columnlist);
		}


		select_display_op(select_table, query.columnlist, query.group_grouping);
		select_table = select_display_constraint(select_table, query.display_constraint, query.columnlist);

		//return true;
		final = clock() - init;
		time_processed = (float)final / ((float)CLOCKS_PER_SEC);
	}

	print_tabledatalist(select_table);
	return true;
}

bool select_display_op(TABLEDATALIST ** tabledatalist, DATA * columnlist, DATA * group_grouping)
{

	TABLEDATALIST 	** table = tabledatalist,
					* tabledataTemp = tabledatalist[0];
					
	

	DATA 			* tempColList = columnlist,
					* dataTemp;
	//return false;
	if(tempColList != NULL)
	{
		do
		{
			//cout << get_string(tempColList->data) << convert_to_string(tabledataTemp);
			if(columnlist != NULL)
			{
				if(get_string(tempColList->data).compare(convert_to_string(tabledataTemp)) == 0)
				{
					dataTemp = tempColList;
					while(dataTemp->down != NULL)
					{
						if(select_display_alteration(tabledataTemp, get_string(dataTemp->down->data), dataTemp->down->down, group_grouping, table))
							dataTemp = dataTemp->down->down;
						else
							dataTemp = dataTemp->down->down;
					}	

				}
			}
			
			tabledataTemp = tabledataTemp->next;
			//tempColList = tempColList->next;
		}while(tabledataTemp != tabledatalist[0]);
	}
	//return true;
	//select_display_constraint(tabledatalist, display_constraint);
	select_alias(tabledatalist, columnlist);
	return true;
}

bool select_display_alteration(TABLEDATALIST * column, string op, DATA * value, DATA * group_grouping, TABLEDATALIST ** table)
{

	TABLEDATALIST 	* temp = column->down->down;

	int 			intTemp;

	float 			floatTemp;
	
	if(op.compare("+") == 0 || op.compare("-") == 0 || op.compare("*") == 0 || op.compare("/") == 0)
	{
		if(temp->type.compare("int") == 0)
		{
			intTemp = get_int(value->data);
			do
			{
				if(temp->type.compare("int") == 0)
				{
					if(op.compare("+") == 0)
						temp->data = new int(get_int(temp->data) + intTemp);
					else if(op.compare("-") == 0)
						temp->data = new int(get_int(temp->data) - intTemp);
					else if(op.compare("*") == 0)
						temp->data = new int(get_int(temp->data) * intTemp);
					else if(op.compare("/") == 0)
						temp->data = new int(get_int(temp->data) / intTemp);
				}
				else
				{
					temp->data = NULL;
				}
				temp = temp->down;
			}while(temp != column);
		}
		else if(temp->type.compare("float") == 0)
		{
			floatTemp = get_float(value->data);
			do
			{
				if(temp->type.compare("float") == 0)
				{
					if(op.compare("+") == 0)
						temp->data = new float(get_float(temp->data) + floatTemp);
					else if(op.compare("-") == 0)
						temp->data = new float(get_float(temp->data) - floatTemp);
					else if(op.compare("*") == 0)
						temp->data = new float(get_float(temp->data) * floatTemp);
					else if(op.compare("/") == 0)
						temp->data = new float(get_float(temp->data) / floatTemp);
				}
				else
				{
					temp->data = NULL;
				}
				temp = temp->down;
			}while(temp != column);
		}
		return true;
	}
	else if(op.compare("substr") == 0)
	{
		substr(column, get_int(value->data), get_int(value->next->data));
	}
	return true;
}
	

/*
	Performs the column renaming
*/
bool select_alias(TABLEDATALIST ** table, DATA * tempColList)
{
	TABLEDATALIST * temp;
	DATA * dataTemp = tempColList;
	
	while(dataTemp != NULL)
	{
		if(dataTemp->up != NULL)
		{
			temp = find_column(table, get_string(dataTemp->data), NULL);
			if(temp != NULL)
			{	
				temp->data = new string(get_string(dataTemp->up->data));
			}
		}
		dataTemp = dataTemp->next;
	}
	return true;
}


/*
	Performs the display constraint
*/
TABLEDATALIST ** select_display_constraint(TABLEDATALIST ** table, DATA * display_constraint, DATA * columnlist)
{
	int 				temp,
						r_count = row_count(table[0]);

	TABLEDATALIST 		** rows;

	if(display_constraint == NULL)
	{
		return table;
	}
	else if(get_string(display_constraint->data).compare("top") == 0)
	{
		temp = get_int(display_constraint->down->data);
		if(display_constraint->down->down == NULL)
		{
			if(temp < r_count)
				rows = select_top_rows(table, temp);
			else
				return table;
		}
		else if(get_string(display_constraint->down->down->data).compare("%") ==0)
		{
			temp = temp * r_count / 100;
			if(temp < r_count)
				rows = select_top_rows(table, temp);
			else
				return table;
		}
		return create_new_tabledatalist(table, rows, NULL);	
	}
	else if(get_string(display_constraint->data).compare("distinct") == 0)
	{
		rows = select_all_distinct_rows(table, find_column(table, get_string(columnlist->data), table[0]->prev));
		return create_new_tabledatalist(table, rows, NULL);
	}
}