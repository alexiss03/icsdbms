#ifndef SELECTSEMANTICS_HPP
#define SELECTSEMANTICS_HPP

void instantiateQuery(SELECTQUERYPARSE *& query)
{
	query = new SELECTQUERYPARSE();

	query->table = NULL;
	query->columnlist = NULL;
	query->display_constraint = NULL;
	query->join_condition = NULL;
	query->search_condition = NULL;
	query->group_grouping = NULL;
	query->group_condition = NULL;
	query->order_grouping = NULL;
}

void instantiateDisplay(SELECTQUERYPARSE *& query, string lexeme, string token)
{
	if (token == "T_INT")
	{
		int val;
		istringstream ss(lexeme);

		ss >> val;

		DATA * newData;

		newData = new DATA("int", new int(val));	
		newData->data2 = NULL;

		if (query->display_constraint == NULL)
			query->display_constraint = newData;
		else
		{
			DATA * temp = query->display_constraint;

			while (temp->down != NULL)
				temp = temp->down;

			newData->up = temp;
			temp->down = newData;
		}
	}
	else if (token == "T_FLOAT")
	{
		float val;
		istringstream ss(lexeme);

		ss >> val;

		DATA * newData;

		newData = new DATA("float", new float(val));	
		newData->data2 = NULL;

		if (query->display_constraint == NULL)
			query->display_constraint = newData;
		else
		{
			DATA * temp = query->display_constraint;

			while (temp->down != NULL)
				temp = temp->down;

			newData->up = temp;
			temp->down = newData;
		}
	}
	else if (token == "%")
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));	
		newData->data2 = NULL;

		if (query->display_constraint == NULL)
			query->display_constraint = newData;
		else
		{
			DATA * temp = query->display_constraint;

			while (temp->down != NULL)
				temp = temp->down;

			newData->up = temp;
			temp->down = newData;
		}
	}
	else if (token == "top" || token == "distinct")
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));
		newData->data2 = NULL;

		if (query->display_constraint == NULL)
			query->display_constraint = newData;
		else
		{
			DATA * temp = query->display_constraint;

			while (temp->down != NULL)
				temp = temp->down;

			newData->up = temp;
			temp->down = newData;
		}
	}
}

void instantiateColumn(SELECTQUERYPARSE *& query, string lexeme, string token, AST *& curEquation, vector<vector<string> >& columns)
{
	if (token == "*" || token == "round" || token == "to_number" || token == "max" || token == "avg" || token == "count" || token == "first" || token == "last" || token == "min" || token == "sum" || token == "T_IDENT")
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;

			newData->prev = temp;
			temp->next = newData;
		}

		if (token == "T_IDENT")
		{
			columns.push_back(vector<string>());
			columns.back().push_back(lexeme);
		}
	}
	else if (token == "T_TABLETIMES" || token == "T_TABLECOLUMN")
	{
		int pos = lexeme.find_first_of('.');

		DATA * newData;

		newData = new DATA("varchar", new string(lexeme.substr(pos+1, lexeme.length()-pos-1)));
		newData->data2 = new string(lexeme.substr(0,pos));

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;

			newData->prev = temp;
			temp->next = newData;
		}

		columns.push_back(vector<string>());
		columns.back().push_back(lexeme.substr(pos+1, lexeme.length()-pos-1));
		columns.back().push_back(lexeme.substr(0,pos));
	}
	/*
	else if (token == "T_IDENT")
	{
		DATA * newData;

		newData = new DATA("", new AST(""));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			newData->prev = temp;
			temp->next = newData;
		}
	
		curEquation = (AST *)newData->data;
		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new string(lexeme));
		else
			curEquation->value2 = new VALUENODE(new string(lexeme));
		columns.push_back(vector<string>());
		columns.back().push_back(lexeme);
	}
	//cout << static_cast<AST*>(query->columnlist->data)->op;
	else if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');

		DATA * newData;

		newData = new DATA("", new AST(""));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			newData->prev = temp;
			temp->next = newData;
		}
	
		curEquation = (AST *)newData->data;	
		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new string(lexeme.substr(pos+1, lexeme.length()-pos-1)), new string(lexeme.substr(0,pos)));
		else
			curEquation->value2 = new VALUENODE(new string(lexeme.substr(pos+1, lexeme.length()-pos-1)), new string(lexeme.substr(0,pos)));
		columns.push_back(vector<string>());
		columns.back().push_back(lexeme.substr(pos+1, lexeme.length()-pos-1));		
	}
	else if (token == "T_INT")
	{
		int val;
		istringstream ss(lexeme);
		ss >> val;

		DATA * newData;

		newData = new DATA("", new AST(""));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			newData->prev = temp;
			temp->next = newData;
		}
	
		curEquation = (AST *)newData->data;	
		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new int(val));
		else
			curEquation->value2 = new VALUENODE(new int(val));		
	}
	else if (token == "T_FLOAT")
	{
		float val;
		istringstream ss(lexeme);
		ss >> val;

		DATA * newData;

		newData = new DATA("", new AST(""));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			newData->prev = temp;
			temp->next = newData;
		}
	
		curEquation = (AST *)newData->data;	
		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new float(val));
		else
			curEquation->value2 = new VALUENODE(new float(val));		
	}
	else if (token == "(")
	{
		DATA * newData;

		newData = new DATA("", new AST(""));
		newData->data2 = NULL;

		if (query->columnlist == NULL)
			query->columnlist = newData;
		else
		{
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			newData->prev = temp;
			temp->next = newData;
		}

		curEquation = (AST *)newData->data;

		AST * emptyEquation;

		emptyEquation = new AST("");

		if (curEquation->value1 == NULL)
			curEquation->value1 = emptyEquation;
		else
			curEquation->value2 = emptyEquation;

		emptyEquation->prev = curEquation;
		curEquation = emptyEquation;
	}
	*/
}

