int num_of_null(TABLEDATALIST * column)
{
	TABLEDATALIST * temp = column;

	int count = 0;

	do
	{
		if(temp->data == NULL) count++;
		temp = temp->down;
	}
	while(temp != column);

	return count;
}

bool remove_column(TABLEDATALIST ** table, TABLEDATALIST * column)
{
	TABLEDATALIST 		* temp = table[0],
						* temp1 = column,
						* temp2;

	temp1->up->down = NULL;

	do
	{
		temp1->prev->next = temp1->next;
		temp1->next->prev = temp1->prev;
		temp1 = temp1->down;
		if(temp1 != NULL)
			free(temp1->up);
		
	}
	while(temp1 != NULL);
}

TABLEDATALIST * find_column_two_tables(TABLEDATALIST ** table, string name, string column_name)
{
	TABLEDATALIST 	* temp = table[0],
					* temp2;
	do
	{
		if(temp->constraint.compare(name) == 0)
		{
			temp2 = temp;
			do
			{
				if(get_string(temp2->data).compare(column_name) == 0)
					return temp2;

				temp2 = temp2->next;
			}
			while(temp2 != temp);
		}
		temp = temp->next;
	}
	while(temp != table[0]);

	return NULL;
}

bool append_two_columns(TABLEDATALIST * column, TABLEDATALIST * column2)
{
	TABLEDATALIST 	* temp = column,
					* temp2 = column2;

	do
	{
		temp->next = temp2;
		temp2->prev = temp;
		temp = temp->down;
		temp2 = temp2->down;
	}
	while(temp != column);
}

TABLEDATALIST ** select_all_distinct_rows(TABLEDATALIST ** table, TABLEDATALIST * column)
{
	TABLEDATALIST 	* temp = column->down->down,
					* temp2 = column->down->down,
					** rows = NULL;

	int				count = 0;
	bool 			is_distinct =false;

	rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * row_count(table[0]) + 1);
	
	do
	{

		is_distinct = false;
		do
		{
			if(is_equal(temp, temp2) && temp != temp2)
			{
				is_distinct = false;
				break;
			}
			else if(is_equal(temp, temp2))
			{
				is_distinct = true;
				break;
			}
			temp2 = temp2->down;
		}
		while(temp2 != column);

		if(is_distinct)
		{
			*(rows + count++) = temp;
		}

		temp = temp->down;
		temp2 = column;
	}
	while(temp != column);

	return rows;

}

TABLEDATALIST ** select_top_rows(TABLEDATALIST ** table, int top_rows)
{
	TABLEDATALIST 	** rows = NULL,
					* temp = table[0]->down->down;
	
	int				count = 0;

	rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * row_count(table[0]) + 1);
	
	for(int i=0; i < row_count(table[0]) + 1; i++)
		rows[i] = NULL;
	if(top_rows != 0)
	{
		do
		{
			*(rows + count++) = temp;
			temp = temp->down;
			if(count == top_rows) break;
		}while(temp != table[0]);
	}

	return rows;	
}

bool append_column(TABLEDATALIST ** table, TABLEDATALIST * column)
{
	TABLEDATALIST 	* before = table[0],
					* after = table[0],
					* temp2 = column,
					* temp3 = NULL;

	while(before->next != NULL)
		before = before->next;
	//cout << convert_to_string(before);
	//return false;

	do
	{
		before->next = temp2;
		temp2->prev = before;

		before = before->down;
		temp2 = temp2->down;
		
	}while(temp2 != column);
	//print_tabledatalist(table);
	return true;
}


TABLEDATALIST * copy_column(TABLEDATALIST * column)
{
	TABLEDATALIST 	* temp = column,
					* temp2 = NULL,
					* temp3;
	// if(column == NULL) cout << "NULL";
	// return NULL;
	// cout << convert_to_string(column);
	do{
		if(temp2 == NULL)
		{
			temp2 = new TABLEDATALIST(temp);
			temp3 = temp2;
		}
		else
		{
			temp2->down = new TABLEDATALIST(temp);
			temp2->down->up = temp2;
			temp2 = temp2->down;
		}

		temp = temp->down;
	}
	while(temp != column);

	temp3->up = temp2;
	temp2->down = temp3;
	/*print_column(column);
	cout << "temp2";
	print_column(temp3);*/

	return temp3;

}

