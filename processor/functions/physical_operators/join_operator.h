/*
	Performs join operator
*/
TABLEDATALIST ** join_op(TABLE table, TABLE table2, CONDLIST * cond = NULL, string table_op = "")
{
	
	TABLEDATALIST 	* table_column,
					* table2_column,
					** table_data,
					** table2_data,
					* table_start,
					* table2_start,
					* table_end,
					* table2_end,
					** join_table;
	
	DATA			* temp;

	CONDLIST		* column,
					* column2;
					
	string 			tablename1,
					tablename2,
					op;

	join_table = join(table.table, table2.table);
;
	if(cond != NULL)
	{	
		column = cond;
		column2 = cond->next;
	}
	
	table_start = join_table[0];
	table2_start = get_other_table(join_table);
	
	table2_end = table_start->prev;
	table_end = table2_start->prev;


	//case: cross join
	if(cond == NULL || row_count(table.table[0]) == 0 || row_count(table2.table[0]) == 0)
	{
		tablename1 = table.tablename;
		tablename2 = table2.tablename;
		table_data = &join_table[0];
		table_start->constraint = tablename1;
		table2_data = &table2_start;
		table2_start->constraint = tablename2;
		join_table[0]->prev->next = join_table[0];
		return join_table;
	}
	//case: other join types
	else
	{	
		join_table[0]->prev->next = join_table[0];
		tablename1 = table.tablename;
		tablename2 = table2.tablename;
		table_data = &join_table[0];
		table_start->constraint = tablename1;
		table2_data = &table2_start;
		table2_start->constraint = tablename2;
		

		if(get_string(column->column2->data).compare(tablename1) == 0)
			table_column = find_column(table_data, column->column_name, table_end->next);
		else
			table_column = find_column(table2_data, column->column_name, table2_end->next);

		if(get_string(column2->column2->data).compare(tablename1) == 0)
			table2_column = find_column(table_data, column2->column_name, table_end->next);
		else
			table2_column = find_column(table2_data, column2->column_name, table2_end->next);

		if(convert_to_string(table_column->down).compare("varchar") == 0)
			op = "like";
		else
			op = "=";
		//return NULL;
		if(table_op.compare("inner join") == 0)
		{
			return inner_join(join_table, table_column, table2_column, op);
		}
		else if(table_op.compare("left join") == 0 || table_op.compare("left outer join") == 0)
		{
			return left_join(join_table, table_column, table2_column, op, row_count(table2.table[0]), table2_data[0]);
		}
		else if(table_op.compare("right join") == 0 || table_op.compare("right outer join") == 0)
		{
			return right_join(join_table, table_column, table2_column, op, row_count(table.table[0]), row_count(table2.table[0]), table2_data[0]);
		}
		else if(table_op.compare("full join") == 0)
		{
			return full_join(join_table, table_column, table2_column, op, row_count(table.table[0]), row_count(table2.table[0]), table2_data[0]);
		}
	}	
}

/*
	Performs right join
*/
TABLEDATALIST ** right_join(TABLEDATALIST ** join_table, TABLEDATALIST * column, TABLEDATALIST * column2, string cond_op, int row_count_table1, int row_count_table2, 
				TABLEDATALIST * table2_start)
{
	TABLEDATALIST 	* temp = join_table[0]->down->down,
					* temp2 = column->down->down,
					* temp3 = column2->down->down,
					* temp4 = table2_start->down->down,
					** rows,
					** selected_rows;
	
	CONDLIST 		* cond_temp = new CONDLIST();
	
	
	int 			count = 0,
					i = 0,
					r_count = row_count(join_table[0]);
					
	bool			has_selected[row_count_table2];
	
	for(int j = 0; j < row_count_table2; j++)
		has_selected[j] = false;
	
	selected_rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * r_count + 1);
	
	for(int i = 0; i < r_count + 1; i++)
		*(selected_rows + i) = NULL;
	
	cond_temp->op = cond_op;				
		
	do
	{
		cond_temp->value = temp3->data;
		cond_temp->type = temp3->type;
		if(cond_temp->value != NULL && temp3->data != NULL)
			rows = condlist_selector(join_table, cond_temp, column, temp2);
		
		i++;
		
		if(*(rows + 0) != NULL)
		{
			*(selected_rows + count++) = *(rows + 0);
			*(rows + 0) = NULL;
			has_selected[i % row_count_table2] = true;
		}
		
		if(i > ((row_count_table1 - 1) * row_count_table2))
		{
			if(has_selected[row_count_table2 - (row_count_table1 * row_count_table2 - i)] == false)
			{
				*(selected_rows + count++) = temp;
				make_row_data_null(temp, temp4);
			}
		}	
		
		temp4 = temp4->down;
		temp3 = temp3->down;
		temp2 = temp2->down;
		temp = temp->down;
		
	}
	while(temp != join_table[0]);
		
	return create_new_tabledatalist(join_table, selected_rows);

}


