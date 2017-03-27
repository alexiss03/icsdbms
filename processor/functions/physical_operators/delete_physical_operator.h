bool delete_op(DELETEQUERYPARSE query)
{

	TABLEDATALIST 	** table = get_table(query.tablename), 
					** rows = NULL;
	bool			temp;
	int 			i;

	clear_bpt_pool();
	
	if(table == NULL)
	{
		return false;
	}
	else
	{
		if(query.search_condition == NULL)
		{
			rows = select_all_rows(table);
			i = row_count(table[0]);
			delete_physical_operator(table, rows);
			if(!isImport)
					cout << "\n" << i <<" row/s deleted\n";
			return false;
		}
		else
		{
			init = clock();
			//searches the best candidate
			//query.search_condition = best_candidate_selector(table, match_pattern(table, query.search_condition));

			//selects the row using 'query.search_condition'
			rows = condlist_selector(table,query.search_condition);
		
			//counts the rows selected
			for(i=0; *(rows + i)!= NULL; i++);
		
			//deletes the rows selected
			temp = delete_physical_operator(table, rows);

			final = clock()-init;
			time_processed = (float)final / ((float)CLOCKS_PER_SEC);

			if(temp)
			{
				
				if(!isImport)
					cout << "\n" << i <<" row/s deleted\n";
 				return true;
			}
			else
			{
				cout << "\n Delete query not processed\n";
				return false;
			}
		}
	}
}

/*
	Deletes the 'rows' in 'tabledatalist'
*/
bool delete_physical_operator(TABLEDATALIST ** tabledatalist, TABLEDATALIST ** rows)
{

	TABLEDATALIST ** 	tabledataTemp1 = rows,
						* tabledataTemp2 = tabledatalist[0], 
						* firstRow = NULL,
						* temp = NULL;
	
	string 				column_name, 
						stringTemp1;
						
	int 				i=0;
	
	if(tabledatalist == NULL)
	{
		return false;
	}
	else if(rows == NULL)
		return true;


	for(int i = 0; *(rows + i) != NULL; i++)
	{
		temp = *(rows + i);
		do
		{
			temp->up->down = temp->down;
			temp->down->up = temp->up;
			temp = temp->next;
			//free(temp->prev);
		}while(temp != *(rows + i));
	}
	
	return true;
}