bool is_column_repeated(DATA * columnlist, string column)
{
	bool boolTemp;
	DATA * temp = columnlist;
	boolTemp = false;

	while(temp != NULL)
	{
		//cout << get_string(temp->data) << column << endl;
		if(get_string(temp->data).compare(column) == 0)
		{
			if(!boolTemp)
				boolTemp = true;
			else if(boolTemp)
				return true;
		}
		temp = temp->next;
	}
	//while(temp != table[0]);
	return false;
}


bool delete_column(TABLEDATALIST ** table, string column_name)
{
	TABLEDATALIST * column = get_column(table[0], column_name),
				  * temp = column;

	temp->up->down = NULL;
	//cout << convert_to_string(temp);

	while(temp != NULL)
	{
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		temp = temp->down;
		//free(temp->up);
	}

	return true;
	
}

void * copy_data( void * data, string type1, string type2)
{
	if(type1.compare(type2) == 0)
	{
		if(type1.compare("int") == 0)
		{
			return new int(get_int(data));
		}
		else if(type1.compare("float") == 0)
		{
			return new float(get_float(data));
		}
		else if(type1.compare("bool") == 0)
		{
			return new bool(get_bool(data));		
		}
		else if(type1.compare("varchar") == 0)
		{
			return new string(get_string(data));
		}
		else if(type1.compare("date") == 0)
		{
			return new DATE(get_date(data));
		}
		else if(type1.compare("") == 0)
		{
			return NULL;
		}
	}
	else
	{
		if(type1.compare("float") == 0 && type2.compare("int") == 0)
			return new float((float)get_int(data));	
	}
}

TABLEDATALIST ** delete_row(TABLEDATALIST * row)
{
	TABLEDATALIST 	** rows = NULL,
					* temp = row;
	
	int				count = 0;

	do
	{
		temp->up->down = temp->down;
		temp->down->up = temp->up;
		temp = temp->next;
		if(temp->prev != row)
			free(temp->prev);	
	}while(temp != row);
	free(temp);
	
}

TABLEDATALIST ** select_all_rows(TABLEDATALIST ** table)
{
	TABLEDATALIST 	** rows = NULL,
					* temp = table[0]->down->down;
	
	int				count = 0;
	if(row_count(table[0]) != 0)
	{
		rows = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * row_count(table[0]) + 1);
		
		for(int i=0; i < row_count(table[0]) + 1; i++)
			rows[i] = NULL;
		
		do
		{
			*(rows + count++) = temp;
			temp = temp->down;
		}while(temp != table[0]);
	}
	return rows;
}

bool is_row_unique_null(TABLEDATALIST ** table, TABLEDATALIST * row, TABLEDATALIST * end = NULL)
{
	TABLEDATALIST 	* temp,
					* temp2 = table[0],
					* temp3;
					
	do
	{
		temp = row;
		temp3 = temp2;
		do
		{
			if(!is_equal(temp, temp3))
			{	
				temp = NULL;
				break;
			}
			temp = temp->next;
			temp3 = temp3->next;
		}
		while(temp != row);
		
		if(temp != NULL || temp == row)
			return false;
			
		temp2 = temp2->down;
	}while(temp2 != end);
	
	return true;
}

int no_of_columns(TABLEDATALIST ** table)
{
	TABLEDATALIST 	* temp = table[0];
	int 			count = 0;
	
	if(table == NULL)
		return 0;
	else
	{
		do
		{
			count++;
			temp = temp->next;
		}
		while(temp != table[0]);
	}
	
	return count;
}

TABLEDATALIST * find_row_head(TABLEDATALIST ** table, TABLEDATALIST * cell)
{
	TABLEDATALIST * temp1 = table[0], * temp2 = cell;
	//cout << "Table head" << *(static_cast<string *>(table[0]->data));
	do
		{
		do
		{
			if(temp1 == temp2)
				return temp1;
				
			temp2 = temp2->next;
			
		}while(temp2 != cell);
		
		temp1 = temp1->down;
		//cout << "Row";
	}while(temp1 != table[0]);
	return NULL;
}

TABLEDATALIST * find_first_null_row(TABLEDATALIST * column)
{
	TABLEDATALIST *	temp = column;
	
	if(column == NULL) return NULL;
	
	while(temp->down != NULL)
	{
		temp = temp->down;
	}
	
	return temp;
}

