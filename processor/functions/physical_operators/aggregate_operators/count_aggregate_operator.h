/*
	Performs the count aggregate operator
*/
void * count_aggregate_operator(TABLEDATALIST ** table, TABLEDATALIST * column, TABLEDATALIST * group_grouping)
{
	int 			int_sum = 0;
	float 			float_sum = 0;
	
	TABLEDATALIST 	* temp,
					* temp2,
					* start_row,
					* first_null_row,
					* end_row;
					
	if(group_grouping == NULL)
	{
		append_new_column(table, "count(" + get_string(column->data) + ")", get_string(column->down->data));	
		count_op(table, column->down->down, column->up, table[0]->prev);
		first_null_row = find_first_null_row(table[0]->prev);
		first_null_row->down = table[0]->prev;
		table[0]->prev->up = first_null_row;
	}
	else
	{
	
		temp = column->down->down;
		temp2 = group_grouping->down->down;
		append_new_column(table, "count(" + get_string(column->data) + ")", get_string(column->down->data));	
		
		do
		{
			start_row = temp2;
			end_row = find_last_equal_row(table, temp2, group_grouping);
			temp2 = end_row->down;
			count_op(table, get_cell(table, find_row_head(table, start_row), "", column),
				get_cell(table, find_row_head(table, end_row), "", column), table[0]->prev);	
		}
		while(temp2 != group_grouping);
		
		first_null_row = find_first_null_row(table[0]->prev);
		first_null_row->down = table[0]->prev;
		table[0]->prev->up = first_null_row;	
	}
}

/*
	Performs the actual counting of rows given the range of rows
*/
void * count_op(TABLEDATALIST ** table, TABLEDATALIST * start, TABLEDATALIST * end, TABLEDATALIST * column)
{	
	int 			count = 0;
	
	TABLEDATALIST 	* temp = start,
					* temp2,
					* current_row;
				
	while(temp != end->down)
	{
		count++;
		temp = temp->down;
	}
	
	temp = start;

	while(temp != end->down->down)
	{
		if(temp == start)
		{
			current_row = find_first_null_row(column);
			create_new_cell(new int(count), "int", current_row->next->down, current_row->prev->down, current_row, NULL);
		}
		else
			if(temp->up != start)
				remove_row(temp->up);
			
		temp = temp->down;
	}
	
}