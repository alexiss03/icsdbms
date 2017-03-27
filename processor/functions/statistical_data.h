//min
void * column_min(string table_name, string column_name)
{
	node 			* root = NULL;
	TABLEDATALIST 	** rows;
	
	root = bpt_index_exists(table_name, column_name);
	TABLEDATALIST ** table = get_table_from_table_pool(table_name)->table;
	
	if(root != NULL)
	{
		rows = sort_asc(root, table);
	}
	else
	{
		rows = sort_asc(insert_to_bpt_pool(table_name,
		column_name,
		build_bpt_index(table, get_column(table[0], column_name))),
		table);
	}
	
	return get_cell(table, (*(rows + 0)), column_name)->data;
}

//max
void * column_max(string table_name, string column_name)
{
	node 			* root = NULL;
	TABLEDATALIST 	** rows;
	
	root = bpt_index_exists(table_name, column_name);
	TABLEDATALIST ** table = get_table_from_table_pool(table_name)->table;
	
	if(root != NULL)
	{
		rows = sort_desc(root, table);
	}
	else
	{
		rows = sort_desc(insert_to_bpt_pool(table_name,
		column_name,
		build_bpt_index(table, get_column(table[0], column_name))),
		table);
	}
	
	return get_cell(table, (*(rows + 0)), column_name)->data;
}

