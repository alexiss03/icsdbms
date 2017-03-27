/*
	Performs the max aggregate operator
*/
void * max_aggregate_operator(TABLEDATALIST ** table, TABLEDATALIST * column, TABLEDATALIST * group_grouping)
{	
	TABLEDATALIST 	* temp,
					* temp2,
					* start_row,
					* first_null_row,
					* end_row;
				
	if(group_grouping == NULL)
	{
		if(get_string(column->down->data).compare("int") == 0 || get_string(column->down->data).compare("float") == 0)
		{

			append_new_column(table, "max(" + get_string(column->data) + ")", get_string(column->down->data));	
			max_op(table, column->down->down, column->up, table[0]->prev);
			first_null_row = find_first_null_row(table[0]->prev);
			first_null_row->down = table[0]->prev;
			table[0]->prev->up = first_null_row;
		}
	}
	else
	{	
		temp = column->down->down;
		temp2 = group_grouping->down->down;
		append_new_column(table, "max(" + get_string(column->data) + ")", get_string(column->down->data));	
		
		if(temp->type.compare("int") == 0 || temp->type.compare("float") == 0)
		{
			do
			{
				start_row = temp2;
				end_row = find_last_equal_row(table, temp2, group_grouping);
				temp2 = end_row->down;
				max_op(table, get_cell(table, find_row_head(table, start_row), "", column),
					get_cell(table, find_row_head(table, end_row), "", column), table[0]->prev);	
			}
			while(temp2 != group_grouping);
			
			first_null_row = find_first_null_row(table[0]->prev);
			first_null_row->down = table[0]->prev;
			table[0]->prev->up = first_null_row;	
		}
		else
		{
			//error: error("Sum cannot be performed on non-int and non-float data");
		}
	}
}

/*
	Finds the actual max given the range of rows
*/
void * max_op(TABLEDATALIST ** table, TABLEDATALIST * start, TABLEDATALIST * end, TABLEDATALIST * column)
{	
	int 			int_max = 0;
	float 			float_max = 0;
	
	TABLEDATALIST 	* temp = start,
					* temp2,
					* current_row;
	bool			has_null = false;
	
	if(start->type.compare("int") == 0)
	{
		int_max = get_int(temp->data);
		temp = temp->down;
	} 
	else if(start->type.compare("float") == 0)
	{
		float_max = get_float(temp->data);
		temp = temp->down;
	}	

	while(temp != end->down)
	{
		if(temp->data != NULL)
		{
			if(start->type.compare("int") == 0)
			{
				if(get_int(temp->data) > int_max)
					int_max = get_int(temp->data);
			} 
			else if(start->type.compare("float") == 0)
			{
				if(get_float(temp->data) > float_max)
					float_max = get_float(temp->data);
			}		
		}
		temp = temp->down;
	}
	
	temp = start;
	
	if(start->type.compare("int") == 0)
	{
		while(temp != end->down->down)
		{
			if(temp == start)
			{
				current_row = find_first_null_row(column);
				create_new_cell(new int(int_max), "int", current_row->next->down, current_row->prev->down, current_row, NULL);
			}
			else
				if(temp->up != start)
					remove_row(temp->up);
				
			temp = temp->down;
		}
	}
	else if(start->type.compare("float") == 0)
	{
		while(temp != end->down->down)
		{
			if(temp == start)
			{
				current_row = find_first_null_row(column);
				create_new_cell(new float(float_max), "float", current_row->next->down, current_row->prev->down, current_row, NULL);
			}
			else
				if(temp->up != start)
					remove_row(temp->up);
				
			temp = temp->down;
		}
	}
}