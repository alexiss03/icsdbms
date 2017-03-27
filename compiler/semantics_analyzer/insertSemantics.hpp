#ifndef INSERTSEMANTICS_HPP
#define INSERTSEMANTICS_HPP

void instantiateQuery(INSERTQUERYPARSE *& query)
{
	query = new INSERTQUERYPARSE();

	query->tablename = "";
	query->datalist = NULL;
	query->inserttype = "";
}

bool instantiateTable(INSERTQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables)
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

void updateInsertType(INSERTQUERYPARSE *& query, string lexeme)
{
	if (lexeme == "values")
		query->inserttype = "manual";
}

bool instantiateColumn(INSERTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	if(check_column_name(tables[0][0].c_str(), lexeme.c_str()))
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));	
		newData->data2 = NULL;

		if (query->datalist == NULL)
			query->datalist = newData;
		else
		{
			DATA * temp = query->datalist;

			while (temp->next != NULL)
				temp = temp->next;

			newData->prev = temp;
			temp->next = newData;
		}

		return true;
	}

	cout << "semantic error: column '" << lexeme << "' does not exist in table '" << tables[0][0] << "'" << endl;

	return false;
}

void autoInstantiateColumn(INSERTQUERYPARSE *& query, vector<vector<string> >& tables)
{
	vector<string> listOfColumns(show_table_names(show_table_contents(tables[0][0].c_str())));

	for (int i = 0; i < listOfColumns.size(); i++)
	{
		DATA * newData;

		newData = new DATA("varchar", new string(listOfColumns[i]));	
		newData->data2 = NULL;

		if (query->datalist == NULL)
			query->datalist = newData;
		else
		{
			DATA * temp = query->datalist;

			while (temp->next != NULL)
				temp = temp->next;

			newData->prev = temp;
			temp->next = newData;
		}
	}
}

bool updateValue(INSERTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	DATA * temp = query->datalist;

	while (temp != NULL && temp->down != NULL)
		temp = temp->next;
	
	if (temp != NULL)
	{
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
			DATA * newData;

			newData = new DATA("", NULL);
			newData->data2 = NULL;
			temp->down = newData;

			return true;
		}

		cout << "semantic error: type mismatch of '" << (*static_cast <string *>(temp->data)) << "' and '" << lexeme << "'" << endl;

		return false;
	}

	cout << "semantic error: column count does not match data count" << endl;

	return false;
}

bool checkNotNullableColumns(INSERTQUERYPARSE *& query, vector<vector<string> >& tables)
{
	vector<string> listOfColumns(get_not_null_columns(tables[0][0].c_str()));
	int i, j;
	bool isExisting;
	DATA * temp;

	for (i = 0; i < listOfColumns.size(); i++)
	{
		temp = query->datalist;
		isExisting = false;

		while (temp != NULL)
		{
			if (listOfColumns[i] == (*static_cast <string *>(temp->data)) && temp->down->type != "")
			{
				isExisting = true;
				break;
			}
			temp = temp->next;
		}

		if (!isExisting)
		{
			cout << "semantic error: '" << listOfColumns[i] << "' must not be null"<< endl;
			return false;
		}
	}

	return true;
}

bool checkCount(INSERTQUERYPARSE *& query)
{
	bool isBalance = true;
	DATA * temp = query->datalist;

	while (temp != NULL && isBalance)
	{		
		if (temp->down == NULL)
			isBalance = false;

		temp = temp->next;
	}

	if (!isBalance)
		cout << "semantic error: column count does not match data count" << endl;

	return isBalance;
}

#endif