/*
void updateAST(SELECTQUERYPARSE *& query, string lexeme, string token, AST *& curEquation, vector<vector<string> >& columns)
{
	if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
	{
		AST * operEquation;

		operEquation = new AST(lexeme);

		if (curEquation->value1 == NULL)
			curEquation->value1 = operEquation;
		else if (curEquation->value2 == NULL)
			curEquation->value2 = operEquation;
		else
		{
			//if (curEquation->prev == NULL)
			operEquation->value1 = curEquation;
			curEquation->prev = operEquation;
			
			DATA * temp = query->columnlist;

			while (temp->next != NULL)
				temp = temp->next;
			temp->data = operEquation;
			curEquation = operEquation;
		}
	}
	else if (token == "T_IDENT")
	{
		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new string(lexeme));
		else
			curEquation->value2 = new VALUENODE(new string(lexeme));				
		columns.push_back(vector<string>());
		columns.back().push_back(lexeme);
	}
	else if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');

		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new string(lexeme.substr(pos+1, lexeme.length()-pos-1)), new string(lexeme.substr(0,pos)));
		else
			curEquation->value2 = new VALUENODE(new string(lexeme.substr(pos+1, lexeme.length()-pos-1)), new string(lexeme.substr(0,pos)));
		columns.push_back(vector<string>());
		columns.back().push_back(lexeme.substr(pos+1, lexeme.length()-pos-1));
	}
	else if (token == "T_INT")
	{
		int val;
		istringstream ss(lexeme);
		ss >> val;

		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new int(val));
		else
			curEquation->value2 = new VALUENODE(new int(val));		
	}
	else if (token == "T_FLOAT")
	{
		float val;
		istringstream ss(lexeme);
		ss >> val;

		if (curEquation->value1 == NULL)
			curEquation->value1 = new VALUENODE(new float(val));
		else
			curEquation->value2 = new VALUENODE(new float(val));		
	}
	else if (token == "(")
	{
		AST * emptyEquation;

		emptyEquation = new AST("");

		if (curEquation->value1 == NULL)
			curEquation->value1 = emptyEquation;
		else
			curEquation->value2 = emptyEquation;

		emptyEquation->prev = curEquation;
		curEquation = emptyEquation;
	}
	else if (token == ")")
		curEquation = curEquation->prev;
}
*/