TABLEDATALIST ** full_join(TABLEDATALIST ** join_table, TABLEDATALIST * column, TABLEDATALIST * column2, string cond_op, int row_count_table1, int row_count_table2, 
							TABLEDATALIST * table2_start)
{	
	TABLEDATALIST 	* temp = join_table[0]->down->down,
					* temp2 = column->down->down,
					* temp3 = column2->down->down,
					* temp4 = table2_start->down->down,
					** rows,
					** selected_rows;
	
	CONDLIST 		* cond_temp = new CONDLIST();
	
	
	int 			count = 0,
					i = 0,
					r_count = row_count(join_table[0]);
					
	bool			has_selected[row_count_table2],
					selected = false;
	
	for(int j = 0; j < row_count_table2; j++)
		has_selected[j] = false;
	
	selected_rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * r_count + 1);
	
	for(int i = 0; i < r_count + 1; i++)
		*(selected_rows + i) = NULL;
	
	cond_temp->op = cond_op;				
		
	do
	{
		cond_temp->value = temp3->data;
		cond_temp->type = temp3->type;
		if(cond_temp->value != NULL && temp3->data != NULL)
			rows = condlist_selector(join_table, cond_temp, column, temp2);

		i++;
		
		if(*(rows + 0) != NULL)
		{
			*(selected_rows + count++) = *(rows + 0);
			*(rows + 0) = NULL;
			selected = true;
			has_selected[i % row_count_table2] = true;
		}
		
		if(i % row_count_table2 == 0 )
		{
			if(selected == false)
			{
				*(selected_rows + count++) = temp;
				make_row_data_null(temp4, temp);
			}
			selected = false;
		}	
		else if(i > ((row_count_table1 - 1) * row_count_table2))
		{
			if(has_selected[row_count_table2 - (row_count_table1 * row_count_table2 - i)] == false)
			{
				*(selected_rows + count++) = temp;
				make_row_data_null(temp, temp4);
			}
		}	
		
		temp4 = temp4->down;
		temp3 = temp3->down;
		temp2 = temp2->down;
		temp = temp->down;
		
	}
	while(temp != join_table[0]);
		
	return create_new_tabledatalist(join_table, selected_rows);
}



TABLEDATALIST ** left_join(TABLEDATALIST ** join_table, TABLEDATALIST * column, TABLEDATALIST * column2, string cond_op, int row_count_table2, 
							TABLEDATALIST * table2_start)
{	
	TABLEDATALIST 	* temp = join_table[0]->down->down,
					* temp2 = column->down->down,
					* temp3 = column2->down->down,
					* temp4 = table2_start->down->down,
					** rows,
					** selected_rows;
	
	CONDLIST 		* cond_temp = new CONDLIST();
	
	
	int 			count = 0,
					i = 0,
					r_count = row_count(join_table[0]);
					
	bool			has_selected = false;
	
	selected_rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * r_count + 1);
	
	for(int i = 0; i < r_count + 1; i++)
		*(selected_rows + i) = NULL;
	
	cond_temp->op = cond_op;				
		
	do
	{
		cond_temp->value = temp3->data;
		cond_temp->type = temp3->type;
		
		if(cond_temp->value != NULL && temp3->data != NULL)
			rows = condlist_selector(join_table, cond_temp, column, temp2);
		
		if(*(rows + 0) != NULL)
		{
			*(selected_rows + count++) = *(rows + 0);
			*(rows + 0) = NULL;
			has_selected = true;
		}
		
		i++;
		
		if(i % row_count_table2 == 0 )
		{
			if(has_selected == false)
			{
				*(selected_rows + count++) = temp;
				make_row_data_null(temp4, temp);
			}
			has_selected = false;
		}	
		
		temp4 = temp4->down;
		temp3 = temp3->down;
		temp2 = temp2->down;
		temp = temp->down;
		
	}
	while(temp != join_table[0]);
		
	return create_new_tabledatalist(join_table, selected_rows);
}