bool create_new_cell(void * data, string type, TABLEDATALIST * next, TABLEDATALIST * prev, TABLEDATALIST * up, TABLEDATALIST * down)
{
	TABLEDATALIST * temp = new TABLEDATALIST(data, type, "", next, prev, up, down);
	
	if(temp->up != NULL)
		temp->up->down = temp;
	if(temp->prev != NULL)
		temp->prev->next = temp;
	if(temp->next != NULL)
		temp->next->prev = temp;
	
	return true;
}

bool remove_row(TABLEDATALIST * row)
{
	TABLEDATALIST * temp = row;
	
	temp->prev->next = NULL;
	
	
	do
	{
		temp->up->down = temp->down;
		temp->down->up = temp->up;
		temp = temp->next;
		free(temp->prev);
	}
	while(temp->next != NULL);
	
	temp->up->down = temp->down;
	temp->down->up = temp->up;
	free(temp);
	
	return true;
	
}

bool append_new_column(TABLEDATALIST ** table, string column_name, string type)
{
	TABLEDATALIST * temp = table[0]->prev;
	
	temp->next = new TABLEDATALIST(new string(column_name), "varchar");
	temp->next->next = table[0];
	temp->next->prev = temp; 
	table[0]->prev = temp->next;
	
	temp = temp->next;
	temp->down = new TABLEDATALIST(new string(type), "varchar");
	temp->down->up = temp;
	temp->down->prev = temp->prev->down;
	temp->prev->down->next = temp->down;
	temp->down->next = table[1];
	table[1]->prev = temp->down;
	temp->down->down = NULL;
	
	return true;
}

TABLEDATALIST * find_last_equal_row(TABLEDATALIST ** table, TABLEDATALIST * start_row, TABLEDATALIST * first_row)
{
	TABLEDATALIST * temp = start_row;
	do
	{
		if(is_equal(start_row, temp))
			temp = temp->down;
		else
			return temp->up;
	}
	while(temp != first_row);
	
	return temp->up;
}

bool swap_rows(TABLEDATALIST * row1, TABLEDATALIST * row2)
{
	TABLEDATALIST 	* temp = row1, 
					* temp2 = row2,
					* temp3,
					* temp4,
					* up,
					* down,
					* next,
					* prev;
					
	do
	{
			temp3 = temp->next;	
			temp4 = temp2->next;
		if(row1 == row2)
			return true;
		else if(temp->down != temp2)
		{
		
			up = temp->up;
			down = temp->down;

			
			up->down = temp2;
			down->up = temp2;
			
			temp->up = temp2->up;
			temp->down = temp2->down;
			
			
			temp2->up->down = temp;
			temp2->down->up = temp;
			
			temp2->up = up;
			temp2->down = down;
			
		}
		else
		{
			temp->up->down = temp2;
			temp2->down->up = temp;
			temp2->up = temp->up;
			temp->down = temp2->down;
			temp2->down = temp;
			temp->up = temp2;
			
		}
			temp = temp3;	
			temp2 = temp4;
	}
	while(temp != row1);
}

TABLEDATALIST * make_row_data_null(TABLEDATALIST * row, TABLEDATALIST * end = NULL)
{
	TABLEDATALIST * temp = row;
	
	if(end == NULL)	
	{
		do
		{
			temp->type = "";
			free(temp->data);
			temp->data = NULL;
			temp = temp->next;
		}while(temp != row);
	}
	else
	{
		do
		{
			temp->type = "";
			free(temp->data);
			temp->data = NULL;
			temp = temp->next;
		}
		while(temp != end);		
	}
	return row;
}

void connect_two_rows_horizontal(TABLEDATALIST * row, TABLEDATALIST * row2)
{
	TABLEDATALIST 	* temp,
					* temp2;
	
	temp = find_row_end(row);
	temp->next = row2;
	temp2 = find_row_end(row2);
	temp2->next = row;
	row2->prev = temp;
	row->prev = temp2;
}

void connect_two_rows_vertical(TABLEDATALIST * row, TABLEDATALIST * row2)
{
	TABLEDATALIST * temp = row,
				  * temp2 = row2;
	do
	{
		temp->down = temp2;
		temp2->up = temp;
		temp = temp->next;
		temp2 = temp2->next;
	}
	while(temp != row && temp2 != row2 && temp != NULL && temp2 != NULL);
	
}

TABLEDATALIST * find_row_end(TABLEDATALIST * row)
{
	return row->prev;
}