void updateColumn(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& columns)
{
	if (token == "T_INT")
	{
		int val;
		istringstream ss(lexeme);
		ss >> val;

		DATA * newData;

		newData = new DATA("int", new int(val));
		newData->data2 = NULL;

		DATA * temp = query->columnlist;

		while (temp->next != NULL)
			temp = temp->next;

		DATA * temp2 = temp;

		while (temp2->down != NULL)
			temp2 = temp2->down;

		newData->up = temp2;
		temp2->down = newData;
	}		
	else if (token == "T_IDENT")
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));
		newData->data2 = NULL;

		DATA * temp = query->columnlist;
		DATA * temp3 = query->columnlist;

		while (temp->next != NULL)
		{	
			temp3 = temp;
			temp = temp->next;
		}
		
		if (temp == query->columnlist)
		{
			newData->down = temp;
			temp->up = newData;
			query->columnlist = newData;
		}
		else
		{
			newData->down = temp;
			temp->up = newData;
			temp3->next = newData;
		}

		columns.push_back(vector<string>());
		columns.back().push_back(lexeme);
	}
	else if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');

		DATA * newData;

		newData = new DATA("varchar", new string(lexeme.substr(pos+1, lexeme.length()-pos-1)));
		newData->data2 = new string(lexeme.substr(0,pos));

		DATA * temp = query->columnlist;
		DATA * temp3 = query->columnlist;

		while (temp->next != NULL)
		{	
			temp3 = temp;
			temp = temp->next;
		}
		
		if (temp == query->columnlist)
		{
			newData->down = temp;
			temp->up = newData;
			query->columnlist = newData;
		}
		else
		{
			newData->down = temp;
			temp->up = newData;
			temp3->next = newData;
		}

		columns.push_back(vector<string>());
		columns.back().push_back(lexeme.substr(pos+1, lexeme.length()-pos-1));
		columns.back().push_back(lexeme.substr(0,pos));
	}
}

bool instantiateTable(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	if (token == "T_IDENT")
	{
		if(check_exist(lexeme.c_str()))
		{

			DATA * newData;

			newData = new DATA("varchar", new string(lexeme));	
			newData->data2 = NULL;

			if (query->table == NULL)
				query->table = newData;
			else
			{
				DATA * temp = query->table;

				while (temp->next != NULL)
					temp = temp->next;
				
				newData->prev = temp;
				temp->next = newData;
			}
			
			tables.push_back(vector<string>());
			tables.back().push_back(lexeme);

			return true;
		}

		cout << "semantic error: table '" << lexeme << "' does not exist" << endl;
	}
	else
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));	
		newData->data2 = NULL;

		if (query->table == NULL)
			query->table = newData;
		else
		{
			DATA * temp = query->table;

			while (temp->next != NULL)
				temp = temp->next;

			newData->prev = temp;
			temp->next = newData;
		}
		return true;
	}

	return false;
}

bool checkColumns(SELECTQUERYPARSE *& query, vector<vector<string> >& tables, vector<vector<string> >& columns)
{
	vector<string> listOfTables;

	for (int i = 0; i < tables.size(); i++)
		listOfTables.push_back(tables[i][0]);

	for (int i = 0; i < columns.size(); i++)
	{
		if (columns[i].size() == 1)
		{

			vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, columns[i][0].c_str()));

			if (tablesWithColumn.size() == 0)
			{
				cout << "semantic error: column '" << columns[i][0] << "' does not exist" << endl;

				return false;
			}
			else if (tablesWithColumn.size() > 1)
			{
				cout << "semantic error: ambiguous reference to '" << columns[i][0] << "'" << endl;

				return false;
			}
		}
		else if (columns[i].size() == 2)
		{	
			int j;
			for (j = 0; j < tables.size(); j++)
			{
				int k;
				for (k = 0; k < tables[j].size(); k++)
				{
					if (columns[i][1] == tables[j][k])
						break;
				}
				if (k < tables[j].size())
					break;
			}
			if (j == tables.size())
			{
				cout << "semantic error: table alias '" << columns[i][1] << "' is not defined" << endl;
				
				return false;
			}
		}
	}

	DATA * temp = query->columnlist;
	string tableAlias;
	int j;

	do
	{
		if (temp->data2 != NULL)
		{
			tableAlias = (*static_cast <string *>(temp->data2));

			for (int i = 0; i < tables.size(); i++)
			{
				for (j = 0; j < tables[i].size(); j++)
				{
					if (tableAlias == tables[i][j])
					{
						temp->data2 = new string(tables[i][0]);
						break;
					}
				}

				if (j < tables[i].size())
					break;
			}
		}

		temp = temp->next;
	} while (temp != NULL);

	return true;
}

void updateTable(SELECTQUERYPARSE *& query, string lexeme)
{
	DATA * temp = query->table;

	while (temp->next != NULL)
		temp = temp->next;
	
	temp->data = new string((*static_cast <string *>(temp->data)) + " " + lexeme);
}

