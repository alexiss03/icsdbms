/*
	Drops 'column' from tablename
*/
bool drop_column(string tablename, string column_name)
{
	TABLEDATALIST ** table;
	commit();
	table = get_table(tablename);
	if(delete_column(table, column_name))
	{
		commit();
		return true;
	}
	return false;
	
}

/*
	Adds 'column' from tablename
*/
bool add_column(string tablename, string column_name, string type)
{

	TABLEDATALIST 	** table,
					* previousCol,
					* nextCol,
					* newData,
					* previousRow = NULL;
	commit();
	table = get_table(tablename);

	previousCol = table[0]->prev;
	nextCol = table[0];

	newData = new TABLEDATALIST(new string(column_name), "varchar");
	previousCol->next = newData;
	newData->prev = previousCol;
	newData->next = nextCol;
	nextCol->prev = newData;
	previousRow = newData;
	previousCol = previousCol->down;
	nextCol = nextCol->down;


	previousRow->down = new TABLEDATALIST(new string(type), "varchar");
	previousRow->down->up = previousRow;
	newData = previousRow->down;
	previousCol->next = newData;
	newData->prev = previousCol;
	newData->next = nextCol;
	nextCol->prev = newData;
	previousRow = previousRow->down;
	previousCol = previousCol->down;
	nextCol = nextCol->down;

	//return true;

	while(nextCol != table[0])
	{
		previousRow->down = new TABLEDATALIST(NULL, "");
		previousRow->down->up = previousRow;
		newData = previousRow->down;
		previousCol->next = newData;
		newData->prev = previousCol;
		newData->next = nextCol;
		nextCol->prev = newData;
		previousRow = previousRow->down;
		previousCol = previousCol->down;
		nextCol = nextCol->down;
	}

	previousRow->down = previousCol->next;
	previousCol->next->up = previousRow;
	circular_test(table);
	return true;
	
}