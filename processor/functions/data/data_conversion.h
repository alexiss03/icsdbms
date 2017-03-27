bool is_first_of_the_repeated_column(DATA * columnlist, DATA * current)
{
	DATA * temp = columnlist;
	while(temp != NULL)
	{	
		if(get_string(temp->data).compare(get_string(current->data)) == 0)
		{
			if(temp == current)
				return true;
			else
				return false;
		}
		temp = temp->next;
	}

}



/*
	Deletes the first duplicate row
*/
bool delete_first_duplicate(DATA * columnlist, string column)
{
	bool boolTemp;
	DATA 	* temp = columnlist,
			* prev = NULL;
	boolTemp = false;

	while(temp != NULL)
	{
		if(get_string(temp->data).compare(column) == 0)
		{
			if(!boolTemp)
				boolTemp = true;
			else if(boolTemp)
			{
				prev->next = temp->next;
				return true;
			}
		}
		
		prev = temp;
		temp = temp->next;

	}

	return true;
}


/*
	Detects if 'cell1' and 'cell2' are equal
*/
bool is_equal(TABLEDATALIST * cell1, TABLEDATALIST * cell2)
{
	void 	* data = cell1->data,
			* data2 = cell2->data;
			
	string type = cell1->type;
	
	if(cell1->type.compare(cell2->type) == 0)
	{
	
		if(type.compare("varchar") == 0 || type.compare("CHAR") == 0)
		{
			if(get_string(data).compare(get_string(data2)) == 0)
				return true;
			return false;
		}
		else if(type.compare("int") == 0)
		{
			if(get_int(data) == get_int(data2))
				return true;
			return false;
		}
		else if(type.compare("float") == 0)
		{
			if(get_float(data) == get_float(data2))
				return true;
			return false;
		}
		else if(type.compare("bool") == 0)
		{
			if(get_bool(data) == get_bool(data2))
				return true;
			return false;
		}
		else if(type.compare("date") == 0)
		{
			if((*(static_cast<DATE *>(data))).month == (*(static_cast<DATE *>(data2))).month)
				if((*(static_cast<DATE *>(data))).day == (*(static_cast<DATE *>(data2))).day)
					if((*(static_cast<DATE *>(data))).year == (*(static_cast<DATE *>(data2))).year)
						return true;
			else false;
		}	
	}
	else
		return false;
}


/*
	Detects if 'node1' and 'node2' are equal
*/
bool is_equal(CONDLIST * node1, CONDLIST * node2)
{
	if(node1->column_name.compare(node2->column_name) == 0)
	{
		if(node1->op.compare(node2->op) == 0)
		{
			if(node1->type.compare(node2->type) == 0)
				return is_void_equal(node1->type, node1->value, node2->value);
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}


/*
	Detects if void 'data' and 'data2' given type 'type'
*/

bool is_void_equal(string type, void * data, void * data2)
{
	if(type.compare("varchar") == 0 || type.compare("CHAR") == 0)
	{
		if(get_string(data).compare(get_string(data2)) == 0)
			return true;
		return false;
	}
	else if(type.compare("int") == 0)
	{
		if(get_int(data) == get_int(data2))
			return true;
		return false;
	}
	else if(type.compare("float") == 0)
	{
		if(get_float(data) == get_float(data2))
			return true;
		return false;
	}
	else if(type.compare("bool") == 0)
	{
		if(get_bool(data) == get_bool(data2))
			return true;
		return false;
	}
	else if(type.compare("date") == 0)
	{
		if((*(static_cast<DATE *>(data))).month == (*(static_cast<DATE *>(data2))).month)
			if((*(static_cast<DATE *>(data))).day == (*(static_cast<DATE *>(data2))).day)
				if((*(static_cast<DATE *>(data))).year == (*(static_cast<DATE *>(data2))).year)
					return true;
		else false;
	}
}

float convert_to_float(TABLEDATALIST * temp)
{
	if(temp->type.compare("int") == 0)
		return (float) get_int(temp->data);
	else if(temp->type.compare("float") == 0)
		return get_float(temp->data);
}

string convert_to_string(TABLEDATALIST * temp)
{

	if(temp->data == NULL)
		return "NULL";
	else if(temp->type.compare("varchar") == 0 || temp->type.compare("CHAR") == 0)
		return get_string(temp->data);
	else if(temp->type.compare("int") == 0)
		return to_string(get_int(temp->data));
	else if(temp->type.compare("float") == 0)
	{
		char intStr[1000];
		sprintf(intStr, "%.5f", get_float(temp->data));
		return std::string(intStr);
	}
	else if(temp->type.compare("bool") == 0)
	{
		if(get_bool(temp->data))
			return "1";
		else
			return "0";
	}
	else if(temp->type.compare("date") == 0)
	{
		return to_string((*(static_cast<DATE *>(temp->data))).month) +
				"-" +
				to_string((*(static_cast<DATE *>(temp->data))).day) +
				"-" +
				to_string((*(static_cast<DATE *>(temp->data))).year);
	}
}


string get_string(void * data)
{
	return *(static_cast<string *>(data));
}

int get_int(void * data)
{
	return *(static_cast<int *>(data));
}

float get_float(void * data)
{
	return *(static_cast<float *>(data));
}

int get_bool(void * data)
{
	if(*(static_cast<bool *>(data)))
		return 1;
	else
		return 0;	
}

DATE get_date(void * data)
{
	return *(static_cast<DATE *>(data));
		
}