bool updateOn(SELECTQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables)
{
	int i;
	int pos = lexeme.find_first_of('.');
	bool isDefined = false;

	for (i = 0; i < tables.size(); i++)
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
		if(check_column_name(tables[i][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
		{
			CONDLIST * newCondList;

			newCondList = new CONDLIST("");
			newCondList->column_name = lexeme.substr(pos+1, lexeme.length()-pos-1);

			DATA * newData;

			newData = new DATA("varchar", new string(tables[i][0]));
			newData->data2 = NULL;
			newCondList->column2 = newData;

			if (query->join_condition == NULL)
				query->join_condition = newCondList;
			else
			{
				CONDLIST * temp = query->join_condition;

				while (temp->next != NULL)
					temp = temp->next;

				newCondList->prev = temp;
				temp->next = newCondList;
			}

			return true;
		}

		cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[i][0] << "'" << endl;
	}
	else
		cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;	

	return false;
}

void instantiateColumnAlias(SELECTQUERYPARSE *& query, vector<vector<string> >& columns, string lexeme)
{
	DATA * temp = query->columnlist;
	DATA * newData;
	
	newData = new DATA("varchar", new string(lexeme));
	newData->data2 = NULL;

	while (temp->next != NULL)
		temp = temp->next;
		
	temp->up = newData;
}

bool instantiateCondition(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns)
{
	if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');
		bool isDefined = false;

		for (i = 0; i < tables.size(); i++)
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
			if(check_column_name(tables[i][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
			{
				CONDLIST * newCondList;

				newCondList = new CONDLIST("");
				newCondList->column_name = lexeme.substr(pos+1, lexeme.length()-pos-1);

				DATA * newData;

				newData = new DATA("varchar", new string(tables[i][0]));
				newData->data2 = NULL;
				newCondList->column2 = newData;

				if (query->group_grouping == NULL)
				{
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
				}
				else
				{
					if (query->group_condition == NULL)
						query->group_condition = newCondList;
					else
					{
						CONDLIST * temp = query->group_condition;

						while (temp->next != NULL)
							temp = temp->next;

						newCondList->prev = temp;
						temp->next = newCondList;
					}
				}
				return true;
			}

			cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[i][0] << "'" << endl;
		}
		else
			cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;
	}
	else if (token == "T_IDENT")
	{
		string tableName;
		int count = 0;

		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[i][0] == lexeme)
			{
				tableName = columns[i][0];
				count++;
			}
		}

		if (count > 1)
			cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;		
		else if (count == 1)
		{
			CONDLIST * newCondList;

			newCondList = new CONDLIST("");
			newCondList->column_name = lexeme;

			DATA * newData;

			newData = new DATA("varchar", new string(tableName));

			newData->data2 = NULL;
		
			newCondList->column2 = newData;

			if (query->group_grouping == NULL)
			{
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
			}
			else
			{
				if (query->group_condition == NULL)
					query->group_condition = newCondList;
				else
				{
					CONDLIST * temp = query->group_condition;

					while (temp->next != NULL)
						temp = temp->next;

					newCondList->prev = temp;
					temp->next = newCondList;
				}
			}

			return true;
		}
		else
		{
			vector<string> listOfTables;

			for (int i = 0; i < tables.size(); i++)
				listOfTables.push_back(tables[i][0]);

			vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, lexeme.c_str()));

			if (tablesWithColumn.size() == 0)
			{
				cout << "semantic error: column '" << lexeme << "' does not exist" << endl;
			}
			else if (tablesWithColumn.size() > 1)
			{
				cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;
			}
			else
			{
				CONDLIST * newCondList;

				newCondList = new CONDLIST("");
				newCondList->column_name = lexeme;

				DATA * newData;

				newData = new DATA("varchar", new string(tablesWithColumn[0]));

				newData->data2 = NULL;
			
				newCondList->column2 = newData;

				if (query->group_grouping == NULL)
				{
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
				}
				else
				{
					if (query->group_condition == NULL)
						query->group_condition = newCondList;
					else
					{
						CONDLIST * temp = query->group_condition;

						while (temp->next != NULL)
							temp = temp->next;

						newCondList->prev = temp;
						temp->next = newCondList;
					}
				}

				return true;
			}
		}
	}
	else if (token == "and" || token == "or")
	{
		CONDLIST * newCondList;

		newCondList = new CONDLIST(lexeme);

		CONDLIST * temp;

		if (query->group_grouping == NULL)
			temp = query->search_condition;
		else
			temp = query->group_condition;

		while (temp->next != NULL)
			temp = temp->next;

		newCondList->prev = temp;
		temp->next = newCondList;

		return true;
	}

	return false;
}

