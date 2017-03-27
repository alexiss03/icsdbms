#ifndef UPDATESEMANTICS_HPP
#define UPDATESEMANTICS_HPP

void instantiateQuery(UPDATEQUERYPARSE *& query)
{
	query = new UPDATEQUERYPARSE();

	query->tablename = "";
	query->column_value = NULL;
	query->search_condition = NULL;
}

bool instantiateTable(UPDATEQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables)
{
	if(check_exist(lexeme.c_str()))
	{
		query->tablename = lexeme;
	
		tables.push_back(vector<string>());
		tables.back().push_back(lexeme);
		
		return true;
	}

	cout << "semantic error: table '" << lexeme << "' does not exist" << endl;

	return false;
}

bool instantiatePair(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	if (token == "T_TABLECOLUMN")
	{
		int pos = lexeme.find_first_of('.');
		bool isDefined = false;
		for (int i = 0; i < tables.size(); i++)
		{
			for (int j = 0; j < tables[i].size(); j++)
			{
				if (lexeme.substr(0,pos) == tables[i][j])
				{
					isDefined = true;
					break;
				}
			}
			if(isDefined)
				break;
		}

		if (isDefined)
		{
			if(check_column_name(tables[0][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
			{
				DATA * newData;

				newData = new DATA("varchar", new string(lexeme.substr(pos+1, lexeme.length()-pos-1)));
				newData->data2 = NULL;

				if (query->column_value == NULL)
					query->column_value = newData;
				else
				{
					DATA * temp = query->column_value;

					while (temp->next != NULL)
						temp = temp->next;

					newData->prev = temp;
					temp->next = newData;
				}

				return true;
			}

			cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[0][0] << "'" << endl;
		}
		else
			cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;
	}
	else if (token == "T_IDENT")
	{
		if(check_column_name(tables[0][0].c_str(), lexeme.c_str()))
		{
			DATA * newData;

			newData = new DATA("varchar", new string(lexeme));
			newData->data2 = NULL;			

			if (query->column_value == NULL)
				query->column_value = newData;
			else
			{
				DATA * temp = query->column_value;

				while (temp->next != NULL)
					temp = temp->next;

				newData->prev = temp;
				temp->next = newData;
			}

			return true;
		}

		cout << "semantic error: column '" << lexeme << "' does not exist in table '" << tables[0][0] << "'" << endl;
	}

	return false;
}

bool updateValue(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	DATA * temp = query->column_value;
	while (temp->next != NULL)
		temp = temp->next;
	
	if (token == "T_INT" && check_data_type(tables[0][0].c_str(), (*static_cast <string *>(temp->data)), "int"))
	{
		int val;
		istringstream ss(lexeme);

		ss >> val;

		DATA * newData;

		newData = new DATA("int", new int(val));
		newData->data2 = NULL;

		temp->down = newData;

		return true;
	}

	else if (token == "T_VARCHAR" && check_data_type(tables[0][0].c_str(), (*static_cast <string *>(temp->data)), "varchar"))
	{
		if ((lexeme.length()-2) <= get_varchar_size(tables[0][0].c_str(), (*static_cast <string *>(temp->data))))
		{
			DATA * newData;

			newData = new DATA("varchar", new string(lexeme.substr(1,lexeme.length()-2)));
			newData->data2 = NULL;
			temp->down = newData;
		
			return true;
		}
		else
		{
			DATA * newData;

			newData = new DATA("varchar", new string(lexeme.substr(1,get_varchar_size(tables[0][0].c_str(), (*static_cast <string *>(temp->data))))));
			newData->data2 = NULL;
			temp->down = newData;

			cout << "semantic warning: '" << lexeme.substr(1,lexeme.length()-2) << "' truncated due to incompliance in allowable max length" << endl;

			return true;
		}

	}

	else if (token == "T_FLOAT" && check_data_type(tables[0][0].c_str(), (*static_cast <string *>(temp->data)), "float"))
	{
		float val;
		istringstream ss(lexeme);

		ss >> val;

		DATA * newData;

		newData = new DATA("float", new float(val));
		newData->data2 = NULL;

		temp->down = newData;

		return true;
	}
	else if (token == "null")
	{
		vector<string> listOfColumns(get_not_null_columns(tables[0][0].c_str()));
		int i;

		for (i = 0; i < listOfColumns.size(); i++)
		{
			if (listOfColumns[i] == (*static_cast <string *>(temp->data)))
				break;
		}

		if (i < listOfColumns.size())
			cout << "semantic error: '" << listOfColumns[i] << "' must not be null"<< endl;
		else
		{
			DATA * newData;

			newData = new DATA("", NULL);
			newData->data2 = NULL;
			temp->down = newData;

			return true;
		}
	}

	if(!(token == "T_VARCHAR" && check_data_type(tables[0][0].c_str(), (*static_cast <string *>(temp->data)), "varchar")) && token != "null")
		cout << "semantic error: type mismatch of '" << (*static_cast <string *>(temp->data)) << "' and '" << lexeme << "'" << endl;

	return false;
}

bool instantiateCondition(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	if (token == "T_TABLECOLUMN")
	{
		int pos = lexeme.find_first_of('.');
		bool isDefined = false;

		for (int i = 0; i < tables.size(); i++)
		{
			for (int j = 0; j < tables[i].size(); j++)
			{
				if (lexeme.substr(0,pos) == tables[i][j])
				{
					isDefined = true;
					break;
				}
			}
			if(isDefined)
				break;
		}

		if (isDefined)
		{
			if(check_column_name(tables[0][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
			{
				CONDLIST * newCondList;

				newCondList = new CONDLIST("");
				newCondList->column_name = lexeme.substr(pos+1, lexeme.length()-pos-1);

				if (query->search_condition == NULL)
					query->search_condition = newCondList;
				else
				{
					CONDLIST * temp = query->search_condition;
					while (temp->next != NULL)
						temp = temp->next;

					newCondList->prev = temp;
					temp->next = newCondList;
				}

				return true;
			}

			cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[0][0] << "'" << endl;
		}
		else
			cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;
	}
	else if (token == "T_IDENT")
	{
		if(check_column_name(tables[0][0].c_str(), lexeme.c_str()))
		{
			CONDLIST * newCondList;

			newCondList = new CONDLIST("");
			newCondList->column_name = lexeme;

			if (query->search_condition == NULL)
				query->search_condition = newCondList;
			else
			{
				CONDLIST * temp = query->search_condition;
				while (temp->next != NULL)
					temp = temp->next;

				newCondList->prev = temp;
				temp->next = newCondList;
			}

			return true;
		}

		cout << "semantic error: column '" << lexeme << "' does not exist in table '" << tables[0][0] << "'" << endl;

	}
	else if (token == "and" || token == "or")
	{
		CONDLIST * newCondList;

		newCondList = new CONDLIST(lexeme);

		CONDLIST * temp = query->search_condition;

		while (temp->next != NULL)
			temp = temp->next;

		newCondList->prev = temp;
		temp->next = newCondList;

		return true;
	}

	return false;
}

void updateCompCondition(UPDATEQUERYPARSE *& query, string lexeme)
{
	CONDLIST * temp = query->search_condition;

	while (temp->next != NULL)
		temp = temp->next;

	if(temp->op == "")
		temp->op = lexeme;
	else
		temp->op += " " + lexeme;
}

bool updateRightCondition(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	CONDLIST * temp = query->search_condition;

	while (temp->next != NULL)
		temp = temp->next;
	
	if (token == "T_INT" && check_data_type(tables[0][0].c_str(), temp->column_name , "int"))
	{
		int val;
		istringstream ss(lexeme);

		ss >> val;
		temp->value = new int(val);
		temp->type = "int";

		return true;
	}

	else if (token == "T_VARCHAR" && check_data_type(tables[0][0].c_str(), temp->column_name , "varchar"))
	{
		temp->value = new string(lexeme.substr(1,lexeme.length()-2));
		temp->type = "varchar";

		return true;
	}

	else if (token == "T_FLOAT" && check_data_type(tables[0][0].c_str(), temp->column_name , "float"))
	{
		float val;
		istringstream ss(lexeme);

		ss >> val;
		temp->value = new float(val);
		temp->type = "float";

		return true;
	}

	cout << "semantic error: type mismatch of '" << temp->column_name << "' and '" << lexeme << "'" << endl;

	return false;
}

bool updateRightList(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	CONDLIST * temp = query->search_condition;

	while (temp->next != NULL)
		temp = temp->next;
	
	if (token == "T_INT" && check_data_type(tables[0][0].c_str(), temp->column_name , "int"))
	{
		int val;
		istringstream ss(lexeme);

		ss >> val;
		temp->type = "int";

		DATA * newData;

		newData = new DATA("int", new int(val));
		newData->data2 = NULL;

		if(temp->column1 == NULL)		
			temp->column1 = newData;
		else
		{
			DATA * temp2 = temp->column1;

			while(temp2->next != NULL)
				temp2 = temp2->next;

			newData->prev = temp2;
			temp2->next = newData;
		}

		return true;
	}

	else if (token == "T_VARCHAR" && check_data_type(tables[0][0].c_str(), temp->column_name , "varchar"))
	{
		DATA * newData;

		temp->type = "varchar";
		newData = new DATA("varchar", new string(lexeme.substr(1,lexeme.length()-2)));
		newData->data2 = NULL;

		if(temp->column1 == NULL)		
			temp->column1 = newData;
		else
		{
			DATA * temp2 = temp->column1;

			while(temp2->next != NULL)
				temp2 = temp2->next;

			newData->prev = temp2;
			temp2->next = newData;
		}

		return true;
	}

	else if (token == "T_FLOAT" && check_data_type(tables[0][0].c_str(), temp->column_name , "float"))
	{
		float val;
		istringstream ss(lexeme);

		ss >> val;
		temp->type = "float";

		DATA * newData;

		newData = new DATA("float", new float(val));
		newData->data2 = NULL;

		if(temp->column1 == NULL)		
			temp->column1 = newData;
		else
		{
			DATA * temp2 = temp->column1;

			while(temp2->next != NULL)
				temp2 = temp2->next;

			newData->prev = temp2;
			temp2->next = newData;
		}

		return true;
	}

	cout << "semantic error: type mismatch of '" << temp->column_name << "' and '" << lexeme << "'" << endl;

	return false;
}

#endif
