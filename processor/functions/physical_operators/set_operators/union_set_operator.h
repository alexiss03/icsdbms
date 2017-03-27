/*
	Performs the union set operator
*/
TABLEDATALIST ** union_set_operator(TABLEDATALIST ** table1, TABLEDATALIST ** table2)
{
	TABLEDATALIST 	** new_table = NULL,
					* temp,
					* temp2;
	
	if(no_of_columns(table1) == 0)
		return NULL;
	else
	{
		//Checks if number of columns in table1 is equal to the number of column2 in table2
		if(no_of_columns(table1) == no_of_columns(table2))
		{
			new_table = get_table_header(table1);
			temp = table1[0]->down->down;
			temp2 = new_table[1];
			do
			{
				if(is_row_unique_null(new_table, temp))
				{
					connect_two_rows_vertical(temp2, copy_row(temp));
					temp2 = temp2->down;
				}
				temp = temp->down;
			}while(temp != table1[0]);
			
			temp = table2[0]->down->down;
			
			do
			{
				//cout << "alll";
				if(is_row_unique_null(new_table, temp))
				{
					connect_two_rows_vertical(temp2, copy_row(temp));
					temp2 = temp2->down;
				}
				temp = temp->down;
			}while(temp != table2[0]);
			
			connect_two_rows_vertical(temp2, new_table[0]);
			
			return new_table;
		}
		else
		{
			//error("The used SELECT statements have a different number of columns");
		}
	}
}


/*
	Connects rows from table1 and table2 vertically
*/
TABLEDATALIST ** union_all_set_operator(TABLEDATALIST ** table1, TABLEDATALIST ** table2)
{
	TABLEDATALIST 	** new_table = NULL,
					* temp,
					* temp2;
	
	if(no_of_columns(table1) == 0)
		return NULL;
	else
	{
		if(no_of_columns(table1) == no_of_columns(table2))
		{
			new_table = get_table_header(table1);
			temp = table1[0]->down->down;
			temp2 = new_table[1];
			do
			{
				connect_two_rows_vertical(temp2, copy_row(temp));
				temp2 = temp2->down;
				temp = temp->down;
			}while(temp != table1[0]);
			
			temp = table2[0]->down->down;
			do
			{
				connect_two_rows_vertical(temp2, copy_row(temp));
				temp2 = temp2->down;
				temp = temp->down;
			}while(temp != table2[0]);
			
			connect_two_rows_vertical(temp2, new_table[0]);
			
			return new_table;
		}
		else
		{
			//error("The used SELECT statements have a different number of columns");
		}
	}
}