void updateCompCondition(SELECTQUERYPARSE *& query, string lexeme)
{
	CONDLIST * temp;

	if (query->group_grouping == NULL)
		temp = query->search_condition;
	else
		temp = query->group_condition;

	while (temp->next != NULL)
		temp = temp->next;

	if(temp->op == "")
		temp->op = lexeme;
	else
		temp->op += " " + lexeme;
}

bool updateRightCondition(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	CONDLIST * temp;

	if (query->group_grouping == NULL)
		temp = query->search_condition;
	else
		temp = query->group_condition;

	string tableName;
	vector<string> listOfTables;
	int i ;

	while (temp->next != NULL)
		temp = temp->next;
	
	for (i = 0; i < tables.size(); i++)
		listOfTables.push_back(tables[i][0]);

	vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, temp->column_name.c_str()));

	if (temp->column2->data2 != NULL)
		tableName = (*static_cast <string *>(temp->column2->data2));
	else
	{
		if (tablesWithColumn.size() == 0)
		{
			cout << "semantic error: column '" << temp->column_name << "' does not exist" << endl;

			return false;
		}
		else if (tablesWithColumn.size() > 1)
		{
			cout << "semantic error: ambiguous reference to '" << temp->column_name << "'" << endl;

			return false;
		}
	}
	if (token == "T_INT" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "int"))
	{
		int val;
		istringstream ss(lexeme);

		ss >> val;
		temp->value = new int(val);
		temp->type = "int";

		return true;
	}

	else if (token == "T_VARCHAR" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "varchar"))
	{
		temp->value = new string(lexeme.substr(1,lexeme.length()-2));
		temp->type = "varchar";
	
		return true;
	}

	else if (token == "T_FLOAT" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "float"))
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

bool updateRightList(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables)
{
	CONDLIST * temp;

	if (query->group_grouping == NULL)
		temp = query->search_condition;
	else
		temp = query->group_condition;

	string tableName;
	vector<string> listOfTables;
	int i ;

	while (temp->next != NULL)
		temp = temp->next;

	for (i = 0; i < tables.size(); i++)
		listOfTables.push_back(tables[i][0]);

	vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, temp->column_name.c_str()));
	
	if (temp->column2->data2 != NULL)
		tableName = (*static_cast <string *>(temp->column2->data2));
	else
	{
		if (tablesWithColumn.size() == 0)
		{
			cout << "semantic error: column '" << temp->column_name << "' does not exist" << endl;

			return false;
		}
		else if (tablesWithColumn.size() > 1)
		{
			cout << "semantic error: ambiguous reference to '" << temp->column_name << "'" << endl;

			return false;
		}
	}

	if (token == "T_INT" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "int"))
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
	else if (token == "T_VARCHAR" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "varchar"))
	{
		temp->type = "varchar";
		
		DATA * newData;

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
	else if (token == "T_FLOAT" && check_data_type(tablesWithColumn[0].c_str(), temp->column_name , "float"))
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

bool instantiateGroup(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns)
{
	if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');
		bool isDefined = false;

		for (i = 0; i < tables.size(); i++)
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
			if(check_column_name(tables[i][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
			{
				DATA * newData;

				newData = new DATA("varchar", new string(lexeme.substr(pos+1, lexeme.length()-pos-1)));

				newData->data2 = new string(tables[i][0]);
				if (query->group_grouping == NULL)
					query->group_grouping = newData;
				else
				{
					DATA * temp = query->group_grouping;
					while (temp->next != NULL)
						temp = temp->next;

					newData->prev = temp;
					temp->next = newData;
				}

				return true;
			}

			cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[i][0] << "'" << endl;
		}
		else
			cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;
	}
	else if (token == "T_IDENT")
	{
		string tableName;
		int count = 0;

		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[i][0] == lexeme)
			{
				tableName = columns[i][0];
				count++;
			}
		}

		if (count > 1)
			cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;		
		else if (count == 1)
		{
			DATA * newData;

			newData = new DATA("varchar", new string(lexeme));
			newData->data2 = new string(tableName);

			if (query->group_grouping == NULL)
				query->group_grouping = newData;
			else
			{
				DATA * temp = query->group_grouping;

				while (temp->next != NULL)
					temp = temp->next;

				newData->prev = temp;
				temp->next = newData;
			}

			return true;
		}
		else
		{
			vector<string> listOfTables;

			for (int i = 0; i < tables.size(); i++)
				listOfTables.push_back(tables[i][0]);

			vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, lexeme.c_str()));

			if (tablesWithColumn.size() == 0)
			{
				cout << "semantic error: column '" << lexeme << "' does not exist" << endl;
			}
			else if (tablesWithColumn.size() > 1)
			{
				cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;
			}
			else
			{
				DATA * newData;

				newData = new DATA("varchar", new string(lexeme));
				newData->data2 = new string(tablesWithColumn[0]);

				if (query->group_grouping == NULL)
					query->group_grouping = newData;
				else
				{
					DATA * temp = query->group_grouping;

					while (temp->next != NULL)
						temp = temp->next;

					newData->prev = temp;
					temp->next = newData;
				}

				return true;
			}
		}
	}

	return false;
}