//operator LIKE %, _, [^ ], [! ], [ ] 
int row_count(TABLEDATALIST ** table, string colname, string op, string * value, TABLEDATALIST * column_search = NULL,
							TABLEDATALIST * row_search = NULL)
{	
	
	TABLEDATALIST * temp = table[0], * temp2Table = NULL;
	DATA * headData = NULL, * tailData = NULL, * temp1Data = NULL, *temp2Data = NULL;
	int count = 0, intTmp1 = 0, intTmp2 = 0;
	string str = *value, strTmp1, strTmp2, strTmp3;
	int strlength = str.length();
	
	//finds the column to be searched
	while(temp->down != table[0])
	{
		if((*(static_cast<string *>(temp->data))).compare(colname) == 0)
			break;
		else
			temp = temp->next;
	}
	
	//checks if column is not found
	if(temp==NULL)
		return -1;
	
	//temp2Table = permanent holder of column header 
	temp2Table = temp;
	
	
	if(column_search == NULL)
		temp = temp->down->down;
	else
		temp = row_search;
	
	if(op.compare("like") == 0)
	{	
		for(int i=0; i<strlength; i++)
		{
			//parses the value of the LIKE operator
			if(str[i] == '%' || str[i] == '_' || str[i] == '[' || str[i] == ']' || str[i] == '^')
			{
				int lowerLimit, upperLimit, temp;
				
				
					upperLimit = temp = i - 1;
					
					while(temp != -1 && (str[temp] != '%' && str[temp] != '_' && str[temp] != '[' && str[temp] != ']' && str[temp] != '^' && str[temp] != '!'))
					{
						temp--;
					}
					
					lowerLimit = temp + 1;
					
					if(upperLimit - lowerLimit != -1)
					{
						if(headData == NULL)
						{
							temp1Data = new DATA("varchar", new string(str.substr(lowerLimit, upperLimit-lowerLimit+1)));
							temp2Data = new DATA("varchar", new string(str.substr(i,1)));
							temp1Data->link(temp2Data);
							headData = temp1Data;
							tailData = temp2Data;
						}
						else
						{
							temp1Data = new DATA("varchar", new string(str.substr(lowerLimit, upperLimit-lowerLimit+1)));
							temp2Data = new DATA("varchar", new string(str.substr(i,1)));
							temp1Data->link(temp2Data);
							tailData->link(temp1Data);
							tailData->next = temp1Data;
							tailData = temp2Data;
						}
					}
					else{
						if(headData == NULL)
						{
							temp1Data = new DATA("varchar", new string(str.substr(i,1)));
							headData = temp1Data;
							tailData = temp1Data;
						}
						else
						{
							temp1Data = new DATA("varchar", new string(str.substr(i,1)));
							tailData->link(temp1Data);
							tailData = temp1Data;
						}
					}
				

			}
			else if(i==strlength-1)
			{
				int lowerLimit, upperLimit, temp;
				
				upperLimit = temp = i;
					
				while(temp != -1 && (str[temp] != '%' && str[temp] != '_' && str[temp] != '[' && str[temp] != ']' && str[temp] != '^' && str[temp] != '!'))
				{
					temp--;
					//cout << "Entered" << "\n";
				}
				
				lowerLimit = temp + 1;
				
				if(headData == NULL)
				{
					temp1Data = new DATA("varchar", new string(str.substr(lowerLimit, upperLimit-lowerLimit+1)));
					headData = temp1Data;
					tailData = temp1Data;
				}
				else
				{
					temp1Data = new DATA("varchar", new string(str.substr(lowerLimit, upperLimit-lowerLimit+1)));
					tailData->link(temp1Data);
					tailData = temp1Data;
				}
			}		
		}
	}else if(op.compare("=") == 0)
	{
		headData = new DATA("varchar", value);
	}
											//::Test printing for the parsing of value for LIKE op (DATA - LINKED LIST)
											/*
											temp1Data = headData;
											do
											{
												cout << *(static_cast<string *>(temp1Data->data)) << "\n";
												temp1Data = temp1Data->next;
											}while(temp1Data!=NULL);
											cout << "Finished parsing" << "\n";
											*/
				
	/*
		temp = first row of data
		temp1Data = parsed value
		strTmp1 = string in the temp1Data
		strTmp2 = current string from the row data
		The algorithm is cut the string from the table using the 
			parsed linked list constructed until it is empty
	*/
	
	do
	{
		if(temp->data != NULL)
		{
			temp1Data = headData;
			strTmp2 = *(static_cast<string *>(temp->data));
			do
			{
				strTmp1 = *(static_cast<string *>(temp1Data->data));
				if(strTmp1.compare("%") == 0)
				{
					if(temp1Data->next == NULL)
						count++;				
				}
				else if(strTmp1.compare("_") == 0)
				{
					strTmp2.erase(0,1);
				}
				else if(strTmp1.compare("]") == 0)
				{
					//do nothing
				}
				else if(strTmp1.compare("[") == 0)
				{
					if((*(static_cast<string *>(temp1Data->next->data))).compare("^") == 0 || (*(static_cast<string *>(temp1Data->next->data))).compare("!") == 0)
					{
						int i;
						strTmp3 = *(static_cast<string *>(temp1Data->next->next->data));
						for(i=0; i < strTmp3.length(); i++)
						{
							if(strTmp3[i] == strTmp2[0])
								break;
						}
						
						if(i == strTmp3.length())
						{
							strTmp2.erase(0,1);
							temp1Data = temp1Data->next->next;
						}
					}
					else
					{
						strTmp3 = *(static_cast<string *>(temp1Data->next->data));
						
						int i;
						for(i=0; i < strTmp3.length(); i++)
						{
							if(strTmp3[i] == strTmp2[0])
							{
								strTmp2.erase(0,1);
								temp1Data = temp1Data->next;
								break;
							}
						}
						
						if(i == strTmp3.length())
						{
							break;
						}
					}
				}
				else
				{
					intTmp1 = strTmp2.find(strTmp1);
					if( intTmp1 == -1)
					{
						break;
					}
					else
					{
						if(temp1Data->prev!=NULL && (*(static_cast<string *>(temp1Data->prev->data))).compare("%") == 0)
						{
							while(strTmp2.find(strTmp1) != -1)
							{
								intTmp2 = intTmp1 + strTmp1.length();
								if(intTmp2 < strTmp2.length())
									strTmp2 = strTmp2.substr(intTmp2);
								else
									strTmp2 = "";
							}
						}
						else if(intTmp1 == 0)
						{
							intTmp2 = intTmp1 + strTmp1.length();
							strTmp2 = strTmp2.substr(intTmp2);
						}
						
						
						if(strTmp2.length() == 0)
						{
							count++;
							break;
						}
					}
				}
				temp1Data = temp1Data->next;
			}while(temp1Data!=NULL);
			
			if(row_search != NULL)
				break;
		}

		temp = temp->down;
		
	}while(temp->down->up != temp2Table);
	
	return count;
}

