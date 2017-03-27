#ifndef INSERTSYNTAX_HPP
#define INSERTSYNTAX_HPP

void checkInsertSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme)
{
	int j = 0;
	int popCounter;
	pair<int, int> action, LHS_RHS;
	bool halt = false;
	bool isSupported = true;

	INSERTQUERYPARSE * query;
	vector<vector<string> > tables, columns;

	stack< pair<int, int> > input_state;
	input_state.push(make_pair(-1, 1));

	ofstream file ("compiler/syntax_analyzer/casePath.txt");
	if (file.is_open())
	{
		do
		{
			file << "state: " << input_state.top().second << endl;
			file << "stack: " << input_state.top().first << endl;
			file << "tokens: " << token_lexeme[j].first.first << endl;

			action = insertGetAction(input_state.top().second, token_lexeme[j].first.first, token_lexeme[j].second.second, query, token_lexeme[j].second.first, tables, columns, token_lexeme[j].first.second, &isSupported);

			switch (action.first)
			{
				case T_SHIFT: input_state.push(make_pair(token_lexeme[j].first.first, action.second));
					j++;
					break;
				case T_REDUCE: LHS_RHS = insertGetLHS_RHS(action.second);
					for(popCounter = 0; popCounter < LHS_RHS.second; popCounter++)
						input_state.pop();
					input_state.push(make_pair(LHS_RHS.first, insertGetGoTo(input_state.top().second, LHS_RHS.first)));
					break;
				case T_ACCEPT:
				case T_REJECT: 
					halt = true;
					break;
			}

			file << "action: " << action.first << endl;
			file << "action #: " << action.second << endl;
			file << "---------------------------" << endl;
		} while (!halt);

		file.close();
	}

	token_lexeme.clear();
}