TABLEDATALIST ** inner_join(TABLEDATALIST ** join_table, TABLEDATALIST * column, TABLEDATALIST * column2, string cond_op)
{	
	
	TABLEDATALIST 	* temp = join_table[0]->down->down,
					* temp2 = column->down->down,
					* temp3 = column2->down->down,
					** rows,
					** selected_rows;
	
	CONDLIST 		* cond_temp = new CONDLIST();
	;
	
	int 			count = 0,
					r_count = row_count(join_table[0]);
	
	selected_rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * r_count + 1);
	
	for(int i = 0; i < r_count + 1; i++)
		*(selected_rows + i) = NULL;
	
		cond_temp->op = cond_op;					

		do
		{
			cond_temp->value = temp3->data;
			cond_temp->type = temp3->type;
			
			if(cond_temp->value == NULL && temp2->data == NULL)
			{
				 *(rows + count++) = find_row_head(join_table, temp2);	
			}
			else if(cond_temp->value != NULL && temp2->data != NULL)
				rows = condlist_selector(join_table, cond_temp, column, temp2);

			if(*(rows + 0) != NULL)
			{
				*(selected_rows + count++) = *(rows + 0);
				*(rows + 0) = NULL;
			}
			temp3 = temp3->down;
			temp2 = temp2->down;
			temp = temp->down;

		}
		while(temp != join_table[0]);
		
		//print_tabledatalist(create_new_tabledatalist(join_table, selected_rows));

	return create_new_tabledatalist(join_table, selected_rows);
}

TABLEDATALIST ** join(TABLEDATALIST ** table, TABLEDATALIST ** table2)
{

	TABLEDATALIST 	** join = get_table_header(table),
					** temp_table2_header = get_table_header(table2),
					* temp = join[0]->down,
					* temp2,
					* temp3,
					* temp_table = table[0]->down->down,
					* temp_table2 = table2[0]->down->down;
	
	connect_two_rows_horizontal(join[0], temp_table2_header[0]);	
	connect_two_rows_horizontal(join[1], temp_table2_header[1]);
	//return NULL;
	temp = join[1];
	if(row_count(table[0]) != 0 && row_count(table2[0]) != 0)
	{
		do
		{	
			do
			{
				temp2 = copy_row(temp_table);
				temp3 = copy_row(temp_table2);
				connect_two_rows_horizontal(temp2, temp3);
				connect_two_rows_vertical(temp, temp2);
				temp_table2 = temp_table2->down;	
				temp = temp->down;
				//free(temp->up);
			}
			while(temp_table2 != table2[0]);	
			temp_table  = temp_table->down;
			temp_table2  = table2[0]->down->down;
			
		}
		while(temp_table != table[0]);
	}
	
	connect_two_rows_vertical(temp, join[0]);
	temp_table2_header[0]->prev->next = NULL;
	return join;
}

TABLEDATALIST * get_other_table(TABLEDATALIST ** table)
{
	TABLEDATALIST * temp = table[0];
	
	while(temp->next != NULL)
		temp = temp->next;
	
	
	temp->next = temp->down->next->up;
	temp = temp->next;
	temp->prev = temp->down->prev->up;
	
	return temp;	
}