//>,<,!=,>=,<=,= operators
int row_count	(	TABLEDATALIST ** table,
					string colname,
					string type,
					void * lower = NULL,
					bool lBound=false,
					void * upper = NULL,
					bool uBound=false,
					TABLEDATALIST * column_search = NULL,
					TABLEDATALIST * row_search = NULL
				)
{	
	
	TABLEDATALIST 			* temp = table[0], 
							* column = NULL,
							** selector,
							* temp2,
							* temp3,
							* temp4;
							
	int rcount = row_count(table[0]), count = 0;
	
		selector = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * rcount + 1);
		
		for(int i=0; i < rcount + 1; i++)
		{
			*(selector + i) = NULL;
		}
	
	int * iUpper = NULL, * iLower = NULL;
	float * fUpper = NULL, * fLower = NULL;

	//checks if there is no lower bound and upper bound
	if(lower == NULL && upper == NULL)
		return 0;
	
	//checks the type of data
	if(type.compare("int")==0)
	{
		if(lower != NULL)
			iLower = static_cast<int*>(lower);
		if(upper != NULL)
			iUpper = static_cast<int*>(upper);
	}
	
	//finds the column to be searched
	if(column_search == NULL)
	{
		while(temp->down != table[0])
		{
			if((*(static_cast<string *>(temp->data))).compare(colname) == 0)
				break;
			else
				temp = temp->next;
		}
	}
	else
		temp = column_search;
	
	//checks if column is not found
	if(temp == NULL)
		return 0;
	else
	{
		column = temp;
		
		temp = temp->down->down;

		//checks again the type of data
		if(type.compare("int")==0 || type.compare("float")==0)
		{
			//holds the data from each data node
			int tempdata;		
			do
			{				
				if(temp->data != NULL)
				{				
					if(temp->type.compare("int")==0)
						tempdata = *(static_cast<int *>(temp->data));
					else if(temp->type.compare("float")==0)
						tempdata = *(static_cast<float *>(temp->data));					
					
					//case: has both lower bound and upper bound
					if(lower != NULL && upper != NULL)
					{			
						if( tempdata > *iLower &&  tempdata < *iUpper )
						{
							
							temp2 = find_row_head(table, temp);
							count++;
							
						}
						//case: lower than or equal
						else if(uBound==true && *iUpper==tempdata)
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
						//case: greater than or equal
						else if(lBound==true && *iLower==tempdata)
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
						else if(lBound == false && uBound == false && *iLower != tempdata)
						{
							temp2 = find_row_head(table, temp);
							count++;	
						}
					}
					//case: has upper bound only
					else if(upper != NULL)
					{
						if( tempdata < *iUpper )
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
						//case: lower than or equal
						else if(uBound==true && *iUpper==tempdata)
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
					}
					//case: has lower bound only
					else
					{
						if( tempdata > *iLower )
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
						//case: greater than or equal
						else if(lBound==true && *iLower==tempdata)
						{
							temp2 = find_row_head(table, temp);
							count++;
						}
					}
				}
				
				temp = temp->down;
			}
			while(temp != column);
			
			return count;
		}
	}
}


int row_count(	TABLEDATALIST ** table,
							string colname, 
							string op,
							TABLEDATALIST ** rows = NULL, 
							string lolop = "",
							TABLEDATALIST * column_search = NULL,
							TABLEDATALIST * row_search = NULL
							)
{

	TABLEDATALIST 			* temp = table[0], 
							* column = NULL,
							** selector,
							* temp2,
							* temp3,
							* temp4;
							
	int 					count = 0;
	
	//checks if there is no lower bound and upper bound
	if(lower == NULL && upper == NULL)
		return 0;
		
	//finds the column to be searched
	if(column_search == NULL)
	{
		while(temp->down != table[0])
		{
			if((*(static_cast<string *>(temp->data))).compare(colname) == 0)
				break;
			else
				temp = temp->next;
		}
	}
	else
		temp = column_search;
	
	if(op.compare("is null") == 0)
	{
		//checks if column is not found
		if(temp==NULL)
			return 0;
		else if(rows == NULL)
		{
			column = temp;
			if(row_search == NULL)
				temp = temp->down->down;
			else
				temp = row_search;
			do{
				if(temp->data == NULL)
					count++;
					
				if(row_search != NULL)
					break;
					
				temp = temp->down;
			}while(temp != column);

			return count;
		}
		
	}
	else if(op.compare("is not null") == 0)
	{
		//checks if column is not found
		if(temp==NULL)
			return 0;
		else if(rows == NULL)
		{
			column = temp;
			
			if(row_search == NULL)
				temp = temp->down->down;
			else
				temp = row_search;
			do{
				if(temp->data == NULL)
					count++;
				
				if(row_search != NULL)
					break;
					
				temp = temp->down;
			}while(temp != column);

			return count;
		}
	}
}

int row_count(TABLEDATALIST * table)
{
	int count = 0;
	TABLEDATALIST * temp = table;

	while(temp->down != table)
	{
		
		temp = temp->down;
		count++;

	}
	
	return count - 1;
}

int col_count(TABLEDATALIST ** table)
{
	TABLEDATALIST * temp = table[0];
	int count = 0;
	
	while(temp!=NULL)
	{
		count++;
		temp = temp->next;
	}
	
	return count;
}

