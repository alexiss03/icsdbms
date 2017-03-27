#ifndef DELETESYNTAX_HPP
#define DELETESYNTAX_HPP

void checkDeleteSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme)
{
	int j = 0;
	int popCounter;
	pair<int, int> action, LHS_RHS;
	bool halt = false;

	DELETEQUERYPARSE * query;
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

			action = deleteGetAction(input_state.top().second, token_lexeme[j].first.first, token_lexeme[j].second.second, query, token_lexeme[j].second.first, tables, columns, token_lexeme[j].first.second);

			switch (action.first)
			{
				case T_SHIFT: input_state.push(make_pair(token_lexeme[j].first.first, action.second));
					j++;
					break;
				case T_REDUCE: LHS_RHS = deleteGetLHS_RHS(action.second);
					for(popCounter = 0; popCounter < LHS_RHS.second; popCounter++)
						input_state.pop();
					input_state.push(make_pair(LHS_RHS.first, deleteGetGoTo(input_state.top().second, LHS_RHS.first)));
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

pair<int, int> deleteGetAction(int state, int input, string lexeme, DELETEQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, string range)
{
	int nextAction;
	int nextState_rule = -1;

	switch (state)
	{
		case 1: switch(input)
			{
				case DELETEQUERY: nextAction = T_SHIFT;
					nextState_rule = 2;
					break;
				case T_DELETE: nextAction = T_SHIFT;
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
					delete_op(*query);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 3: switch(input)
			{
				case T_FROM: nextAction = T_SHIFT;
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
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 8;
					instantiateTableAlias(tables, lexeme);
					break;
				case OPTIONAL_ALIAS: nextAction = T_SHIFT;
					nextState_rule = 7;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 7: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case OPTIONAL_WHERE: nextAction = T_SHIFT;
					nextState_rule = 9;
					break;
				case T_WHERE: nextAction = T_SHIFT;
					nextState_rule = 10;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 8: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 3;
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
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 13;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case SEARCH_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 11;
					break;
				case CONDVALUE: nextAction = T_SHIFT;
					nextState_rule = 12;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 14;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 11: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 12: switch(input)
			{
				case CONDITION: nextAction = T_SHIFT;
					nextState_rule = 15;
					break;
				case T_IS: nextAction = T_SHIFT;
					nextState_rule = 16;
					updateCompCondition(query, lexeme);
					break;
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 20;
					break;
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case EQUALITYOP1: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case EQUALITYOP2: nextAction = T_SHIFT;
					nextState_rule = 18;
					break;
				case T_LIKE: nextAction = T_SHIFT;
					nextState_rule = 19;
					updateCompCondition(query, lexeme);
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 27;
					updateCompCondition(query, lexeme);
					break;
				case T_EQL: nextAction = T_SHIFT;
					nextState_rule = 21;
					updateCompCondition(query, lexeme);
					break;
				case T_NEQ: nextAction = T_SHIFT;
					nextState_rule = 22;
					updateCompCondition(query, lexeme);
					break;
				case T_GEQ: nextAction = T_SHIFT;
					nextState_rule = 23;
					updateCompCondition(query, lexeme);
					break;
				case T_LEQ: nextAction = T_SHIFT;
					nextState_rule = 24;
					updateCompCondition(query, lexeme);
					break;
				case T_GTR: nextAction = T_SHIFT;
					nextState_rule = 25;
					updateCompCondition(query, lexeme);
					break;
				case T_LSS: nextAction = T_SHIFT;
					nextState_rule = 26;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 13: switch(input)
			{
				case T_IS: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_LIKE: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_NOT: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_EQL: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_NEQ: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_GEQ: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_LEQ: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_GTR: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_LSS: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 14: switch(input)
			{
				case T_IS: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_LIKE: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_NOT: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_EQL: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_NEQ: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_GEQ: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_LEQ: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_GTR: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_LSS: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 15: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case REPEATED_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 28;
					break;
				case LOGICALOP: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 30;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_OR: nextAction = T_SHIFT;
					nextState_rule = 31;
					if(!instantiateCondition(query, lexeme, token, tables))
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
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 32;
					break;
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 27;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 17: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 33;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 18: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 37;
					break;
				case GTR_LSS_RIGHT_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 36;
					break;
				case COUNT_COMP: nextAction = T_SHIFT;
					nextState_rule = 38;
					break;
				case T_ALL: nextAction = T_SHIFT;
					nextState_rule = 39;
					updateCompCondition(query, lexeme);
					break;
				case T_ANY: nextAction = T_SHIFT;
					nextState_rule = 40;
					updateCompCondition(query, lexeme);
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 19: switch(input)
			{
				case LIKE_RIGHT_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 41;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 42;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 20: switch(input)
			{
				case CONDITION_TYPE: nextAction = T_SHIFT;
					nextState_rule = 43;
					break;
				case T_BETWEEN: nextAction = T_SHIFT;
					nextState_rule = 44;
					updateCompCondition(query, lexeme);
					break;
				case T_IN: nextAction = T_SHIFT;
					nextState_rule = 45;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 21: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 27;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 27;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 22: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 28;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 28;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 23: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 29;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 24: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 30;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 30;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 25: switch(input)
			{
				case T_ALL: nextAction = T_REDUCE;
					nextState_rule = 31;
					break;
				case T_ANY: nextAction = T_REDUCE;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 31;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 31;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 26: switch(input)
			{
				case T_ALL: nextAction = T_REDUCE;
					nextState_rule = 32;
					break;
				case T_ANY: nextAction = T_REDUCE;
					nextState_rule = 32;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 32;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 32;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 27: switch(input)
			{
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 28: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 29: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 13;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case CONDVALUE: nextAction = T_SHIFT;
					nextState_rule = 46;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 14;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 30: switch(input)
			{
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 31: switch(input)
			{
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 32: switch(input)
			{
				case T_NULL: nextAction = T_SHIFT;
					nextState_rule = 47;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 33: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 34: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 35: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 36: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 37: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 38: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 48;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 39: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 25;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 40: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 26;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 41: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 42: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 33;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 33;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 33;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 43: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 44: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 49;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 45: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 50;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 46: switch(input)
			{
				case CONDITION: nextAction = T_SHIFT;
					nextState_rule = 51;
					break;
				case T_IS: nextAction = T_SHIFT;
					nextState_rule = 16;
					updateCompCondition(query, lexeme);
					break;
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 20;
					break;			
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case EQUALITYOP1: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case EQUALITYOP2: nextAction = T_SHIFT;
					nextState_rule = 18;
					break;
				case T_LIKE: nextAction = T_SHIFT;
					nextState_rule = 19;
					updateCompCondition(query, lexeme);
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 27;
					updateCompCondition(query, lexeme);
					break;
				case T_EQL: nextAction = T_SHIFT;
					nextState_rule = 21;
					updateCompCondition(query, lexeme);
					break;
				case T_NEQ: nextAction = T_SHIFT;
					nextState_rule = 22;
					updateCompCondition(query, lexeme);
					break;
				case T_GEQ: nextAction = T_SHIFT;
					nextState_rule = 23;
					updateCompCondition(query, lexeme);
					break;
				case T_LEQ: nextAction = T_SHIFT;
					nextState_rule = 24;
					updateCompCondition(query, lexeme);
					break;
				case T_GTR: nextAction = T_SHIFT;
					nextState_rule = 25;
					updateCompCondition(query, lexeme);
					break;
				case T_LSS: nextAction = T_SHIFT;
					nextState_rule = 26;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 47: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 48: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 53;
					break;
				case LISTED_NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 52;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 49: switch(input)
			{
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 54;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 50: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 53;
					break;
				case LIST_VALUE: nextAction = T_SHIFT;
					nextState_rule = 55;
					break;
				case LISTED_NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 56;
					break;
				case LISTED_STRING: nextAction = T_SHIFT;
					nextState_rule = 57;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 58;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 51: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case REPEATED_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 59;
					break;
				case LOGICALOP: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 30;
					if(!instantiateCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_OR: nextAction = T_SHIFT;
					nextState_rule = 31;
					if(!instantiateCondition(query, lexeme, token, tables))	
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 52: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 60;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 53: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case REPEATED_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 61;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 62;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 54: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 63;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 55: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 64;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 56: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 34;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 57: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 35;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 58: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 66;
					break;
				case REPEATED_STRING: nextAction = T_SHIFT;
					nextState_rule = 65;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 59: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 60: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 24;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 24;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 24;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 61: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 36;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 62: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 67;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 63: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 64: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 65: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 66: switch(input)
			{
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 68;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 67: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case REPEATED_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 69;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 62;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 68: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 66;
					break;
				case REPEATED_STRING: nextAction = T_SHIFT;
					nextState_rule = 70;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 69: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
		case 70: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);

			}
			break;
	}

	return make_pair(nextAction, nextState_rule);
}

pair<int, int> deleteGetLHS_RHS(int rule)
{
	int LHS;
	int RHS;
	
	switch (rule)
	{
		case 1: 
			break;
		case 2: LHS = DELETEQUERY;
			RHS = 5;
			break;
		case 3: LHS = OPTIONAL_ALIAS;
			RHS = 1;
			break;
		case 4: LHS = OPTIONAL_ALIAS;
			RHS = 0;
			break;
		case 5: LHS = OPTIONAL_WHERE;
			RHS = 2;
			break;
		case 6: LHS = OPTIONAL_WHERE;
			RHS = 0;
			break;
		case 7: LHS = SEARCH_CONDITION;
			RHS = 3;
			break;
		case 8: LHS = REPEATED_CONDITION;
			RHS = 4;
			break;
		case 9: LHS = REPEATED_CONDITION;
			RHS = 0;
			break;
		case 10: LHS = LOGICALOP;
			RHS = 1;
			break;
		case 11: LHS = LOGICALOP;
			RHS = 1;
			break;
		case 12: LHS = CONDITION;
			RHS = 3;
			break;
		case 13: LHS = CONDITION;
			RHS = 2;
			break;
		case 14: LHS = CONDITION;
			RHS = 2;
			break;
		case 15: LHS = CONDITION;
			RHS = 2;
			break;
		case 16: LHS = CONDITION;
			RHS = 2;
			break;
		case 17: LHS = CONDITION_TYPE;
			RHS = 4;
			break;
		case 18: LHS = CONDITION_TYPE;
			RHS = 4;
			break;
		case 19: LHS = CONDVALUE;
			RHS = 1;
			break;
		case 20: LHS = CONDVALUE;
			RHS = 1;
			break;
		case 21: LHS = OPTIONAL_NOT;
			RHS = 1;
			break;
		case 22: LHS = OPTIONAL_NOT;
			RHS = 0;
			break;
		case 23: LHS = GTR_LSS_RIGHT_CONDITION;
			RHS = 1;
			break;
		case 24: LHS = GTR_LSS_RIGHT_CONDITION;
			RHS = 4;
			break;
		case 25: LHS = COUNT_COMP;
			RHS = 1;
			break;
		case 26: LHS = COUNT_COMP;
			RHS = 1;
			break;
		case 27: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 28: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 29: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 30: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 31: LHS = EQUALITYOP2;
			RHS = 1;
			break;
		case 32: LHS = EQUALITYOP2;
			RHS = 1;
			break;
		case 33: LHS = LIKE_RIGHT_CONDITION;
			RHS = 1;
			break;
		case 34: LHS = LIST_VALUE;
			RHS = 1;
			break;
		case 35: LHS = LIST_VALUE;
			RHS = 1;
			break;
		case 36: LHS = LISTED_NUMBER_TYPE;
			RHS = 2;
			break;
		case 37: LHS = REPEATED_NUMBER;
			RHS = 3;
			break;
		case 38: LHS = REPEATED_NUMBER;
			RHS = 0;
			break;
		case 39: LHS = LISTED_STRING;
			RHS = 2;
			break;
		case 40: LHS = REPEATED_STRING;
			RHS = 3;
			break;
		case 41: LHS = REPEATED_STRING;
			RHS = 0;
			break;
		case 42: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
		case 43: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
	}

	return make_pair(LHS, RHS);
}

int deleteGetGoTo(int state, int input)
{
	int newState = -1;	

	switch (state)
	{
		case 1: switch(input)
			{
				case DELETEQUERY: newState = 2;
					break;
				case T_DELETE: newState = 3;
					break;
			}
			break;
		case 2: switch(input)
			{
			}
			break;
		case 3: switch(input)
			{
				case T_FROM: newState = 5;
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
				case T_IDENT: newState = 8;
					break;
				case OPTIONAL_ALIAS: newState = 7;
					break;
			}
			break;
		case 7: switch(input)
			{
				case OPTIONAL_WHERE: newState = 9;
					break;
				case T_WHERE: newState = 10;
					break;
			}
			break;
		case 8: switch(input)
			{
			}
			break;
		case 9: switch(input)
			{
			}
			break;
		case 10: switch(input)
			{
				case T_IDENT: newState = 13;
					break;
				case SEARCH_CONDITION: newState = 11;
					break;
				case CONDVALUE: newState = 12;
					break;
				case T_TABLECOLUMN: newState = 14;
					break;
			}
			break;
		case 11: switch(input)
			{
			}
			break;
		case 12: switch(input)
			{
				case CONDITION: newState = 15;
					break;
				case T_IS: newState = 16;
					break;
				case OPTIONAL_NOT: newState = 20;
					break;
				case EQUALITYOP1: newState = 17;
					break;
				case EQUALITYOP2: newState = 18;
					break;
				case T_LIKE: newState = 19;
					break;
				case T_NOT: newState = 27;
					break;
				case T_EQL: newState = 21;
					break;
				case T_NEQ: newState = 22;
					break;
				case T_GEQ: newState = 23;
					break;
				case T_LEQ: newState = 24;
					break;
				case T_GTR: newState = 25;
					break;
				case T_LSS: newState = 26;
					break;
			}
			break;
		case 13: switch(input)
			{
			}
			break;
		case 14: switch(input)
			{
			}
			break;
		case 15: switch(input)
			{
				case REPEATED_CONDITION: newState = 28;
					break;
				case LOGICALOP: newState = 29;
					break;
				case T_AND: newState = 30;
					break;
				case T_OR: newState = 31;
					break;
			}
			break;
		case 16: switch(input)
			{
				case OPTIONAL_NOT: newState = 32;
					break;
				case T_NOT: newState = 27;
					break;
			}
			break;
		case 17: switch(input)
			{
				case NUMBER_TYPE: newState = 33;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 18: switch(input)
			{
				case NUMBER_TYPE: newState = 37;
					break;
				case GTR_LSS_RIGHT_CONDITION: newState = 36;
					break;
				case COUNT_COMP: newState = 38;
					break;
				case T_ALL: newState = 39;
					break;
				case T_ANY: newState = 40;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 19: switch(input)
			{
				case LIKE_RIGHT_CONDITION: newState = 41;
					break;
				case T_VARCHAR: newState = 42;
					break;
			}
			break;
		case 20: switch(input)
			{
				case CONDITION_TYPE: newState = 43;
					break;
				case T_BETWEEN: newState = 44;
					break;
				case T_IN: newState = 45;
					break;
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
			}
			break;
		case 24: switch(input)
			{
			}
			break;
		case 25: switch(input)
			{
			}
			break;
		case 26: switch(input)
			{
			}
			break;
		case 27: switch(input)
			{
			}
			break;
		case 28: switch(input)
			{
			}
			break;
		case 29: switch(input)
			{
				case T_IDENT: newState = 13;
					break;
				case CONDVALUE: newState = 46;
					break;
				case T_TABLECOLUMN: newState = 14;
					break;
			}
			break;
		case 30: switch(input)
			{
			}
			break;
		case 31: switch(input)
			{
			}
			break;
		case 32: switch(input)
			{
				case T_NULL: newState = 47;
					break;
			}
			break;
		case 33: switch(input)
			{
			}
			break;
		case 34: switch(input)
			{
			}
			break;
		case 35: switch(input)
			{
			}
			break;
		case 36: switch(input)
			{
			}
			break;
		case 37: switch(input)
			{
			}
			break;
		case 38: switch(input)
			{
				case T_LPAREN: newState = 48;
					break;
			}
			break;
		case 39: switch(input)
			{
			}
			break;
		case 40: switch(input)
			{
			}
			break;
		case 41: switch(input)
			{
			}
			break;
		case 42: switch(input)
			{
			}
			break;
		case 43: switch(input)
			{
			}
			break;
		case 44: switch(input)
			{
				case NUMBER_TYPE: newState = 49;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 45: switch(input)
			{
				case T_LPAREN: newState = 50;
					break;
			}
			break;
		case 46: switch(input)
			{
				case CONDITION: newState = 51;
					break;
				case T_IS: newState = 16;
					break;
				case OPTIONAL_NOT: newState = 20;
					break;
				case EQUALITYOP1: newState = 17;
					break;
				case EQUALITYOP2: newState = 18;
					break;
				case T_LIKE: newState = 19;
					break;
				case T_NOT: newState = 27;
					break;
				case T_EQL: newState = 21;
					break;
				case T_NEQ: newState = 22;
					break;
				case T_GEQ: newState = 23;
					break;
				case T_LEQ: newState = 24;
					break;
				case T_GTR: newState = 25;
					break;
				case T_LSS: newState = 26;
					break;
			}
			break;
		case 47: switch(input)
			{
			}
			break;
		case 48: switch(input)
			{
				case NUMBER_TYPE: newState = 53;
					break;
				case LISTED_NUMBER_TYPE: newState = 52;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 49: switch(input)
			{
				case T_AND: newState = 54;
					break;
			}
			break;
		case 50: switch(input)
			{
				case NUMBER_TYPE: newState = 53;
					break;
				case LIST_VALUE: newState = 55;
					break;
				case LISTED_NUMBER_TYPE: newState = 56;
					break;
				case LISTED_STRING: newState = 57;
					break;
				case T_VARCHAR: newState = 58;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 51: switch(input)
			{
				case REPEATED_CONDITION: newState = 59;
					break;
				case LOGICALOP: newState = 29;
					break;
				case T_AND: newState = 30;
					break;
				case T_OR: newState = 31;
					break;
			}
			break;
		case 52: switch(input)
			{
				case T_RPAREN: newState = 60;
					break;
			}
			break;
		case 53: switch(input)
			{
				case REPEATED_NUMBER: newState = 61;
					break;
				case T_COMMA: newState = 62;
					break;
			}
			break;
		case 54: switch(input)
			{
				case NUMBER_TYPE: newState = 63;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 55: switch(input)
			{
				case T_RPAREN: newState = 64;
					break;
			}
			break;
		case 56: switch(input)
			{
			}
			break;
		case 57: switch(input)
			{
			}
			break;
		case 58: switch(input)
			{
				case T_COMMA: newState = 66;
					break;
				case REPEATED_STRING: newState = 65;
					break;
			}
			break;
		case 59: switch(input)
			{
			}
			break;
		case 60: switch(input)
			{
			}
			break;
		case 61: switch(input)
			{
			}
			break;
		case 62: switch(input)
			{
				case NUMBER_TYPE: newState = 67;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 63: switch(input)
			{
			}
			break;
		case 64: switch(input)
			{
			}
			break;
		case 65: switch(input)
			{
			}
			break;
		case 66: switch(input)
			{
				case T_VARCHAR: newState = 68;
					break;
			}
			break;
		case 67: switch(input)
			{
				case REPEATED_NUMBER: newState = 69;
					break;
				case T_COMMA: newState = 62;
					break;
			}
			break;
		case 68: switch(input)
			{
				case T_COMMA: newState = 66;
					break;
				case REPEATED_STRING: newState = 70;
					break;
			}
			break;
		case 69: switch(input)
			{
			}
			break;
		case 70: switch(input)
			{
			}
			break;
	}

	return newState;
}

void display_error(string message, string range)
{
	cout << range << ": " << "syntax error: unexpected '" << message << "' at location" << endl;
}

#endif
