bool substr(TABLEDATALIST * column, int pos, int len)
{
	TABLEDATALIST 			* temp;
	string 					stringTemp;
	if(column == NULL)
		return false;
	else
	{
		temp = column;
		temp->down->update(new string("varchar"), "varchar");
		temp = temp->down->down;
		do
		{
			if(temp->data != NULL)
			{
				stringTemp = convert_to_string(temp);
				if( pos >  stringTemp.length())
					stringTemp = "";
				else if( pos + len > stringTemp.length())
					stringTemp = stringTemp.substr(pos-1);
				else
					stringTemp = stringTemp.substr(pos-1, len);
				temp->data = new string(stringTemp);
				temp->type = "varchar";
				
			}
			temp = temp->down;
		}while(temp != column);
	}
	return true;	
}

bool replace(TABLEDATALIST * column, string tosearchfor, string replacewiththis )
{
	TABLEDATALIST 			* temp;
	string 					stringTemp;
	int 					strpos,
							start = 0;
	if(column == NULL)
		return false;
	else
	{
		temp = column;
		temp->down->update(new string("varchar"), "varchar");
		temp = temp->down->down;
		do
		{
			if(temp->data != NULL)
			{
				stringTemp = convert_to_string(temp);
				cout << stringTemp;
				do{
					if(stringTemp.find(tosearchfor, start) != std::string::npos)
					{
						strpos = stringTemp.find(tosearchfor);
						stringTemp.replace(strpos, tosearchfor.length(), replacewiththis);
						start = stringTemp.find_last_of(replacewiththis, start) + replacewiththis.length();
					}
					else
						break;
				}while(1);
				start = 0;
				temp->data = new string(stringTemp);
				temp->type = "varchar";
				
			}
			temp = temp->down;
		}while(temp != column);
	}
	return true;	
}

TABLEDATALIST * length(TABLEDATALIST * column)
{
	TABLEDATALIST 		* temp1 = NULL,
						* temp3,
						* temp4,
						* newcolumn;
						
	string 				stringTemp1;
						
	char				* charTemp1,
						* charTemp2;
	
	int					intTemp;
						
	if(column == NULL)
		return false;
	else
	{
		temp1 = column;
		
		temp1 = temp1->down->down;
		
		newcolumn = new TABLEDATALIST(new string("length(" + get_string(column->data) + ")"), "varchar");
		newcolumn->prev = newcolumn->next = newcolumn;
		newcolumn->down = new TABLEDATALIST(new string("int"), "varchar");
		newcolumn->down->prev = newcolumn->down->next = newcolumn;
		newcolumn->down->up = newcolumn;
		temp3 = newcolumn->down;
		
		do
		{
			stringTemp1 = convert_to_string(temp1);							
			if(temp1->data != NULL)
				temp4 = new TABLEDATALIST(new int(stringTemp1.length()), "int");
			else
				temp4 = new TABLEDATALIST(NULL, "");
				
			temp3->down = temp4;
			temp4->up = temp3;
			temp3 = temp3->down;
			temp1 = temp1->down;
		}while(temp1 != column);
		
		temp3->down = newcolumn;
		newcolumn->up = temp3;
		
		return newcolumn;
	}
}

bool initcap(TABLEDATALIST * column)
{
	TABLEDATALIST 			* temp;
	string 					stringTemp;
	if(column == NULL)
		return false;
	else
	{
		temp = column;
		temp = temp->down->down;
		do
		{
			//cout << "Loop";
			if(temp->data != NULL)
			{
				stringTemp = convert_to_string(temp);
				for(int i = 0; i< stringTemp.length(); i++)
				{
					if(i==0)
						stringTemp[i] = toupper(stringTemp[i]);
					else if(stringTemp[i-1] == ' ')
						stringTemp[i] = toupper(stringTemp[i]);
					else
						stringTemp[i] = tolower(stringTemp[i]);
				}
				temp->type = "varchar";
				temp->data = new string(stringTemp);
			}
			temp = temp->down;
		}while(temp != column);
	}
	return true;	
}

TABLEDATALIST * concat(TABLEDATALIST * column1, TABLEDATALIST * column2)
{
	TABLEDATALIST 		* temp1 = NULL,
						* temp2 = NULL,
						* temp3,
						* temp4,
						* newcolumn;
						
	string 				stringTemp1,
						stringTemp2,
						stringTemp3;
						
	char				* charTemp1,
						* charTemp2;
	
						
	if(column1 == NULL || column2 == NULL)
		return false;
	else
	{
		temp1 = column1;
		temp2 = column2;
		
		temp1 = temp1->down->down;
		temp2 = temp2->down->down;
		
		newcolumn = new TABLEDATALIST(new string("concat(" + get_string(temp1->data) + "," + get_string(temp2->data) + ")"), "varchar");
		newcolumn->prev = newcolumn->next = newcolumn;
		newcolumn->down = new TABLEDATALIST(new string("varchar"), "varchar");
		newcolumn->down->prev = newcolumn->down->next = newcolumn;
		newcolumn->down->up = newcolumn;
		temp3 = newcolumn->down;
		
		do
		{
			cout << "Loop";

			stringTemp1 = convert_to_string(temp1);
			stringTemp2 = convert_to_string(temp2);
							
			temp4 = new TABLEDATALIST(new string(stringTemp1 + stringTemp2), "varchar");
			temp3->down = temp4;
			temp4->up = temp3;
			temp3 = temp3->down;
			
			temp1 = temp1->down;
			temp2 = temp2->down;
			
		}while(temp1 != column1);
		
		
		temp3->down = newcolumn;
		newcolumn->up = temp3;
		
		return newcolumn;
	}
}

bool lower(TABLEDATALIST * column)
{
	TABLEDATALIST 			* temp;
	string 					stringTemp;
	if(column == NULL)
		return false;
	else
	{
		temp = column;
		temp->down->update(new string("varchar"), "varchar");
		temp = temp->down->down;
		do
		{
			
			if(temp->data !=  NULL)
			{
				stringTemp = convert_to_string(temp);
				for(int i = 0; i< stringTemp.length(); i++)
				{
					
					stringTemp[i] = tolower(stringTemp[i]);
				}
				temp->type = "varchar";
				temp->data = new string(stringTemp);
			}
			temp = temp->down;
		}while(temp != column);
	}
	return true;	
}

bool upper(TABLEDATALIST * column)
{
	TABLEDATALIST 		* temp;
	string 				stringTemp;
	if(column == NULL)
		return false;
	else
	{
		temp = column;
		temp->down->update(new string("varchar"), "varchar");
		temp = temp->down->down;
		do
		{
			if(temp->data !=  NULL)
			{
				stringTemp = convert_to_string(temp);
				for(int i = 0; i< stringTemp.length(); i++)
				{
					
					stringTemp[i] = toupper(stringTemp[i]);
				}
				temp->type = "varchar";
				temp->data = new string(stringTemp);
			}
			temp = temp->down;
		}while(temp != column);
	}
	return true;	
}