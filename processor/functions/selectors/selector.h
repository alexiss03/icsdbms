TABLEDATALIST ** selector(	TABLEDATALIST ** table
							)
{
	TABLEDATALIST 	* temp = table[0],
					** selector = NULL;
	
	int 			count = 0,
					rcount = row_count(table[0]);
					

	selector = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * rcount + 1);
	
	for(int i=0; i < rcount; i++)
	{
		*(selector + i) = NULL;
	}
	
	temp = temp->down->down;
	
	do{
		*(selector + count) = temp;
		count++;
		temp = temp->down;
		
	}while(temp != table[0]);					
	
	return selector;
}

TABLEDATALIST ** selector(	TABLEDATALIST ** table,
							string colname, 
							string op,
							string * value, 
							TABLEDATALIST ** rows = NULL, 
							string lolop = "",
							TABLEDATALIST * column_search = NULL,
							TABLEDATALIST * row_search = NULL
							)
{	
	TABLEDATALIST 	* temp = table[0],
					* temp2Table = NULL,
					** selector = NULL,
					* temp2 = NULL,
					* temp3 = NULL,
					* temp4 = NULL;
					
	DATA 			* headData = NULL,
					* tailData = NULL,
					* temp1Data = NULL,
					*temp2Data = NULL;
					
	string 			str = *value,
					strTmp1,
					strTmp2,
					strTmp3;

	int 			count = 0,
					intTmp1 = 0,
					intTmp2 = 0,
					rcount = row_count(table[0]),
					strlength = str.length();
	
	int 			j;
					

	selector = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * rcount + 1);	

	for(int i=0; i<rcount + 1; i++)
	{
		*(selector + i) = NULL;
	}

	
	//START: finds the column to be searched
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
	//END: finds the column to be searched
	
	//START: checks if column is not found
	if(temp==NULL)
		return NULL;
	//END: checks if column is not found
	
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
			//parses the value of the LIKE loloperator
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
	}
											//::Test printing for the parsing of value for LIKE lolop (DATA - LINKED LIST)
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
	if(rows == NULL && strlength != 0)
	{
		do
		{
			temp1Data = headData;
			if(temp->data != NULL)
			{
				strTmp2 = *(static_cast<string *>(temp->data));
				do
				{
					strTmp1 = *(static_cast<string *>(temp1Data->data));
					if(strTmp1.compare("%") == 0)
					{
						if(temp1Data->next == NULL)
						{
							temp2 = find_row_head(table, temp);
							*(selector + count) = temp2;
							count++;
						}
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
						if((*(static_cast<string *>(temp1Data->next->data))).compare("^") == 0 || get_string(temp1Data->next->data).compare("!") == 0)
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
							else
								break;

							if(strTmp2.length() == 0)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
								break;
							}
						}
					}

					if(strTmp2.compare("")==0)
					{
						temp2 = find_row_head(table, temp);
						*(selector + count) = temp2;
						count++;
						break;
					}
					temp1Data = temp1Data->next;
				}while(temp1Data!=NULL);
			}
			
			if(row_search != NULL)
				break;
				
			temp = temp->down;
			
		}while(temp->down->up != temp2Table);
	}
	else if(lolop.compare("and") == 0)
	{
		int j;

		//for(j = 0; *(rows + j) != NULL; j++);

		//selector = (TABLEDATALIST **) malloc(sizeof(TABLEDATALIST *) * j + 1);

		for(j = 0; *(rows + j) != NULL; j++)
			selector[j] = NULL;

		for(int i = 0; *(rows + i) != NULL; i++)
		{
			temp3 = table[0];
			temp4 = *(rows+i);
			
			while((*(static_cast<string *>(temp3->data))).compare(colname) != 0)
			{
				temp3 = temp3->next;
				temp4 = temp4->next;
			}
			
			temp = temp4;
			temp1Data = headData;
			if(temp->data != NULL && strlength !=0)
			{
				strTmp2 = *(static_cast<string *>(temp->data));
				do
				{
					strTmp1 = *(static_cast<string *>(temp1Data->data));
					if(strTmp1.compare("%") == 0)
					{
						if(temp1Data->next == NULL)
						{
							temp2 = find_row_head(table, temp);
							*(selector + count) = temp2;
							count++;
						}
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
							if(temp1Data->prev != NULL && (*(static_cast<string *>(temp1Data->prev->data))).compare("%") == 0)
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
							else
								break;
							
							
							if(strTmp2.length() == 0)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
								break;
							}
						}
					}

					if(strTmp2.compare("")==0)
					{
						temp2 = find_row_head(table, temp);
						*(selector + count) = temp2;
						count++;
						break;
					}

					temp1Data = temp1Data->next;
				}while(temp1Data != NULL);
			}
			temp = temp->down;
		}
		
	}
	else if(lolop.compare("or") == 0)
	{
		for(count = 0; *(rows + count) != NULL; count++);

		if(count == row_count(table[0]))
			return rows;
		
		temp3 = table[0]->down->down;

		selector = rows;
		
		int i = 0;
		
		do
		{
			if(temp == *(rows + i)) 
			{
				i++;
				temp = temp->down;
			}
			else
			{
				if(temp->data != NULL && strlength != 0)
				{
					temp1Data = headData;
					strTmp2 = *(static_cast<string *>(temp->data));
					do
					{
						strTmp1 = *(static_cast<string *>(temp1Data->data));
						if(strTmp1.compare("%") == 0)
						{
							if(temp1Data->next == NULL)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
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
								else
									break;
								
								
								if(strTmp2.length() == 0)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
									break;
								}
							}
						}

						if(strTmp2.compare("")==0)
						{
							temp2 = find_row_head(table, temp);
							*(selector + count) = temp2;
							count++;	
							break;
						}
						temp1Data = temp1Data->next;
					}while(temp1Data!=NULL);

					temp = temp->down;
				}
			}
					
		}while(temp->down->up != temp2Table);
		
	}
		// if(selector[0] != NULL) cout << convert_to_string(selector[0]);
		// return NULL;
	
	return selector;
}

