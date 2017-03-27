/*
	Rearranges the rows according to 'column'
*/
TABLEDATALIST ** group_by(TABLEDATALIST ** table, DATA * column, DATA * columnlist, CONDLIST * condition)
{		
	TABLEDATALIST 	* col,
					* temp,
					* temp2;

	DATA			* temp3 = columnlist;

	if(column != NULL)
	{
		col = find_column(table, get_string(column->data));
		temp = col->down->down;
		while(temp != col)
		{
			temp2 = temp->down;
			do
			{
				if(is_equal(temp, temp2) && temp != temp2)
				{
					swap_rows(temp->down, temp2);
					temp = temp->down;
				}	
				temp2 = temp2->down;
			}
			while(temp2 != col);
			
			temp = temp->down;	
		}
	}	
	return table;
}


/*
	Calls at the aggregates at the same time trims the unnecessary rows
*/
TABLEDATALIST ** group_by_operator(TABLEDATALIST ** table, CONDLIST * condition, DATA * column, DATA * columnlist)
{
	TABLEDATALIST 	//* col = find_column(table, get_string(column->data)),
					* temp,
					* col,
					** rows;

	DATA			* temp3 = columnlist;
	
	while(temp3 != NULL)
	{
		if(temp3->down != NULL)
		{
			if(get_string(temp3->down->data).compare("sum") == 0)
			{
				if(column == NULL)
					sum_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					sum_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				// cout << "max";
				temp3->data = new string( "sum(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
				//temp3->down = temp3->down->down;
			}
			else if(get_string(temp3->down->data).compare("count") == 0)
			{
				if(column == NULL)
					count_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					count_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				temp3->data = new string( "count(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
				
			}
			else if(get_string(temp3->down->data).compare("first") == 0)
			{
				
				if(column == NULL)
					first_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					first_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				temp3->data = new string( "first(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
			}
			else if(get_string(temp3->down->data).compare("last") == 0)
			{
				if(column == NULL)
					last_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					last_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				temp3->data = new string( "last(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
			}
			else if(get_string(temp3->down->data).compare("min") == 0)
			{
				if(column == NULL)
					min_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					min_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				temp3->data = new string( "min(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
			}
			else if(get_string(temp3->down->data).compare("avg") == 0)
			{
				if(column == NULL)
					avg_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					avg_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));
				temp3->data = new string( "avg(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
			}
			else if(get_string(temp3->down->data).compare("max") == 0)
			{
				if(column == NULL)
					max_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), NULL);	
				else					
					max_aggregate_operator(table, find_column(table, get_string(temp3->data), NULL), find_column(table, get_string(column->data), NULL));

				temp3->data = new string( "max(" + get_string(temp3->data) + ")");
				temp3->down = temp3->down->down;
			}
		}
		temp3 = temp3->next;
	}
	//return NULL;

	if(column != NULL)
	{
		col = find_column(table, get_string(column->data));
		temp = col;
		if(condition == NULL)	
		{
			temp = temp->down->down;
			do
			{
				if(is_equal(temp, temp->down))
				{
					temp = temp->down;
					delete_row(temp->up);
				}
				else
					temp = temp->down;
			}while(col != temp);
			return NULL;
		}
		else
		{
			//cout << "elsse";
			temp = temp->down->down;
			do
			{
				if(is_equal(temp, temp->down))
				{
					temp = temp->down;
					delete_row(temp->up);
				}
				else
					temp = temp->down;
			}while(col != temp);

			rows = condlist_selector(table, condition);
			return rows;
		}
	}	
}