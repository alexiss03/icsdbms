/*
	Performs the set operator difference
*/
TABLEDATALIST ** difference_set_operator(TABLEDATALIST ** table1, TABLEDATALIST ** table2)
{
	TABLEDATALIST 	** new_table = NULL,
					* temp,
					* temp2;
	
	if(no_of_columns(table1) == 0)
		return NULL;
	else
	{
		//Checks if table1 has the same column from table2
		if(no_of_columns(table1) == no_of_columns(table2))
		{
			new_table = get_table_header(table1);
			temp = table1[0]->down->down;
			temp2 = new_table[1];
			do
			{
				//Identifies if the row is unique
				if(is_row_unique_null(table2, temp, table2[0]->up))
				{
					connect_two_rows_vertical(temp2, copy_row(temp));
					temp2 = temp2->down;
				}
				temp = temp->down;
			}while(temp != table1[0]);
			
			connect_two_rows_vertical(temp2, new_table[0]);
			
			return new_table;
		}
		else
		{
			//error("The used SELECT statements have a different number of columns");
		}
	}
}
