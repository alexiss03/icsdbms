/*
	Sorts column in 'order_grouping->data' according to 'order_grouping->down->data'

*/

TABLEDATALIST ** order_by(TABLEDATALIST ** tabledatalist, string tablename, DATA * order_grouping)
{
	TABLEDATALIST 	* temp = tabledatalist[0],
					** rows = NULL;
	node			* root = NULL;
		
	
	temp = get_column(tabledatalist[0], get_string(order_grouping->data));

	if(temp != NULL)
	{
		if(order_grouping->down == NULL || get_string(order_grouping->down->data).compare("asc") == 0)
		{
			root = bpt_index_exists(tablename, get_string(order_grouping->data));
			if(root != NULL)
			{
				rows = sort_asc(root, tabledatalist);
			}
			else
			{
				rows = sort_asc(insert_to_bpt_pool(tablename,
				get_string(order_grouping->data),
				build_bpt_index(tabledatalist, find_column(tabledatalist, get_string(order_grouping->data)))),
				tabledatalist);
			}
		}
		else if(get_string(order_grouping->down->data).compare("desc") == 0)
		{
			root = bpt_index_exists(tablename, get_string(order_grouping->data));
			
			if(root != NULL)
			{
				rows = sort_desc(root, tabledatalist);
			}
			else
			{
				rows = sort_desc(insert_to_bpt_pool(tablename,
				get_string(order_grouping->data),
				build_bpt_index(tabledatalist, get_column(tabledatalist[0], get_string(order_grouping->data)))),
				tabledatalist);

			}
			
		}
	}
	return rows;
}


/*
	Returns a bpt index of 'table' according to 'column'. 
	Caches the index into 'bpt_pool'
*/
node * build_bpt_index( TABLEDATALIST ** table, TABLEDATALIST * column )
{
	TABLEDATALIST 	* temp = table[0]->down->down,
					* temp2 = column->down->down,
					* temp3 = NULL,
					* temp4 = NULL,
					* temp5 = NULL;

	node 			* root = NULL;
	int 			nullFiller = -10;

	
	if(get_string(column->down->data).compare("int") != 0 && get_string(column->down->data).compare("float") != 0
		&& get_string(column->down->data).compare("varchar") != 0)
		return NULL;
	else if(get_string(column->down->data).compare("int") == 0)
	{
		do
		{
			temp5 = temp;
			temp3 = temp->down;
			temp4 = temp2->down;

			while(temp4 != column)
			{
				if(is_equal(temp2, temp4))
				{
					temp5->data2 = temp3;
					temp5 = temp5->data2;
				}

				temp3 = temp3->down;
				temp4 = temp4->down;
			}

			if(temp2->data != NULL)
			{
				root = insert(root, (float) get_int(temp2->data), (float) get_int(temp2->data), temp);
			}
			else
			{
				root = insert(root, nullFiller--, nullFiller--, temp);	
			}

			temp = temp->down;
			temp2 = temp2->down;
		}
		while( temp != table[0] );
	}
	else if(get_string(column->down->data).compare("float") == 0)
	{
		do
		{

			temp5 = temp;
			temp3 = temp->down;
			temp4 = temp2->down;

			if(temp2->data != NULL)
			{
				root = insert(root, get_float(temp2->data), get_float(temp2->data), temp);
			}
			else
			{
				root = insert(root, nullFiller--, nullFiller--, temp);	
			}

			temp = temp->down;
			temp2 = temp2->down;
		}
		while( temp != table[0] );
	}
	else if(get_string(column->down->data).compare("varchar") == 0)
	{
		float key;
		int length;
		string str;
		int rcount = row_count(table[0]);
		string strArray[rcount];
		int j = 0;

		temp5 = temp2;

		do
		{
			// if(temp5->data != NULL)
			if(temp5->data != NULL)
			{
				strArray[j++] = get_string(temp5->data);
			}
			else
				root = insert(root, nullFiller--, nullFiller--, temp);

			temp = temp->down;
			temp5 = temp5->down;
		}
		while(temp5 != column);

		sort(strArray + 0, strArray + rcount);
			
		for (size_t i = 0; i != rcount; ++i)
		{
			temp = table[0]->down->down;
			temp2 = column->down->down;

			if((i==0 || strArray[i].compare(strArray[i-1]) != 0) && (i!=0 || strArray[i].compare("") != 0))	
			{
    			do
    			{
    				if(temp2->data != NULL && strArray[i].compare(get_string(temp2->data)) == 0)
    				{
    					break;
    				}

    				temp = temp->down;
    				temp2 = temp2->down;
 
    			}
    			while(temp != table[0]);		

    			temp5 = temp;
				temp3 = temp->down;
				temp4 = temp2->down;		

				if(i!=rcount && strArray[i].compare(strArray[i+1]) == 0)
				{
					do
					{
						if(temp2->data !=NULL && temp4->data !=NULL) 
						{
							if(is_equal(temp2, temp4))
							{
								
								temp5->data2 = temp3;
								temp5 = temp5->data2;
							}
						}

						temp4 = temp4->down;
						temp3 = temp3->down;
					}
					while(temp4 != column);
				}

				if(temp2->data != NULL)
				{
					root = insert(root, i, i, temp);
				}
			}
    	}
	}
	return root;
}

/*
	Retrieves ascendingly the nodes in root 
*/
TABLEDATALIST ** sort_asc(node * root,  TABLEDATALIST ** table)
{
	TABLEDATALIST ** rows;
	rows = get_leaves_asc(root, row_count(table[0]));
	/*
	for(int count = 0; *(rows + count) != NULL; count++)
	{
		cout << get_string((*(rows + count))->data) << "\n";
	}
	*/	
	return rows;
}

/*
	Retrieves descendingly the nodes in root 
*/

TABLEDATALIST ** sort_desc(node * root,  TABLEDATALIST ** table)
{
	TABLEDATALIST ** rows;
	rows = get_leaves_desc(root, row_count(table[0]));	
	/*	
	for(int count = 0; *(rows + count) != NULL; count++)
	{
		cout << get_string((*(rows + count))->data) << "\n";
		
	}
	*/	
	return rows;
}

node * bpt_index_exists(string tablename, string columnname)
{
	
	for(int i = 0; *(bpt_pool + i) != NULL; i++)
	{
		if((*(bpt_pool + i))->table_name.compare(tablename) == 0 && (*(bpt_pool + i))->column_name.compare(columnname) == 0)
			return (*(bpt_pool + i))->root;
	}
	return NULL;
}

node * insert_to_bpt_pool(string table_name, string column_name, node * root)
{
	int 	i;
	
	for(i = 0; *(bpt_pool + i) != NULL; i++);	
	*(bpt_pool + i) = new BPT(table_name, column_name, root);
	return root;
}