TABLEDATALIST ** selector(	TABLEDATALIST ** table, 
							string colname,
							string type,
							void * lower = NULL,
							bool lBound=false,
							void * upper = NULL,
							bool uBound = false,
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
							
	int rcount = row_count(table[0]), count = 0;
	//if(rows == NULL)
	//{
		selector = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * rcount + 1);
		
		for(int i=0; i < rcount + 1; i++)
		{
			*(selector + i) = NULL;
		}
	//}
	
	int * iUpper = NULL, * iLower = NULL;
	float * fUpper = NULL, * fLower = NULL;

	//checks if there is no lower bound and upper bound
	if(lower == NULL && upper == NULL)
		return NULL;
	
	//checks the type of data
	if(type.compare("int")==0)
	{
		if(lower != NULL)
			iLower = static_cast<int*>(lower);
		if(upper != NULL)
			iUpper = static_cast<int*>(upper);
	}
	else if((type.compare("float")==0))
	{
		if(lower != NULL)
			fLower = static_cast<float*>(lower);
		if(upper != NULL)
			fUpper = static_cast<float*>(upper);	
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
		return NULL;
	else if(rows == NULL)
	{
		column = temp;
		
		if(row_search == NULL)
			temp = temp->down->down;
		else
			temp = row_search;
		//checks again the type of data
		if(type.compare("int")==0 || type.compare("float")==0)
		{
			//holds the data from each data node
			float tempdata;		

			do
			{				
				if(temp->data != NULL)
				{
					//traverses the column linked list
					//cout << type << endl;
					//traverses the column linked list
					//tempdata = *(static_cast<float *>(temp->data));
				

					if(temp->type.compare("int")==0)
						tempdata = *(static_cast<int *>(temp->data));
					else if(temp->type.compare("float")==0)
						tempdata = *(static_cast<float *>(temp->data));					
						//cout << "tempdata" << *fLower;
					//return NULL;
					//case: has both lower bound and upper bound
					if(temp->type.compare("int")==0)
					{
						
						if(lower != NULL && upper != NULL)
						{			
							if( tempdata > *iLower &&  tempdata < *iUpper )
							{
								
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
								
							}
							//case: lower than or equal
							else if(uBound==true && *iUpper==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: greater than or equal
							else if(lBound==true && *iLower==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							else if(lBound == false && uBound == false && *iLower != tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;	
							}
						}
						//case: has upper bound only
						else if(upper != NULL)
						{
							if( tempdata < *iUpper )
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: lower than or equal
							else if(uBound==true && *iUpper==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
						}
						//case: has lower bound only
						else
						{
							if( tempdata > *iLower )
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: greater than or equal
							else if(lBound==true && *iLower==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
						}
					}
					else if(temp->type.compare("float")==0)
					{
						if(lower != NULL && upper != NULL)
						{			
							if( tempdata > *fLower &&  tempdata < *fUpper )
							{
								
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
								
							}
							//case: lower than or equal
							else if(uBound==true && *fUpper==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: greater than or equal
							else if(lBound==true && *fLower==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							else if(lBound == false && uBound == false && *fLower != tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;	
							}
						}
						//case: has upper bound only
						else if(upper != NULL)
						{
							if( tempdata < *fUpper )
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: lower than or equal
							else if(uBound==true && *fUpper==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
						}
						//case: has lower bound only
						else
						{
							if( tempdata > *fLower )
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
							//case: greater than or equal
							else if(lBound==true && *fLower==tempdata)
							{
								temp2 = find_row_head(table, temp);
								*(selector + count) = temp2;
								count++;
							}
						}
					}
				}
				
				//for JOIN
				if(row_search != NULL)
					break;
				
				temp = temp->down;
			}
			while(temp != column);
			
			return selector;
		}
	}
	else if(lolop.compare("and") == 0)
	{
		int j;

		for(j = 0; *(rows + j) != NULL; j++);

		//selector = (TABLEDATALIST **) malloc(sizeof(TABLEDATALIST *) * j + 1);

		//for(j = 0; *(rows + j) != NULL; j++)
		//{
		//	selector[j] = NULL;
		//}

		for(int i = 0; *(rows + i) != NULL; i++)
		{
			temp3 = table[0];
			temp4 = *(rows+i);
			
			while((*(static_cast<string *>(temp3->data))).compare(colname) != 0)
			{
				temp3 = temp3->next;
				temp4 = temp4->next;
			}
			
			column = temp4;
			temp = temp4;

			//temp = temp->down;
			//checks again the type of data
			if(type.compare("int")==0 || type.compare("float")==0)
			{
				//holds the data from each data node
				float tempdata;
				//do
				//{
					if(temp->data != NULL)
					{
						//traverses the column linked list
						if(temp->type.compare("int")==0)
							tempdata = *(static_cast<int *>(temp->data));
						else if(temp->type.compare("float")==0)
							tempdata = *(static_cast<float *>(temp->data));	
						
						//case: has both lower bound and upper bound
						if(temp->type.compare("int")==0)
						{
							if(lower != NULL && upper != NULL)
							{			
								if( tempdata > *iLower &&  tempdata < *iUpper )
								{	
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: lower than or equal
								else if(uBound==true && *iUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *iLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;

								}
								else if(lBound == false && uBound == false && *iLower != tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;	
								}
							}
							//case: has upper bound only
							else if(upper != NULL)
							{
								if( tempdata < *iUpper )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: lower than or equal
								else if(uBound==true && *iUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
							//case: has lower bound only
							else
							{
								if( tempdata > *iLower )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *iLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
						}
						else if(temp->type.compare("float")==0)
						{
							if(lower != NULL && upper != NULL)
							{			
								if( tempdata > *fLower &&  tempdata < *fUpper )
								{
									
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
									
								}
								//case: lower than or equal
								else if(uBound==true && *fUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *fLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								else if(lBound == false && uBound == false && *fLower != tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;	
								}
							}
							//case: has upper bound only
							else if(upper != NULL)
							{
								if( tempdata < *fUpper )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: lower than or equal
								else if(uBound==true && *fUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
							//case: has lower bound only
							else
							{
								if( tempdata > *fLower )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *fLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
						}
					}
				//}
				//while(temp != column);
				
				
			}
		}
		return selector;
	}
	else if(lolop.compare("or") == 0)
	{
		count=0;
		temp3 = table[0]->down->down;
	
		//selector = (TABLEDATALIST **) malloc(sizeof(TABLEDATALIST *)* 10);

		//selector = rows;

		int i = 0;
		
		column = temp;
		temp = temp->down->down;
		
		//checks again the type of data
		if(type.compare("int")==0 || type.compare("float")==0)
		{
			//holds the data from each data node
			int tempdata;
			do
			{
				//if(temp3 == *(rows + i)) 
				if(*(rows + i) != NULL && temp3 == *(rows + i)) 
				{
					i++;
					*(selector + count++) = temp3;
					temp = temp->down;
					temp3 = temp3->down;

				}
				else
				{
					if(temp->data != NULL)
					{
						//traverses the column linked list
						if(temp->type.compare("int")==0)
							tempdata = *(static_cast<int *>(temp->data));
						else if(temp->type.compare("float")==0)
							tempdata = *(static_cast<float *>(temp->data));	
						
						//case: has both lower bound and upper bound
						if(temp->type.compare("int") == 0)
						{
							if(lower != NULL && upper != NULL)
							{	
								if( tempdata > *iLower &&  tempdata < *iUpper )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
									
								}
								//case: lower than or equal
								else if(uBound==true && *iUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;

								}
								//case: greater than or equal
								else if(lBound==true && *iLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								else if(lBound == false && uBound == false && *iLower != tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;	
								}
							}
							//case: has upper bound only
							else if(upper != NULL)
							{
								if( tempdata < *iUpper)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: lower than or equal
								else if(uBound==true && *iUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;

								}
							}
							//case: has lower bound only
							else
							{
								if( tempdata > *iLower )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && (*iLower==tempdata || tempdata > *iLower))
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
						}
						else if(temp->type.compare("float")==0)
						{
							if(lower != NULL && upper != NULL)
							{			
								if( tempdata > *fLower &&  tempdata < *fUpper )
								{
									
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
									
								}
								//case: lower than or equal
								else if(uBound==true && *fUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *fLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								else if(lBound == false && uBound == false && *fLower != tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;	
								}
							}
							//case: has upper bound only
							else if(upper != NULL)
							{
								if( tempdata < *fUpper )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: lower than or equal
								else if(uBound==true && *fUpper==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
							//case: has lower bound only
							else
							{
								if( tempdata > *fLower )
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
								//case: greater than or equal
								else if(lBound==true && *fLower==tempdata)
								{
									temp2 = find_row_head(table, temp);
									*(selector + count) = temp2;
									count++;
								}
							}
						}
					}
					
					temp = temp->down;
					temp3 = temp3->down;
				}
			}
			while(temp != column);

			//cout << convert_to_string(*(rows + count) -1) << " ";
			*(selector + count) = NULL;
			return selector;
		}
	}
}

TABLEDATALIST ** selector(	TABLEDATALIST ** table,
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
							
	int rcount = row_count(table[0]), count = 0;

	//if(rows == NULL)
//	{
		//cout << "row is null";
		selector = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *) * rcount + 1);
		
		for(int i=0; i<rcount + 1; i++)
		{
			*(selector + i) = NULL;
		}
//	}
	
	if(lower == NULL && upper == NULL)
		return NULL;
		
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
			return NULL;
		else if(rows == NULL)
		{
			column = temp;
			
			if(row_search == NULL)
				temp = temp->down->down;
			else
				temp = row_search;
				
			do{
				if(temp->data == NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
				
				if(row_search != NULL)
					break;
				
				temp = temp->down;
			}while(temp != column);

			return selector;
		}
		else if (lolop.compare("and") == 0)
		{
			int j;

			for(j = 0; *(rows + j) != NULL; j++)
				selector[j] = NULL;

			for(int i = 0; *(rows + i) != NULL; i++)
			{
				temp3 = table[0];
				temp4 = *(rows+i);
				
				while((*(static_cast<string *>(temp3->data))).compare(colname) != 0)
				{
					temp3 = temp3->next;
					temp4 = temp4->next;
				}
				
				column = temp4;
				temp = temp4;

				if(temp->data == NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
				temp = temp->down;
			}
			return selector;
		}
		else if(lolop.compare("or") == 0)
		{
		
			for(count = 0; *(rows + count) != NULL; count++);
			
			temp3 = table[0]->down->down;
			selector = rows;
			
			int i = 0;
			
			column = temp;
			temp = temp->down->down;
			
			do{

				if(*(rows + i) != NULL && temp3 == *(rows + i)) 
				{
					i++;
				}
				
				else if(temp->data == NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
					
				temp3 = temp3->down;
				temp = temp->down;		
						
			}while(temp != column);
			
			return selector;
		}	
	}
	else if(op.compare("is not null") == 0)
	{
		//checks if column is not found
		if(temp==NULL)
			return NULL;
		else if(rows == NULL)
		{
			column = temp;
			
			if(row_search == NULL)
				temp = temp->down->down;
			else
				temp = row_search;
				
			do{
				if(temp->data != NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
				
				if(row_search != NULL)
					break;
				
				temp = temp->down;
			}while(temp != column);

			return selector;
		}
		else if (lolop.compare("and") == 0)
		{
			int j;

			for(j = 0; *(rows + j) != NULL; j++)
				selector[j] = NULL;

			for(int i = 0; *(rows + i) != NULL; i++)
			{
				temp3 = table[0];
				temp4 = *(rows+i);
				
				while((*(static_cast<string *>(temp3->data))).compare(colname) != 0)
				{
					temp3 = temp3->next;
					temp4 = temp4->next;
				}
				
				column = temp4;
				temp = temp4;

				if(temp->data != NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
				temp = temp->down;
			}
			return selector;
		}
		else if(lolop.compare("or") == 0)
		{
		
			for(count = 0; *(rows + count) != NULL; count++);
			
			temp3 = table[0]->down->down;
			selector = rows;
			
			int i = 0;
			
			column = temp;
			temp = temp->down->down;
			
			do{

				if(*(rows + i) != NULL && temp3 == *(rows + i)) 
				{
					i++;
				}
				
				else if(temp->data != NULL)
				{
					temp2 = find_row_head(table, temp);
					*(selector + count) = temp2;
					count++;
				}
					
				temp3 = temp3->down;
				temp = temp->down;		
						
			}while(temp != column);
			
			return selector;
		}	
	}
}