TABLEDATALIST * copy_row(TABLEDATALIST * row)
{
	TABLEDATALIST 	* temp = row,
					* tail = NULL,
					* head = NULL;
	do
	{
		if(head == NULL)
		{
			head = new TABLEDATALIST(temp);
			tail = head;
		}
		else
		{
			tail->next = new TABLEDATALIST(temp);
			tail->next->prev = tail;
			tail = tail->next;
		}
		temp = temp->next;
	}
	while(temp != row);
	
	tail->next = head;
	head->prev = tail;
	
	return head;
}

TABLEDATALIST * find_column(TABLEDATALIST ** table, string column, TABLEDATALIST * end = NULL)
{
	TABLEDATALIST * temp = table[0];
	
	if(end == NULL)	
	{
		do
		{
			if(column.compare(get_string(temp->data)) == 0)
				return temp;
			temp = temp->next;
		}while(temp != table[0]);
	}
	else
	{
		do
		{
			if(column.compare(get_string(temp->data)) == 0)
				return temp;
			temp = temp->next;
		}while(temp != end);		
	}
	
	return NULL;
}


bool remove_column(TABLEDATALIST ** table, DATA * columnlist)
{
	TABLEDATALIST 		* temp = table[0],
						* temp1,
						* temp2;
	
	DATA 				* dataTemp = columnlist;
	
	string 				stringTemp;
	do
	{
		do{
			stringTemp = get_string(dataTemp->data);
			if(get_string(temp->data).compare(stringTemp) == 0)
			{
				temp1 = temp;
				temp1->up->down = NULL;
				do
				{
					if(temp1 == table[0])
						table[0] = temp1->next;
					if(temp1 == table[1])
						table[1] = temp1->next;
					if(temp1->next == temp1)
					{
						table[0] = NULL;
					}
					temp1->prev->next = temp1->next;
					temp1->next->prev = temp1->prev;
					temp2 = temp1;
					temp1 = temp1->down;
					free(temp2);
				}while(temp1 != NULL);
				break;
			}
			temp = temp->next;
		}while(temp != table[0]);
		temp = table[0];
		dataTemp = dataTemp->next;
	}while(dataTemp != NULL);
}


 TABLEDATALIST ** get_table_header(TABLEDATALIST ** table)
 {

	if(table == NULL)
		return NULL;
	
	TABLEDATALIST 	* temp1 = table[0], 
					* temp2 = NULL, 
					** newtable, 
					** newtable2,
					* temp4 = NULL, 
					* temp5 = NULL, 
					* temp6, 
					* previousRow = NULL,
					* previousColumn = NULL,
					* oldrow = NULL,
					* temp7 = NULL,
					* temp8 = NULL;
	DATA 			* dataTemp;
	bool			boolTemp;
	
	newtable = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *)*2);
	newtable[0] = copy_row(table[0]);
	newtable[1] = copy_row(table[1]);
	connect_two_rows_vertical(newtable[0],newtable[1]);
	connect_two_rows_vertical(newtable[1],newtable[0]);
	/*newtable[0] = new TABLEDATALIST(table[0]);
	newtable[1] = new TABLEDATALIST(table[1]);
	newtable[0]->down = newtable[1];
	newtable[1]->up = newtable[0];
	
	
	//copying the first two rows : the column name and column data type
	for(int i=0; i < 2; i++)
	{
		temp6 = temp4 = temp5 = newtable[i];
		temp2 = temp1->next;
		do
		{
			temp4 = new TABLEDATALIST(temp2);
			temp4->prev = temp5;
			temp5->next = temp4;
			temp5 = temp4;
			temp2 = temp2->next;
			if(temp2 == temp1)
			{
				temp4->next = temp6;
				temp6->prev = temp4;
			}
		}while(temp2 != temp1);

		temp1 = table[i+1];
	}
	
	temp1 = temp2 = newtable[0];
	temp4 = newtable[1];
	
	do
	{
		temp1->up = NULL;
		temp1->down = temp4;
		temp4->up = temp1;
		temp4->down = NULL;
		temp1 = temp1->next;
		temp4 = temp4->next;
	}while(temp1 != temp2);
	*/
	
	return newtable;
	
	//end of copying of the first two rows: the column name and column data type
 }
 
 TABLEDATALIST ** create_new_tabledatalist(TABLEDATALIST ** table, TABLEDATALIST ** rows, DATA * columnlist = NULL)
 {

	if(table == NULL)
		return NULL;
	
	TABLEDATALIST 	* temp1 = table[0], 
					* temp2 = NULL, 
					** temp3 = rows, 
					** newtable,
					** newtable2, 
					* temp4 = NULL, 
					* temp5 = NULL, 
					* temp6, 
					* previousRow = NULL,
					* previousColumn = NULL,
					* oldrow = NULL,
					* temp7 = NULL,
					* temp8 = NULL;
	DATA 			* dataTemp,
					* columnlistTemp = columnlist;
	bool			boolTemp,
					is_repeated = false;
	
	// if(rows == NULL)
	// {
	// 	newtable = get_table_header(table);
	// 	connect_two_rows_horizontal(newtable[1], newtable[0]);
	// 	return newtable;
	// }

	newtable = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * 2);
	newtable2 = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * 2);
	newtable[0] = new TABLEDATALIST(table[0]);
	newtable[1] = new TABLEDATALIST(table[1]);
	newtable[0]->down = newtable[1];
	newtable[1]->up = newtable[0];
	newtable[0]->prev = newtable[0];
	newtable[0]->next= newtable[0];
	newtable[0]->up= newtable[1];
	newtable[1]->prev = newtable[1];
	newtable[1]->next= newtable[1];
	newtable[1]->up= newtable[0];

	//copying the first two rows : the column name and column data type
	if(no_of_columns(table) != 1)
	{
		for(int i=0; i < 2; i++)
		{
			temp6 = temp4 = temp5 = newtable[i];
			temp2 = temp1->next;
			do
			{
				temp4 = new TABLEDATALIST(temp2);
				temp4->prev = temp5;
				temp5->next = temp4;
				temp5 = temp4;
				temp2 = temp2->next;

				if(temp2 == temp1)
				{
					temp4->next = temp6;
					temp6->prev = temp4;
				}
			}while(temp2 != temp1);

			temp1 = table[i+1];
		}
		
		temp1 = temp2 = newtable[0];
		temp4 = newtable[1];
		
		if(newtable[0]->next != newtable[0])
		{
			do
			{
				temp1->down = temp4;
				temp4->up = temp1;
				temp1 = temp1->next;
				temp4 = temp4->next;
			}while(temp1 != temp2);
		}
	}
	connect_two_rows_vertical(newtable[1], newtable[0]);

	if((rows == NULL || rows[0] == NULL) && columnlist == NULL)
	{
		return newtable;
	}
	//print_tabledatalist(table);
	//end of copying of the first two rows: the column name and column data type

	//temp4, temp5, temp6 are available temporary variables
	previousRow = newtable[0];
	
	//copying of the rows listed
	if(rows != NULL)
	{
		for(int i=0; *(rows + i) != NULL; i++)
		{
			previousRow = previousRow->down;
			previousRow->down = new TABLEDATALIST(*(rows + i ));
			temp4 = previousRow->down;
			temp6 = temp5 = (*(rows + i))->next;
			
			while(temp6 != temp5->next){
				temp4->next = new TABLEDATALIST(temp5);
				temp4->next->prev = temp4;
				temp5 = temp5->next;
				temp4 = temp4->next;
			}

			temp4->next = previousRow->down;
			previousRow->down->prev = temp4;
			
			temp7 = previousRow;
			//temp6 = temp6->prev;
			temp6 = previousRow->down;
			
			do
			{
				temp7->down = temp6;
				temp6->up = temp7;
				temp7 = temp7->next;
				temp6 = temp6->next;
			}while(temp7 != previousRow);
			
			//circular_test(table);
		}
		
		temp5 = previousRow->down;
		temp6 = temp4 = newtable[0];	
		do
		{
			temp4->up = temp5;
			temp5->down = temp4;
			temp4 = temp4->next;
			temp5 = temp5->next;
		}while(temp4 != temp6);
	}
	//end of copying of rows listed
	//cout << "test";
	//print_tabledatalist(newtable);
	
	temp6 = temp5 = temp7 = newtable[0];
	//circular_test(newtable); //: PASS : CORRECT DATA
	
	if(columnlist == NULL || (*(static_cast<string *>(columnlist->data))).compare("*") == 0)
	{	
		//return NULL;
		return newtable;
	}
	else
	{
		TABLEDATALIST * column;
		newtable2[0] = NULL;
		
		for(previousColumn = NULL ; columnlist != NULL; columnlist = columnlist->next)
		{
			if(newtable2[0] == NULL)
			{
				//if(columnlist->data2 != NULL) cout << "tablename working";
				if(columnlist->data2 != NULL && newtable[0]->constraint.compare("") != 0) 
					column = copy_column(find_column_two_tables(newtable,get_string(columnlist->data2),  get_string(columnlist->data)));
				else
					column = copy_column(find_column(newtable, get_string(columnlist->data)));
				
				newtable2[0] = column;
				newtable2[1] = newtable2[0]->down;
			}
			else
			{
				if(columnlist->data2 != NULL && newtable[0]->constraint.compare("") != 0) 
					column = copy_column(find_column_two_tables(newtable,get_string(columnlist->data2), get_string(columnlist->data)));
				else
					column = copy_column(find_column(newtable, get_string(columnlist->data)));
				column = copy_column(find_column(newtable, get_string(columnlist->data)));
				append_column(newtable2, column);
			}
		}
		append_two_columns(column, newtable2[0]);
	}
	return newtable2;
 }
 
 TABLEDATALIST ** get_table(string tablename)
 {
 	int i = 0;

 	string name = tablename;

	for(i = 0; table_pool[i] != NULL; i++)
	{
		// /cout << table_pool[i]->tablename;
		if(table_pool[i]->tablename.compare(tablename) == 0)
		{
			return table_pool[i]->table;
		}
	}
	
	if(check_exist(name.c_str()) != 0)
	{
		table_pool[i] = new TABLE(read_file_start(tablename.c_str()), tablename);

		return table_pool[i]->table;
	}
	return NULL;
 }
 
 TABLEDATALIST *  get_primary_key(TABLEDATALIST ** table)
 {
	TABLEDATALIST * temp = table[1];
	

	do
	{
		if(temp->constraint.compare("PRIMARY") == 0)
			return temp->up;
		temp = temp->next;
	}
	while(temp != table[1]);
	
	return NULL;
 }
 
 TABLEDATALIST * get_column(TABLEDATALIST * table, string column_name)
 {
	TABLEDATALIST * temp = table;
	
	do
	{
		if(get_string(temp->data).compare(column_name) == 0)
			return temp;
		temp = temp->next;
	}
	while(temp != table);
	
	return NULL;
 
 }
 
 TABLE * get_table_from_table_pool(string table_name)
 {

	for(int i = 0; (*(table_pool + i)) != NULL; i++)
	{
		if((*(table_pool + i))->tablename.compare(table_name) == 0)
			return *(table_pool + i);
	}
	return NULL;
 }
 
 TABLEDATALIST * get_cell(TABLEDATALIST ** table, TABLEDATALIST * row, string column_name, TABLEDATALIST * column_search = NULL)
 {
	TABLEDATALIST 	* temp = table[0],
					* temp2 = row;
	do
	{
		if(column_search == NULL && get_string(temp->data).compare(column_name) == 0)
			return temp2;
		else if(temp == column_search)
			return temp2;
			
		temp = temp->next;
		temp2 = temp2->next;
	}
	while( temp != table[0]);
	
	return NULL;
 }
 
 void free_tables()
 {
	for(int i = 0; (*(table_pool + i)) != NULL; i++)
	{
		//circular_test((*(table_pool + i))->table);
		free_tabledatalist((*(table_pool + i))->table);
		//free(*(table_pool + i)); 
		table_pool[i]=NULL;
	}
	//cout << "Tables freed\n";
 }
 
 bool free_tabledatalist(TABLEDATALIST ** tabledatalist)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST 		* temp1,
						* temp2;
						
	int 				count = 0;
	
	//cout << convert_to_string(tabledatalist[0]) << "varchar";
	if (tabledatalist == NULL || tabledatalist[0] == NULL)
	{
		cout << "TABLE EMPTY";
		return false;
	}
	temp2 = temp1 = tabledatalist[0];
	
	do{
		if(temp1->up != tabledatalist[0])
		{
			do
			{
				
				temp1 = temp1->next;	
				free(temp1->prev);
			}while(temp1!=temp2);
		}
		temp2 = temp2->down;
		free(temp2->up);
		temp1 = temp2;		
	//}while(temp2 != NULL);
	}while(temp2 != tabledatalist[0]);
	
	free(temp2);
	
	return true;
}