bool instantiateOrder(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns)
{
	/*
	CONDLIST *test = query->search_condition;

	while (test != NULL)
	{
		DATA * testData = test->column1;

		cout << "op: "<< test->op << endl;
		cout << "column_name: " << test->column_name << endl;
		while (testData != NULL)
		{
			cout << "list :" << (*static_cast <int *>(testData->data)) << endl;
			testData = testData->next;
		}

		test = test->next;
	}
	*/

	if (token == "T_TABLECOLUMN")
	{
		int i;
		int pos = lexeme.find_first_of('.');
		bool isDefined = false;

		for (i = 0; i < tables.size(); i++)
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
			if(check_column_name(tables[i][0].c_str(), lexeme.substr(pos+1, lexeme.length()-pos-1).c_str()))
			{
				DATA * newData;

				newData = new DATA("varchar", new string(lexeme.substr(pos+1, lexeme.length()-pos-1)));

				newData->data2 = new string(tables[i][0]);
				if (query->order_grouping == NULL)
					query->order_grouping = newData;
				else
				{
					DATA * temp = query->order_grouping;
					while (temp->next != NULL)
						temp = temp->next;
					newData->prev = temp;
					temp->next = newData;
				}
				return true;
			}

			cout << "semantic error: column '" << lexeme.substr(pos+1, lexeme.length()-pos-1) << "' does not exist in table '" << tables[i][0] << "'" << endl;
		}
		else
			cout << "semantic error: table alias '" << lexeme.substr(0,pos) << "' is not defined" << endl;
	}
	else if (token == "T_IDENT")
	{
		string tableName;
		int count = 0;

		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[i][0] == lexeme)
			{
				tableName = columns[i][0];
				count++;
			}
		}

		if (count > 1)
			cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;		
		else if (count == 1)
		{
			DATA * newData;

			newData = new DATA("varchar", new string(lexeme));
			newData->data2 = new string(tableName);

			if (query->order_grouping == NULL)
				query->order_grouping = newData;
			else
			{
				DATA * temp = query->order_grouping;
				while (temp->next != NULL)
					temp = temp->next;
				newData->prev = temp;
				temp->next = newData;
			}
			return true;
		}
		else
		{
			vector<string> listOfTables;

			for (int i = 0; i < tables.size(); i++)
				listOfTables.push_back(tables[i][0]);

			vector<string> tablesWithColumn(get_tables_with_same_column_name(listOfTables, lexeme.c_str()));

			if (tablesWithColumn.size() == 0)
			{
				cout << "semantic error: column '" << lexeme << "' does not exist" << endl;
			}
			else if (tablesWithColumn.size() > 1)
			{
				cout << "semantic error: ambiguous reference to '" << lexeme << "'" << endl;
			}
			else
			{
				DATA * newData;

				newData = new DATA("varchar", new string(lexeme));
				newData->data2 = new string(tablesWithColumn[0]);

				if (query->order_grouping == NULL)
					query->order_grouping = newData;
				else
				{
					DATA * temp = query->order_grouping;

					while (temp->next != NULL)
						temp = temp->next;

					newData->prev = temp;
					temp->next = newData;
				}

				return true;
			}
		}
	}
	else if (token == "asc" || token == "desc")
	{
		DATA * newData;

		newData = new DATA("varchar", new string(lexeme));
		newData->data2 = NULL;
		query->order_grouping->down = newData;

		return true;
	}

	return false;
}

#endif