pair<int, int> insertGetAction(int state, int input, string lexeme, INSERTQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, string range, bool *isSupported)
{
	int nextAction;
	int nextState_rule = -1;

	switch (state)
	{
		case 1: switch(input)
			{
				case INSERTQUERY: nextAction = T_SHIFT;
					nextState_rule = 2;
					break;
				case T_INSERT: nextAction = T_SHIFT;
					nextState_rule = 3;
					instantiateQuery(query);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 2: switch(input)
			{
				case T_SEMICOLON: nextAction = T_ACCEPT;
					if (*isSupported)
						insert_op(*query);
					else
						cout << "version warning: multiple insert not supported" << endl;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 3: switch(input)
			{
				case T_INTO: nextAction = T_SHIFT;
					nextState_rule = 5;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 4: switch(input)
			{
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 5: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 6;
					if(!instantiateTable(query, lexeme, tables))
					{
						nextAction = T_REJECT;
					}					
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 6: switch(input)
			{
				case COLUMN_LIST: nextAction = T_SHIFT;
					nextState_rule = 7;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 8;
					break;
				case T_VALUES: nextAction = T_REDUCE;
					nextState_rule = 4;
					autoInstantiateColumn(query, tables);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 7: switch(input)
			{
				case INSERT_TYPE: nextAction = T_SHIFT;
					nextState_rule = 9;
					break;
				case T_VALUES: nextAction = T_SHIFT;
					updateInsertType(query, lexeme);
					nextState_rule = 10;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 8: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 11;
					if(!instantiateColumn(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 9: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 2;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 10: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 13;
					break;
				case VALUE_LIST: nextAction = T_SHIFT;
					nextState_rule = 12;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 11: switch(input)
			{
				case REPEATED_COLUMN_LIST: nextAction = T_SHIFT;
					nextState_rule = 14;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 15;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 12: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 13: switch(input)
			{
				case VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 16;
					break;
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 18;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_NULL: nextAction = T_SHIFT;
					nextState_rule = 19;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 20;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 21;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 14: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 22;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 15: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 23;
					if(!instantiateColumn(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 16: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 24;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 17: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 18: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 19: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 20: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 21: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 22: switch(input)
			{
				case T_VALUES: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 23: switch(input)
			{
				case REPEATED_COLUMN_LIST: nextAction = T_SHIFT;
					nextState_rule = 26;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 15;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 24: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 27;
					if(!checkCount(query))
					{
						nextAction = T_REJECT;
					}
					else if(!checkNotNullableColumns(query, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 25: switch(input)
			{
				case VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 28;
					break;
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 18;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_NULL: nextAction = T_SHIFT;
					nextState_rule = 19;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 20;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 21;
					if(*isSupported && (!updateValue(query, lexeme, token, tables)))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 26: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 27: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 30;
					*isSupported = false;
					break;
				case REPEATED_VALUE_LIST: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 28: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 29: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 30: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 32;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 31: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 32: switch(input)
			{
				case VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 33;
					break;
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 18;
					break;
				case T_NULL: nextAction = T_SHIFT;
					nextState_rule = 19;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 20;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 21;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 33: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: nextAction = T_SHIFT;
					nextState_rule = 34;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 34: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 35;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 35: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 30;
					break;
				case REPEATED_VALUE_LIST: nextAction = T_SHIFT;
					nextState_rule = 36;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 36: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
	}

	return make_pair(nextAction, nextState_rule);
}

pair<int, int> insertGetLHS_RHS(int rule)
{
	int LHS;
	int RHS;
	
	switch (rule)
	{
		case 1: 
			break;
		case 2: LHS = INSERTQUERY;
			RHS = 5;
			break;
		case 3: LHS = COLUMN_LIST;
			RHS = 4;
			break;
		case 4: LHS = COLUMN_LIST;
			RHS = 0;
			break;
		case 5: LHS = REPEATED_COLUMN_LIST;
			RHS = 3;
			break;
		case 6: LHS = REPEATED_COLUMN_LIST;
			RHS = 0;
			break;
		case 7: LHS = INSERT_TYPE;
			RHS = 2;
			break;
		case 8: LHS = VALUE_LIST;
			RHS = 5;
			break;
		case 9: LHS = VALUE_LIST_TYPE;
			RHS = 1;
			break;
		case 10: LHS = VALUE_LIST_TYPE;
			RHS = 1;
			break;
		case 11: LHS = VALUE_LIST_TYPE;
			RHS = 1;
			break;
		case 12: LHS = REPEATED_VALUE_LIST;
			RHS = 6;
			break;
		case 13: LHS = REPEATED_VALUE_LIST;
			RHS = 0;
			break;
		case 14: LHS = REPEATED_VALUE_LIST_TYPE;
			RHS = 3;
			break;
		case 15: LHS = REPEATED_VALUE_LIST_TYPE;
			RHS = 0;
			break;
		case 16: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
		case 17: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
	}

	return make_pair(LHS, RHS);
}

int insertGetGoTo(int state, int input)
{
	int newState = -1;	

	switch (state)
	{
		case 1: switch(input)
			{
				case INSERTQUERY: newState = 2;
					break;
				case T_INSERT: newState = 3;
					break;
			}
			break;
		case 2: switch(input)
			{
			}
			break;
		case 3: switch(input)
			{
				case T_INTO: newState = 5;
					break;
			}
			break;
		case 4: switch(input)
			{
			}
			break;
		case 5: switch(input)
			{
				case T_IDENT: newState = 6;
					break;
			}
			break;
		case 6: switch(input)
			{
				case COLUMN_LIST: newState = 7;
					break;
				case T_LPAREN: newState = 8;
					break;
			}
			break;
		case 7: switch(input)
			{
				case INSERT_TYPE: newState = 9;
					break;
				case T_VALUES: newState = 10;
					break;
			}
			break;
		case 8: switch(input)
			{
				case T_IDENT: newState = 11;
					break;
			}
			break;
		case 9: switch(input)
			{
			}
			break;
		case 10: switch(input)
			{
				case T_LPAREN: newState = 13;
					break;
				case VALUE_LIST: newState = 12;
					break;
			}
			break;
		case 11: switch(input)
			{
				case REPEATED_COLUMN_LIST: newState = 14;
					break;
				case T_COMMA: newState = 15;
					break;
			}
			break;
		case 12: switch(input)
			{
			}
			break;
		case 13: switch(input)
			{
				case VALUE_LIST_TYPE: newState = 16;
					break;
				case NUMBER_TYPE: newState = 17;
					break;
				case T_VARCHAR: newState = 18;
					break;
				case T_NULL: newState = 19;
					break;
				case T_FLOAT: newState = 20;
					break;
				case T_INT: newState = 21;
					break;
			}
			break;
		case 14: switch(input)
			{
				case T_RPAREN: newState = 22;
					break;
			}
			break;
		case 15: switch(input)
			{
				case T_IDENT: newState = 23;
					break;
			}
			break;
		case 16: switch(input)
			{
				case T_COMMA: newState = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: newState = 24;
					break;
			}
			break;
		case 17: switch(input)
			{
			}
			break;
		case 18: switch(input)
			{
			}
			break;
		case 19: switch(input)
			{
			}
			break;
		case 20: switch(input)
			{
			}
			break;
		case 21: switch(input)
			{
			}
			break;
		case 22: switch(input)
			{
			}
			break;
		case 23: switch(input)
			{
				case REPEATED_COLUMN_LIST: newState = 26;
					break;
				case T_COMMA: newState = 15;
					break;
			}
			break;
		case 24: switch(input)
			{
				case T_RPAREN: newState = 27;
					break;
			}
			break;
		case 25: switch(input)
			{
				case VALUE_LIST_TYPE: newState = 28;
					break;
				case NUMBER_TYPE: newState = 17;
					break;
				case T_VARCHAR: newState = 18;
					break;
				case T_NULL: newState = 19;
					break;
				case T_FLOAT: newState = 20;
					break;
				case T_INT: newState = 21;
					break;
			}
			break;
		case 26: switch(input)
			{
			}
			break;
		case 27: switch(input)
			{
				case T_COMMA: newState = 30;
					break;
				case REPEATED_VALUE_LIST: newState = 29;
					break;
			}
			break;
		case 28: switch(input)
			{
				case T_COMMA: newState = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: newState = 31;
					break;
			}
			break;
		case 29: switch(input)
			{
			}
			break;
		case 30: switch(input)
			{
				case T_LPAREN: newState = 32;
					break;
			}
			break;
		case 31: switch(input)
			{
			}
			break;
		case 32: switch(input)
			{
				case VALUE_LIST_TYPE: newState = 33;
					break;
				case NUMBER_TYPE: newState = 17;
					break;
				case T_VARCHAR: newState = 18;
					break;
				case T_NULL: newState = 19;
					break;
				case T_FLOAT: newState = 20;
					break;
				case T_INT: newState = 21;
					break;
			}
			break;
		case 33: switch(input)
			{
				case T_COMMA: newState = 25;
					break;
				case REPEATED_VALUE_LIST_TYPE: newState = 34;
					break;
			}
			break;
		case 34: switch(input)
			{
				case T_RPAREN: newState = 35;
					break;
			}
			break;
		case 35: switch(input)
			{
				case T_COMMA: newState = 30;
					break;
				case REPEATED_VALUE_LIST: newState = 36;
					break;
			}
			break;
		case 36: switch(input)
			{
			}
			break;
	}

	return newState;
}

#endif