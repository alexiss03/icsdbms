#ifndef DELETESEMANTICS_HPP
#define DELETESEMANTICS_HPP

void instantiateQuery(DELETEQUERYPARSE *& query)
{
	query = new DELETEQUERYPARSE();

	query->tablename = "";
	query->search_condition = NULL;
}

bool instantiateTable(DELETEQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables)
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

void instantiateTableAlias(vector<vector<string> >& tables, string lexeme)
{
	tables.back().push_back(lexeme);
}

bool instantiateCondition(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
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

void updateCompCondition(DELETEQUERYPARSE *& query, string lexeme)
{
	CONDLIST * temp = query->search_condition;

	while (temp->next != NULL)
		temp = temp->next;

	if(temp->op == "")
		temp->op = lexeme;
	else
		temp->op += " " + lexeme;
}

bool updateRightCondition(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
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

bool updateRightList(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
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
