/*
	Transfer all the cached table into tmp
	Clears the table cache
	Clears the bpt cache
	Empties tmp and transfer all file into main
*/

void commit()
{
	for(int i = 0; *(table_pool + i) != NULL; i++)
	{
		write_tabledatalist(table_pool[i]->table, table_pool[i]->tablename.c_str());
		table_pool[i] = NULL;
	}

	clear_bpt_pool();
	compare_data_bank();
}

/*
	Removes all files in tmp
	Clears the table cache
*/

void rollback()
{
	TABLEDATALIST ** temp;	
	for(int i = 0; *(table_pool + i) != NULL; i++)
	{
		remove_all_in_tmp(table_pool[i]->tablename.c_str());
		table_pool[i] = NULL;
	 	//free(*(table_pool + i));
	}
	clear_bpt_pool();
}


/*
	Clears the bpt pool
*/
void clear_bpt_pool()
{
	for(int i = 0; *(bpt_pool + i) != NULL; i++)
	{
		bpt_pool[i] = NULL;
	}
}
