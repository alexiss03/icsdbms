#ifndef SELECTSYNTAX_HPP
#define SELECTSYNTAX_HPP

void checkSelectSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme)
{
	int j = 0;
	int popCounter;
	pair<int, int> action, LHS_RHS;
	bool halt = false;
	bool isSupported = true;

	SELECTQUERYPARSE * query;
	AST * curEquation;
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

			action = selectGetAction(input_state.top().second, token_lexeme[j].first.first, token_lexeme[j].second.second, query, token_lexeme[j].second.first, tables, columns, curEquation, token_lexeme[j].first.second, &isSupported);

			switch (action.first)
			{
				case T_SHIFT: input_state.push(make_pair(token_lexeme[j].first.first, action.second));
					j++;
					break;
				case T_REDUCE: LHS_RHS = selectGetLHS_RHS(action.second);
					for(popCounter = 0; popCounter < LHS_RHS.second; popCounter++)
						input_state.pop();
					input_state.push(make_pair(LHS_RHS.first, selectGetGoTo(input_state.top().second, LHS_RHS.first)));
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

pair<int, int> selectGetAction(int state, int input, string lexeme, SELECTQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, AST *& curEquation, string range, bool *isSupported)
{
	int nextAction;
	int nextState_rule = -1;

	switch (state)
	{
		case 1: switch(input)
			{
				case SELECTQUERY: nextAction = T_SHIFT;
					nextState_rule = 2;
					break;
				case T_SELECT: nextAction = T_SHIFT;
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
						select_op(*query);
					else
						cout << "version warning: round, to_number, arithmetic and subquery not supported" << endl;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 3: switch(input)
			{
				case OPTIONAL_DISPLAY: nextAction = T_SHIFT;
					nextState_rule = 5;
					break;
				case T_DISTINCT: nextAction = T_SHIFT;
					nextState_rule = 6;
					instantiateDisplay(query, lexeme, token);
					break;
				case T_TOP: nextAction = T_SHIFT;
					nextState_rule = 7;
					instantiateDisplay(query, lexeme, token);
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 5;
					break;
				case T_SUM: nextAction = T_REDUCE;
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
				case COLUMN_LIST: nextAction = T_SHIFT;
					nextState_rule = 8;
					break;
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					*isSupported = false;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					*isSupported = false;
					break;
				case COLUMN: nextAction = T_SHIFT;
					nextState_rule = 9;
					break;
				case T_TIMES: nextAction = T_SHIFT;
					nextState_rule = 10;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_TABLETIMES: nextAction = T_SHIFT;
					nextState_rule = 11;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case EXPRESSION: nextAction = T_SHIFT;
					nextState_rule = 12;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 32;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case FUNCTION: nextAction = T_SHIFT;
					nextState_rule = 13;
					break;
				case GROUP_FUNCTION: nextAction = T_SHIFT;
					nextState_rule = 14;
					break;
				case ARITHMETIC: nextAction = T_SHIFT;
					nextState_rule = 15;
					break;
				case ROUND: nextAction = T_SHIFT;
					nextState_rule = 16;
					break;
				case TO_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case T_ROUND: nextAction = T_SHIFT;
					nextState_rule = 20;
					*isSupported = false;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 30;
					*isSupported = false;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 33;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_TONUMBER: nextAction = T_SHIFT;
					nextState_rule = 21;
					*isSupported = false;
					break;
				case GROUP_FUNCTION_TYPE: nextAction = T_SHIFT;
					nextState_rule = 18;
					break;
				case T_AVG: nextAction = T_SHIFT;
					nextState_rule = 22;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_COUNT: nextAction = T_SHIFT;
					nextState_rule = 23;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_FIRST: nextAction = T_SHIFT;
					nextState_rule = 24;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_LAST: nextAction = T_SHIFT;
					nextState_rule = 25;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_MIN: nextAction = T_SHIFT;
					nextState_rule = 26;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_MAX: nextAction = T_SHIFT;
					nextState_rule = 27;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_SUM: nextAction = T_SHIFT;
					nextState_rule = 28;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case TERM: nextAction = T_SHIFT;
					nextState_rule = 19;
					break;
				case FACTOR: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 6: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 3;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 7: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 36;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					instantiateDisplay(query, lexeme, token);
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					instantiateDisplay(query, lexeme, token);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 8: switch(input)
			{
				case T_FROM: nextAction = T_SHIFT;
					nextState_rule = 37;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 9: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case REPEATED_COLUMN: nextAction = T_SHIFT;
					nextState_rule = 38;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 39;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 10: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 11;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 11: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 12;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 12: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				case OPTIONAL_HEADER: nextAction = T_SHIFT;
					nextState_rule = 40;
					break;
				case T_AS: nextAction = T_SHIFT;
					nextState_rule = 41;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 15;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 13: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 18;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 14: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 19;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 15: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 20;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 16: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 21;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 17: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 22;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 18: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 42;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 19: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case ARITHMETIC1: nextAction = T_SHIFT;
					nextState_rule = 43;
					break;
				case ARITHMETICOP1: nextAction = T_SHIFT;
					nextState_rule = 44;
					break;
				case T_PLUS: nextAction = T_SHIFT;
					nextState_rule = 45;
					*isSupported = false;
					break;
				case T_MINUS: nextAction = T_SHIFT;
					nextState_rule = 46;
					*isSupported = false;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 20: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 47;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 21: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 48;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 22: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 28;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 23: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 24: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 30;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 25: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 31;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 26: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 32;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 27: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 33;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 28: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 34;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 29: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_MODULO: nextAction = T_SHIFT;
					nextState_rule = 53;
					*isSupported = false;
					break;
				case T_TIMES: nextAction = T_SHIFT;
					nextState_rule = 51;
					*isSupported = false;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case TERM1: nextAction = T_SHIFT;
					nextState_rule = 49;
					break;
				case ARITHMETICOP2: nextAction = T_SHIFT;
					nextState_rule = 50;
					break;
				case T_DIVIDE: nextAction = T_SHIFT;
					nextState_rule = 52;
					*isSupported = false;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 30: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 32;
					break;
				case ARITHMETIC: nextAction = T_SHIFT;
					nextState_rule = 54;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 30;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 33;
					break;
				case TERM: nextAction = T_SHIFT;
					nextState_rule = 19;
					break;
				case FACTOR: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 31: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 47;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 32: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 48;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 33: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 49;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 34: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 6;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 35: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 7;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 36: switch(input)
			{
				case OPTIONAL_PERCENT: nextAction = T_SHIFT;
					nextState_rule = 55;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_MODULO: nextAction = T_SHIFT;
					nextState_rule = 56;
					instantiateDisplay(query, lexeme, token);
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 9;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 37: switch(input)
			{
				case TABLEE: nextAction = T_SHIFT;
					nextState_rule = 57;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 58;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 60;
					*isSupported = false;
					break;
				case SUBQUERY: nextAction = T_SHIFT;
					nextState_rule = 59;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 38: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 10;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 39: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					*isSupported = false;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					*isSupported = false;
					break;
				case COLUMN: nextAction = T_SHIFT;
					nextState_rule = 61;
					break;
				case T_TIMES: nextAction = T_SHIFT;
					nextState_rule = 10;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_TABLETIMES: nextAction = T_SHIFT;
					nextState_rule = 11;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case EXPRESSION: nextAction = T_SHIFT;
					nextState_rule = 12;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 32;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case FUNCTION: nextAction = T_SHIFT;
					nextState_rule = 13;
					break;
				case GROUP_FUNCTION: nextAction = T_SHIFT;
					nextState_rule = 14;
					break;
				case ARITHMETIC: nextAction = T_SHIFT;
					nextState_rule = 15;
					break;
				case ROUND: nextAction = T_SHIFT;
					nextState_rule = 16;
					break;
				case TO_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 17;
					break;
				case T_ROUND: nextAction = T_SHIFT;
					nextState_rule = 20;
					*isSupported = false;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 30;
					*isSupported = false;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 33;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_TONUMBER: nextAction = T_SHIFT;
					nextState_rule = 21;
					*isSupported = false;
					break;
				case GROUP_FUNCTION_TYPE: nextAction = T_SHIFT;
					nextState_rule = 18;
					break;
				case T_AVG: nextAction = T_SHIFT;
					nextState_rule = 22;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_COUNT: nextAction = T_SHIFT;
					nextState_rule = 23;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_FIRST: nextAction = T_SHIFT;
					nextState_rule = 24;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_LAST: nextAction = T_SHIFT;
					nextState_rule = 25;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_MIN: nextAction = T_SHIFT;
					nextState_rule = 26;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_MAX: nextAction = T_SHIFT;
					nextState_rule = 27;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case T_SUM: nextAction = T_SHIFT;
					nextState_rule = 28;
					instantiateColumn(query, lexeme, token, curEquation, columns);
					break;
				case TERM: nextAction = T_SHIFT;
					nextState_rule = 19;
					break;
				case FACTOR: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 40: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 13;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 41: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 62;
					instantiateColumnAlias(query, columns, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 42: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 64;
					updateColumn(query, lexeme, token, columns);
					break;
				case SOURCE: nextAction = T_SHIFT;
					nextState_rule = 63;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 65;
					updateColumn(query, lexeme, token, columns);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 43: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 35;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 35;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 35;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 35;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 44: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 32;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 30;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 33;
					break;
				case TERM: nextAction = T_SHIFT;
					nextState_rule = 66;
					break;
				case FACTOR: nextAction = T_SHIFT;
					nextState_rule = 29;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 45: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 38;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 46: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 39;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 47: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 64;
					break;
				case SOURCE: nextAction = T_SHIFT;
					nextState_rule = 67;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 65;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 48: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 64;
					break;
				case SOURCE: nextAction = T_SHIFT;
					nextState_rule = 68;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 65;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 49: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 40;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 50: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 31;
					break;
				case T_FLOAT: nextAction = T_SHIFT;
					nextState_rule = 34;
					break;
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 35;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 32;
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 30;
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 33;
					break;
				case FACTOR: nextAction = T_SHIFT;
					nextState_rule = 69;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 51: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 43;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 52: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 44;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 44;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 44;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 44;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 44;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 53: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 45;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 45;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 45;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 45;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 45;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 54: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 70;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 55: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 4;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 56: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_TABLETIMES: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_ROUND: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_TONUMBER: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_AVG: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_COUNT: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_FIRST: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_LAST: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_MIN: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_MAX: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				case T_SUM: nextAction = T_REDUCE;
					nextState_rule = 8;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 57: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				case OPTIONAL_JOIN: nextAction = T_SHIFT;
					nextState_rule = 71;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				case JOIN_TYPE: nextAction = T_SHIFT;
					nextState_rule = 72;
					break;
				case T_CROSS: nextAction = T_SHIFT;
					nextState_rule = 73;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_INNER: nextAction = T_SHIFT;
					nextState_rule = 74;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case OUTER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 75;
					break;
				case T_LEFT: nextAction = T_SHIFT;
					nextState_rule = 76;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_RIGHT: nextAction = T_SHIFT;
					nextState_rule = 77;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_FULL: nextAction = T_SHIFT;
					nextState_rule = 78;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 57;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 58: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 80;
					instantiateTableAlias(tables, lexeme);
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case OPTIONAL_ALIAS: nextAction = T_SHIFT;
					nextState_rule = 79;
					break;
				case T_ON: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_CROSS: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_INNER: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_LEFT: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_RIGHT: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_FULL: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 53;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 59: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 81;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 60: switch(input)
			{
				case SELECTQUERY: nextAction = T_SHIFT;
					nextState_rule = 82;
					break;
				case T_SELECT: nextAction = T_SHIFT;
					nextState_rule = 3;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 61: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 17;
					break;
				case REPEATED_COLUMN: nextAction = T_SHIFT;
					nextState_rule = 83;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 39;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 62: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 14;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 63: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 84;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 64: switch(input)
			{
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 24;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 24;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 65: switch(input)
			{
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 25;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 25;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 66: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 37;
					break;
				case ARITHMETIC1: nextAction = T_SHIFT;
					nextState_rule = 85;
					break;
				case ARITHMETICOP1: nextAction = T_SHIFT;
					nextState_rule = 44;
					break;
				case T_PLUS: nextAction = T_SHIFT;
					nextState_rule = 45;
					*isSupported = false;
					break;
				case T_MINUS: nextAction = T_SHIFT;
					nextState_rule = 46;
					*isSupported = false;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 67: switch(input)
			{
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 86;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 68: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 87;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 69: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_MODULO: nextAction = T_SHIFT;
					nextState_rule = 53;
					break;
				case T_TIMES: nextAction = T_SHIFT;
					nextState_rule = 51;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 42;
					break;
				case TERM1: nextAction = T_SHIFT;
					nextState_rule = 88;
					break;
				case ARITHMETICOP2: nextAction = T_SHIFT;
					nextState_rule = 50;
					break;
				case T_DIVIDE: nextAction = T_SHIFT;
					nextState_rule = 52;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 70: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_MODULO: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_TIMES: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				case T_DIVIDE: nextAction = T_REDUCE;
					nextState_rule = 46;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 71: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 70;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case OPTIONAL_WHERE: nextAction = T_SHIFT;
					nextState_rule = 89;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 70;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_WHERE: nextAction = T_SHIFT;
					nextState_rule = 90;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 70;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 70;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 70;
					if(!checkColumns(query, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 72: switch(input)
			{
				case T_JOIN: nextAction = T_SHIFT;
					nextState_rule = 91;
					updateTable(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 73: switch(input)
			{
				case T_JOIN: nextAction = T_SHIFT;
					nextState_rule = 92;
					updateTable(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 74: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 58;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 75: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 61;
					break;
				case OPTIONAL_OUTER: nextAction = T_SHIFT;
					nextState_rule = 93;
					break;
				case T_OUTER: nextAction = T_SHIFT;
					nextState_rule = 94;
					updateTable(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 76: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 62;
					break;
				case T_OUTER: nextAction = T_REDUCE;
					nextState_rule = 62;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 77: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 63;
					break;
				case T_OUTER: nextAction = T_REDUCE;
					nextState_rule = 63;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 78: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 64;
					break;
				case T_OUTER: nextAction = T_REDUCE;
					nextState_rule = 64;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 79: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_ON: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_CROSS: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_INNER: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_LEFT: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_RIGHT: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_FULL: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 50;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 80: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_ON: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_CROSS: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_INNER: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_LEFT: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_RIGHT: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_FULL: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 52;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 81: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_ON: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_CROSS: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_INNER: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_LEFT: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_RIGHT: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_FULL: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 51;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 82: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 95;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 83: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 16;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 84: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 27;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 27;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 27;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 85: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 36;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 36;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 36;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 36;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 86: switch(input)
			{
				case T_INT: nextAction = T_SHIFT;
					nextState_rule = 96;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 87: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 26;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 26;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 26;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 88: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_PLUS: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				case T_MINUS: nextAction = T_REDUCE;
					nextState_rule = 41;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 89: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 107;
					break;
				case OPTIONAL_GROUP: nextAction = T_SHIFT;
					nextState_rule = 97;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 107;
					break;
				case T_GROUP: nextAction = T_SHIFT;
					nextState_rule = 98;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 107;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 107;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 90: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 101;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 102;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case SEARCH_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 99;
					break;
				case CONDVALUE: nextAction = T_SHIFT;
					nextState_rule = 100;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 91: switch(input)
			{
				case TABLEE: nextAction = T_SHIFT;
					nextState_rule = 103;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 58;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 60;
					isSupported = false;
					break;
				case SUBQUERY: nextAction = T_SHIFT;
					nextState_rule = 59;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 92: switch(input)
			{
				case TABLEE: nextAction = T_SHIFT;
					nextState_rule = 104;
					break;
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 58;
					if(!instantiateTable(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 60;
					*isSupported = false;
					break;
				case SUBQUERY: nextAction = T_SHIFT;
					nextState_rule = 59;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 93: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 59;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 94: switch(input)
			{
				case T_JOIN: nextAction = T_REDUCE;
					nextState_rule = 60;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 95: switch(input)
			{
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 54;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 96: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 105;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 97: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 113;
					break;
				case OPTIONAL_HAVING: nextAction = T_SHIFT;
					nextState_rule = 106;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 113;
					break;
				case T_HAVING: nextAction = T_SHIFT;
					nextState_rule = 107;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 113;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 98: switch(input)
			{
				case T_BY: nextAction = T_SHIFT;
					nextState_rule = 108;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 99: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 69;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 69;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 69;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 69;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 69;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 100: switch(input)
			{
				case T_EQL: nextAction = T_SHIFT;
					nextState_rule = 115;
					updateCompCondition(query, lexeme);
					break;
				case CONDITION: nextAction = T_SHIFT;
					nextState_rule = 109;
					break;
				case T_IS: nextAction = T_SHIFT;
					nextState_rule = 110;
					updateCompCondition(query, lexeme);
					break;
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 114;
					break;
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case EQUALITYOP1: nextAction = T_SHIFT;
					nextState_rule = 111;
					break;
				case EQUALITYOP2: nextAction = T_SHIFT;
					nextState_rule = 112;
					break;
				case T_LIKE: nextAction = T_SHIFT;
					nextState_rule = 113;
					updateCompCondition(query, lexeme);
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 121;
					updateCompCondition(query, lexeme);
					break;
				case T_NEQ: nextAction = T_SHIFT;
					nextState_rule = 116;
					updateCompCondition(query, lexeme);
					break;
				case T_GEQ: nextAction = T_SHIFT;
					nextState_rule = 117;
					updateCompCondition(query, lexeme);
					break;
				case T_LEQ: nextAction = T_SHIFT;
					nextState_rule = 118;
					updateCompCondition(query, lexeme);
					break;
				case T_GTR: nextAction = T_SHIFT;
					nextState_rule = 119;
					updateCompCondition(query, lexeme);
					break;
				case T_LSS: nextAction = T_SHIFT;
					nextState_rule = 120;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 101: switch(input)
			{
				case T_EQL: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_IS: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_LIKE: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_NOT: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_NEQ: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_GEQ: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_LEQ: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_GTR: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				case T_LSS: nextAction = T_REDUCE;
					nextState_rule = 83;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 102: switch(input)
			{
				case T_EQL: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_IS: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_LIKE: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_NOT: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_NEQ: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_GEQ: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_LEQ: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_GTR: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				case T_LSS: nextAction = T_REDUCE;
					nextState_rule = 84;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 103: switch(input)
			{
				case T_ON: nextAction = T_SHIFT;
					nextState_rule = 122;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 104: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 56;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 105: switch(input)
			{
				case T_FROM: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				case T_AS: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 23;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 106: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 115;
					break;
				case OPTIONAL_ORDER: nextAction = T_SHIFT;
					nextState_rule = 123;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 115;
					break;
				case T_ORDER: nextAction = T_SHIFT;
					nextState_rule = 124;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 107: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 101;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 102;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case SEARCH_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 125;
					break;
				case CONDVALUE: nextAction = T_SHIFT;
					nextState_rule = 100;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 108: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 127;
					if(!instantiateGroup(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 128;
					if(!instantiateGroup(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case GROUPING: nextAction = T_SHIFT;
					nextState_rule = 126;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 109: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 131;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case REPEATED_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 129;
					break;
				case LOGICALOP: nextAction = T_SHIFT;
					nextState_rule = 130;
					break;
				case T_OR: nextAction = T_SHIFT;
					nextState_rule = 132;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 110: switch(input)
			{
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 133;
					break;
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 121;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 111: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 134;
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
		case 112: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 136;
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
				case GTR_LSS_RIGHT_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 135;
					break;
				case COUNT_COMP: nextAction = T_SHIFT;
					nextState_rule = 137;
					break;
				case T_ALL: nextAction = T_SHIFT;
					nextState_rule = 138;
					updateCompCondition(query, lexeme);
					break;
				case T_ANY: nextAction = T_SHIFT;
					nextState_rule = 139;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 113: switch(input)
			{
				case LIKE_RIGHT_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 140;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 141;
					if(!updateRightCondition(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 114: switch(input)
			{
				case CONDITION_TYPE: nextAction = T_SHIFT;
					nextState_rule = 142;
					break;
				case T_BETWEEN: nextAction = T_SHIFT;
					nextState_rule = 143;
					updateCompCondition(query, lexeme);
					break;
				case T_IN: nextAction = T_SHIFT;
					nextState_rule = 144;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 115: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 91;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 91;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 116: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 92;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 92;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 117: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 93;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 93;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 118: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 94;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 94;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 119: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 95;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 95;
					break;
				case T_ALL: nextAction = T_REDUCE;
					nextState_rule = 95;
					break;
				case T_ANY: nextAction = T_REDUCE;
					nextState_rule = 95;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 120: switch(input)
			{
				case T_FLOAT: nextAction = T_REDUCE;
					nextState_rule = 96;
					break;
				case T_INT: nextAction = T_REDUCE;
					nextState_rule = 96;
					break;
				case T_ALL: nextAction = T_REDUCE;
					nextState_rule = 96;
					break;
				case T_ANY: nextAction = T_REDUCE;
					nextState_rule = 96;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 121: switch(input)
			{
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 85;
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 85;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 85;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 122: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 146;
					break;
				case JOIN_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 145;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 123: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 2;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 2;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 124: switch(input)
			{
				case T_BY: nextAction = T_SHIFT;
					nextState_rule = 147;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 125: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 112;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 112;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 112;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 126: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 149;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case REPEATED_GROUPING: nextAction = T_SHIFT;
					nextState_rule = 148;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 127: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 108;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 108;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 108;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 108;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 108;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 128: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 109;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 109;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 109;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 109;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 109;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 129: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 71;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 71;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 71;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 71;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 71;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 130: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 101;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 102;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case CONDVALUE: nextAction = T_SHIFT;
					nextState_rule = 150;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 131: switch(input)
			{
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 74;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 74;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 132: switch(input)
			{
				case T_IDENT: nextAction = T_REDUCE;
					nextState_rule = 75;
					break;
				case T_TABLECOLUMN: nextAction = T_REDUCE;
					nextState_rule = 75;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 133: switch(input)
			{
				case T_NULL: nextAction = T_SHIFT;
					nextState_rule = 151;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 134: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 77;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 135: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 78;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 136: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 87;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 137: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 152;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 138: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 89;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 139: switch(input)
			{
				case T_LPAREN: nextAction = T_REDUCE;
					nextState_rule = 90;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 140: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 79;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 141: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 97;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 142: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 80;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 143: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 153;
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
		case 144: switch(input)
			{
				case T_LPAREN: nextAction = T_SHIFT;
					nextState_rule = 154;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 145: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 55;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 146: switch(input)
			{
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 156;
					if(!updateOn(query, lexeme,tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case COLUMN_PAIR: nextAction = T_SHIFT;
					nextState_rule = 155;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 147: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 158;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 159;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case ORDERING: nextAction = T_SHIFT;
					nextState_rule = 157;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 148: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 106;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 106;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 106;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 106;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 149: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 127;
					if(!instantiateGroup(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 128;
					if(!instantiateGroup(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case GROUPING: nextAction = T_SHIFT;
					nextState_rule = 160;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 150: switch(input)
			{
				case T_EQL: nextAction = T_SHIFT;
					nextState_rule = 115;
					updateCompCondition(query, lexeme);
					break;
				case CONDITION: nextAction = T_SHIFT;
					nextState_rule = 161;
					break;
				case T_IS: nextAction = T_SHIFT;
					nextState_rule = 110;
					updateCompCondition(query, lexeme);
					break;
				case OPTIONAL_NOT: nextAction = T_SHIFT;
					nextState_rule = 114;
					break;
				case T_NULL: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case EQUALITYOP1: nextAction = T_SHIFT;
					nextState_rule = 111;
					break;
				case EQUALITYOP2: nextAction = T_SHIFT;
					nextState_rule = 112;
					break;
				case T_LIKE: nextAction = T_SHIFT;
					nextState_rule = 113;
					updateCompCondition(query, lexeme);
					break;
				case T_BETWEEN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_IN: nextAction = T_REDUCE;
					nextState_rule = 86;
					break;
				case T_NOT: nextAction = T_SHIFT;
					nextState_rule = 121;
					updateCompCondition(query, lexeme);
					break;
				case T_NEQ: nextAction = T_SHIFT;
					nextState_rule = 116;
					updateCompCondition(query, lexeme);
					break;
				case T_GEQ: nextAction = T_SHIFT;
					nextState_rule = 117;
					updateCompCondition(query, lexeme);
					break;
				case T_LEQ: nextAction = T_SHIFT;
					nextState_rule = 118;
					updateCompCondition(query, lexeme);
					break;
				case T_GTR: nextAction = T_SHIFT;
					nextState_rule = 119;
					updateCompCondition(query, lexeme);
					break;
				case T_LSS: nextAction = T_SHIFT;
					nextState_rule = 120;
					updateCompCondition(query, lexeme);
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 151: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 76;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 152: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 163;
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
				case LISTED_NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 162;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 153: switch(input)
			{
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 164;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 154: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 163;
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
				case LIST_VALUE: nextAction = T_SHIFT;
					nextState_rule = 165;
					break;
				case LISTED_NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 166;
					break;
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 168;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case LISTED_STRING: nextAction = T_SHIFT;
					nextState_rule = 167;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 155: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 67;
					break;
				case REPEATED_COLUMN_PAIR: nextAction = T_SHIFT;
					nextState_rule = 169;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 170;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 156: switch(input)
			{
				case T_EQL: nextAction = T_SHIFT;
					nextState_rule = 171;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 157: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 173;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case REPEATED_ORDERING: nextAction = T_SHIFT;
					nextState_rule = 172;
					break;
				case T_ASC: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case T_DESC: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 158: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 121;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 121;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 121;
					break;
				case T_ASC: nextAction = T_REDUCE;
					nextState_rule = 121;
					break;
				case T_DESC: nextAction = T_REDUCE;
					nextState_rule = 121;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 159: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 122;
					break;
				case T_COMMA: nextAction = T_REDUCE;
					nextState_rule = 122;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 122;
					break;
				case T_ASC: nextAction = T_REDUCE;
					nextState_rule = 122;
					break;
				case T_DESC: nextAction = T_REDUCE;
					nextState_rule = 122;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 160: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 149;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case REPEATED_GROUPING: nextAction = T_SHIFT;
					nextState_rule = 174;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 111;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 161: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 131;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case REPEATED_CONDITION: nextAction = T_SHIFT;
					nextState_rule = 175;
					break;
				case LOGICALOP: nextAction = T_SHIFT;
					nextState_rule = 130;
					break;
				case T_OR: nextAction = T_SHIFT;
					nextState_rule = 132;
					if(!instantiateCondition(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 73;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 162: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 176;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 163: switch(input)
			{
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 178;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 102;
					break;
				case REPEATED_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 177;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 164: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 179;
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
		case 165: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 180;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 166: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 98;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 167: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 99;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 168: switch(input)
			{
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 182;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 105;
					break;
				case REPEATED_STRING: nextAction = T_SHIFT;
					nextState_rule = 181;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 169: switch(input)
			{
				case T_RPAREN: nextAction = T_SHIFT;
					nextState_rule = 183;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 170: switch(input)
			{
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 156;
					if(!updateOn(query, lexeme,tables))
					{
						nextAction = T_REJECT;
					}
					break;
				case COLUMN_PAIR: nextAction = T_SHIFT;
					nextState_rule = 184;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 171: switch(input)
			{
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 185;
					if(!updateOn(query, lexeme,tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 172: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 118;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 118;
					break;
				case OPTIONAL_ORDERING: nextAction = T_SHIFT;
					nextState_rule = 186;
					break;
				case T_ASC: nextAction = T_SHIFT;
					nextState_rule = 187;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_DESC: nextAction = T_SHIFT;
					nextState_rule = 188;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 173: switch(input)
			{
				case T_IDENT: nextAction = T_SHIFT;
					nextState_rule = 158;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case T_TABLECOLUMN: nextAction = T_SHIFT;
					nextState_rule = 159;
					if(!instantiateOrder(query, lexeme, token, tables, columns))
					{
						nextAction = T_REJECT;
					}
					break;
				case ORDERING: nextAction = T_SHIFT;
					nextState_rule = 189;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 174: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 110;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 110;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 110;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 110;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 175: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 72;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 72;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 72;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 72;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 72;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 176: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 88;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 177: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 100;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 178: switch(input)
			{
				case NUMBER_TYPE: nextAction = T_SHIFT;
					nextState_rule = 190;
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
		case 179: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 81;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 180: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_OR: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 82;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 181: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 103;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 182: switch(input)
			{
				case T_VARCHAR: nextAction = T_SHIFT;
					nextState_rule = 191;
					if(!updateRightList(query, lexeme, token, tables))
					{
						nextAction = T_REJECT;
					}
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 183: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				case T_WHERE: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				case T_GROUP: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				case T_HAVING: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				case T_ORDER: nextAction = T_REDUCE;
					nextState_rule = 65;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 184: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 67;
					break;
				case REPEATED_COLUMN_PAIR: nextAction = T_SHIFT;
					nextState_rule = 192;
					break;
				case T_AND: nextAction = T_SHIFT;
					nextState_rule = 170;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 185: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 68;
					break;
				case T_AND: nextAction = T_REDUCE;
					nextState_rule = 68;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 186: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 114;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 114;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 187: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 116;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 116;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 188: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 117;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 117;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 189: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 173;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case REPEATED_ORDERING: nextAction = T_SHIFT;
					nextState_rule = 193;
					break;
				case T_ASC: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				case T_DESC: nextAction = T_REDUCE;
					nextState_rule = 120;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 190: switch(input)
			{
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 178;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 102;
					break;
				case REPEATED_NUMBER: nextAction = T_SHIFT;
					nextState_rule = 194;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 191: switch(input)
			{
				case T_COMMA: nextAction = T_SHIFT;
					nextState_rule = 182;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 105;
					break;
				case REPEATED_STRING: nextAction = T_SHIFT;
					nextState_rule = 195;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 192: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 66;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 193: switch(input)
			{
				case T_SEMICOLON: nextAction = T_REDUCE;
					nextState_rule = 119;
					break;
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 119;
					break;
				case T_ASC: nextAction = T_REDUCE;
					nextState_rule = 119;
					break;
				case T_DESC: nextAction = T_REDUCE;
					nextState_rule = 119;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 194: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 101;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
		case 195: switch(input)
			{
				case T_RPAREN: nextAction = T_REDUCE;
					nextState_rule = 104;
					break;
				default: nextAction = T_REJECT;
					display_error(lexeme, range);
			}
			break;
	}

	return make_pair(nextAction, nextState_rule);
}

pair<int, int> selectGetLHS_RHS(int rule)
{
	int LHS;
	int RHS;
	
	switch (rule)
	{
		case 1: 
			break;
		case 2: LHS = SELECTQUERY;
			RHS = 10;
			break;
		case 3: LHS = OPTIONAL_DISPLAY;
			RHS = 1;
			break;
		case 4: LHS = OPTIONAL_DISPLAY;
			RHS = 3;
			break;
		case 5: LHS = OPTIONAL_DISPLAY;
			RHS = 0;
			break;
		case 6: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
		case 7: LHS = NUMBER_TYPE;
			RHS = 1;
			break;
		case 8: LHS = OPTIONAL_PERCENT;
			RHS = 1;
			break;
		case 9: LHS = OPTIONAL_PERCENT;
			RHS = 0;
			break;
		case 10: LHS = COLUMN_LIST;
			RHS = 2;
			break;
		case 11: LHS = COLUMN;
			RHS = 1;
			break;
		case 12: LHS = COLUMN;
			RHS = 1;
			break;
		case 13: LHS = COLUMN;
			RHS = 2;
			break;
		case 14: LHS = OPTIONAL_HEADER;
			RHS = 2;
			break;
		case 15: LHS = OPTIONAL_HEADER;
			RHS = 0;
			break;
		case 16: LHS = REPEATED_COLUMN;
			RHS = 3;
			break;
		case 17: LHS = REPEATED_COLUMN;
			RHS = 0;
			break;
		case 18: LHS = EXPRESSION;
			RHS = 1;
			break;
		case 19: LHS = EXPRESSION;
			RHS = 1;
			break;
		case 20: LHS = EXPRESSION;
			RHS = 1;
			break;
		case 21: LHS = FUNCTION;
			RHS = 1;
			break;
		case 22: LHS = FUNCTION;
			RHS = 1;
			break;
		case 23: LHS = ROUND;
			RHS = 6;
			break;
		case 24: LHS = SOURCE;
			RHS = 1;
			break;
		case 25: LHS = SOURCE;
			RHS = 1;
			break;
		case 26: LHS = TO_NUMBER;
			RHS = 4;
			break;
		case 27: LHS = GROUP_FUNCTION;
			RHS = 4;
			break;
		case 28: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 29: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 30: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 31: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 32: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 33: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 34: LHS = GROUP_FUNCTION_TYPE;
			RHS = 1;
			break;
		case 35: LHS = ARITHMETIC;
			RHS = 2;
			break;
		case 36: LHS = ARITHMETIC1;
			RHS = 3;
			break;
		case 37: LHS = ARITHMETIC1;
			RHS = 0;
			break;
		case 38: LHS = ARITHMETICOP1;
			RHS = 1;
			break;
		case 39: LHS = ARITHMETICOP1;
			RHS = 1;
			break;
		case 40: LHS = TERM;
			RHS = 2;
			break;
		case 41: LHS = TERM1;
			RHS = 3;
			break;
		case 42: LHS = TERM1;
			RHS = 0;
			break;
		case 43: LHS = ARITHMETICOP2;
			RHS = 1;
			break;
		case 44: LHS = ARITHMETICOP2;
			RHS = 1;
			break;
		case 45: LHS = ARITHMETICOP2;
			RHS = 1;
			break;
		case 46: LHS = FACTOR;
			RHS = 3;
			break;
		case 47: LHS = FACTOR;
			RHS = 1;
			break;
		case 48: LHS = FACTOR;
			RHS = 1;
			break;
		case 49: LHS = FACTOR;
			RHS = 1;
			break;
		case 50: LHS = TABLEE;
			RHS = 2;
			break;
		case 51: LHS = TABLEE;
			RHS = 2;
			break;
		case 52: LHS = OPTIONAL_ALIAS;
			RHS = 1;
			break;
		case 53: LHS = OPTIONAL_ALIAS;
			RHS = 0;
			break;
		case 54: LHS = SUBQUERY;
			RHS = 3;
			break;
		case 55: LHS = OPTIONAL_JOIN;
			RHS = 5;
			break;
		case 56: LHS = OPTIONAL_JOIN;
			RHS = 3;
			break;
		case 57: LHS = OPTIONAL_JOIN;
			RHS = 0;
			break;
		case 58: LHS = JOIN_TYPE;
			RHS = 1;
			break;
		case 59: LHS = JOIN_TYPE;
			RHS = 2;
			break;
		case 60: LHS = OPTIONAL_OUTER;
			RHS = 1;
			break;
		case 61: LHS = OPTIONAL_OUTER;
			RHS = 0;
			break;
		case 62: LHS = OUTER_TYPE;
			RHS = 1;
			break;
		case 63: LHS = OUTER_TYPE;
			RHS = 1;
			break;
		case 64: LHS = OUTER_TYPE;
			RHS = 1;
			break;
		case 65: LHS = JOIN_CONDITION;
			RHS = 4;
			break;
		case 66: LHS = REPEATED_COLUMN_PAIR;
			RHS = 3;
			break;
		case 67: LHS = REPEATED_COLUMN_PAIR;
			RHS = 0;
			break;
		case 68: LHS = COLUMN_PAIR;
			RHS = 3;
			break;
		case 69: LHS = OPTIONAL_WHERE;
			RHS = 2;
			break;
		case 70: LHS = OPTIONAL_WHERE;
			RHS = 0;
			break;
		case 71: LHS = SEARCH_CONDITION;
			RHS = 3;
			break;
		case 72: LHS = REPEATED_CONDITION;
			RHS = 4;
			break;
		case 73: LHS = REPEATED_CONDITION;
			RHS = 0;
			break;
		case 74: LHS = LOGICALOP;
			RHS = 1;
			break;
		case 75: LHS = LOGICALOP;
			RHS = 1;
			break;
		case 76: LHS = CONDITION;
			RHS = 3;
			break;
		case 77: LHS = CONDITION;
			RHS = 2;
			break;
		case 78: LHS = CONDITION;
			RHS = 2;
			break;
		case 79: LHS = CONDITION;
			RHS = 2;
			break;
		case 80: LHS = CONDITION;
			RHS = 2;
			break;
		case 81: LHS = CONDITION_TYPE;
			RHS = 4;
			break;
		case 82: LHS = CONDITION_TYPE;
			RHS = 4;
			break;
		case 83: LHS = CONDVALUE;
			RHS = 1;
			break;
		case 84: LHS = CONDVALUE;
			RHS = 1;
			break;
		case 85: LHS = OPTIONAL_NOT;
			RHS = 1;
			break;
		case 86: LHS = OPTIONAL_NOT;
			RHS = 0;
			break;
		case 87: LHS = GTR_LSS_RIGHT_CONDITION;
			RHS = 1;
			break;
		case 88: LHS = GTR_LSS_RIGHT_CONDITION;
			RHS = 4;
			break;
		case 89: LHS = COUNT_COMP;
			RHS = 1;
			break;
		case 90: LHS = COUNT_COMP;
			RHS = 1;
			break;
		case 91: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 92: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 93: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 94: LHS = EQUALITYOP1;
			RHS = 1;
			break;
		case 95: LHS = EQUALITYOP2;
			RHS = 1;
			break;
		case 96: LHS = EQUALITYOP2;
			RHS = 1;
			break;
		case 97: LHS = LIKE_RIGHT_CONDITION;
			RHS = 1;
			break;
		case 98: LHS = LIST_VALUE;
			RHS = 1;
			break;
		case 99: LHS = LIST_VALUE;
			RHS = 1;
			break;
		case 100: LHS = LISTED_NUMBER_TYPE;
			RHS = 2;
			break;
		case 101: LHS = REPEATED_NUMBER;
			RHS = 3;
			break;
		case 102: LHS = REPEATED_NUMBER;
			RHS = 0;
			break;
		case 103: LHS = LISTED_STRING;
			RHS = 2;
			break;
		case 104: LHS = REPEATED_STRING;
			RHS = 3;
			break;
		case 105: LHS = REPEATED_STRING;
			RHS = 0;
			break;
		case 106: LHS = OPTIONAL_GROUP;
			RHS = 4;
			break;
		case 107: LHS = OPTIONAL_GROUP;
			RHS = 0;
			break;
		case 108: LHS = GROUPING;
			RHS = 1;
			break;
		case 109: LHS = GROUPING;
			RHS = 1;
			break;
		case 110: LHS = REPEATED_GROUPING;
			RHS = 3;
			break;
		case 111: LHS = REPEATED_GROUPING;
			RHS = 0;
			break;
		case 112: LHS = OPTIONAL_HAVING;
			RHS = 2;
			break;
		case 113: LHS = OPTIONAL_HAVING;
			RHS = 0;
			break;
		case 114: LHS = OPTIONAL_ORDER;
			RHS = 5;
			break;
		case 115: LHS = OPTIONAL_ORDER;
			RHS = 0;
			break;
		case 116: LHS = OPTIONAL_ORDERING;
			RHS = 1;
			break;
		case 117: LHS = OPTIONAL_ORDERING;
			RHS = 1;
			break;
		case 118: LHS = OPTIONAL_ORDERING;
			RHS = 0;
			break;
		case 119: LHS = REPEATED_ORDERING;
			RHS = 3;
			break;
		case 120: LHS = REPEATED_ORDERING;
			RHS = 0;
			break;
		case 121: LHS = ORDERING;
			RHS = 1;
			break;
		case 122: LHS = ORDERING;
			RHS = 1;
			break;
	}

	return make_pair(LHS, RHS);
}

int selectGetGoTo(int state, int input)
{
	int newState = -1;	

	switch (state)
	{
		case 1: switch(input)
			{
				case SELECTQUERY: newState = 2;
					break;
				case T_SELECT: newState = 3;
					break;
			}
			break;
		case 2: switch(input)
			{
			}
			break;
		case 3: switch(input)
			{
				case OPTIONAL_DISPLAY: newState = 5;
					break;
				case T_DISTINCT: newState = 6;
					break;
				case T_TOP: newState = 7;
					break;
			}
			break;
		case 4: switch(input)
			{
			}
			break;
		case 5: switch(input)
			{
				case COLUMN_LIST: newState = 8;
					break;
				case NUMBER_TYPE: newState = 31;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case COLUMN: newState = 9;
					break;
				case T_TIMES: newState = 10;
					break;
				case T_TABLETIMES: newState = 11;
					break;
				case EXPRESSION: newState = 12;
					break;
				case T_IDENT: newState = 32;
					break;
				case FUNCTION: newState = 13;
					break;
				case GROUP_FUNCTION: newState = 14;
					break;
				case ARITHMETIC: newState = 15;
					break;
				case ROUND: newState = 16;
					break;
				case TO_NUMBER: newState = 17;
					break;
				case T_ROUND: newState = 20;
					break;
				case T_LPAREN: newState = 30;
					break;
				case T_TABLECOLUMN: newState = 33;
					break;
				case T_TONUMBER: newState = 21;
					break;
				case GROUP_FUNCTION_TYPE: newState = 18;
					break;
				case T_AVG: newState = 22;
					break;
				case T_COUNT: newState = 23;
					break;
				case T_FIRST: newState = 24;
					break;
				case T_LAST: newState = 25;
					break;
				case T_MIN: newState = 26;
					break;
				case T_MAX: newState = 27;
					break;
				case T_SUM: newState = 28;
					break;
				case TERM: newState = 19;
					break;
				case FACTOR: newState = 29;
					break;
			}
			break;
		case 6: switch(input)
			{
			}
			break;
		case 7: switch(input)
			{
				case NUMBER_TYPE: newState = 36;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 8: switch(input)
			{
				case T_FROM: newState = 37;
					break;
			}
			break;
		case 9: switch(input)
			{
				case REPEATED_COLUMN: newState = 38;
					break;
				case T_COMMA: newState = 39;
					break;
			}
			break;
		case 10: switch(input)
			{
			}
			break;
		case 11: switch(input)
			{
			}
			break;
		case 12: switch(input)
			{
				case OPTIONAL_HEADER: newState = 40;
					break;
				case T_AS: newState = 41;
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
			}
			break;
		case 16: switch(input)
			{
			}
			break;
		case 17: switch(input)
			{
			}
			break;
		case 18: switch(input)
			{
				case T_LPAREN: newState = 42;
					break;
			}
			break;
		case 19: switch(input)
			{
				case ARITHMETIC1: newState = 43;
					break;
				case ARITHMETICOP1: newState = 44;
					break;
				case T_PLUS: newState = 45;
					break;
				case T_MINUS: newState = 46;
					break;
			}
			break;
		case 20: switch(input)
			{
				case T_LPAREN: newState = 47;
					break;
			}
			break;
		case 21: switch(input)
			{
				case T_LPAREN: newState = 48;
					break;
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
				case T_MODULO: newState = 53;
					break;
				case T_TIMES: newState = 51;
					break;
				case TERM1: newState = 49;
					break;
				case ARITHMETICOP2: newState = 50;
					break;
				case T_DIVIDE: newState = 52;
					break;
			}
			break;
		case 30: switch(input)
			{
				case NUMBER_TYPE: newState = 31;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case T_IDENT: newState = 32;
					break;
				case ARITHMETIC: newState = 54;
					break;
				case T_LPAREN: newState = 30;
					break;
				case T_TABLECOLUMN: newState = 33;
					break;
				case TERM: newState = 19;
					break;
				case FACTOR: newState = 29;
					break;
			}
			break;
		case 31: switch(input)
			{
			}
			break;
		case 32: switch(input)
			{
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
				case OPTIONAL_PERCENT: newState = 55;
					break;
				case T_MODULO: newState = 56;
					break;
			}
			break;
		case 37: switch(input)
			{
				case TABLEE: newState = 57;
					break;
				case T_IDENT: newState = 58;
					break;
				case T_LPAREN: newState = 60;
					break;
				case SUBQUERY: newState = 59;
					break;
			}
			break;
		case 38: switch(input)
			{
			}
			break;
		case 39: switch(input)
			{
				case NUMBER_TYPE: newState = 31;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case COLUMN: newState = 61;
					break;
				case T_TIMES: newState = 10;
					break;
				case T_TABLETIMES: newState = 11;
					break;
				case EXPRESSION: newState = 12;
					break;
				case T_IDENT: newState = 32;
					break;
				case FUNCTION: newState = 13;
					break;
				case GROUP_FUNCTION: newState = 14;
					break;
				case ARITHMETIC: newState = 15;
					break;
				case ROUND: newState = 16;
					break;
				case TO_NUMBER: newState = 17;
					break;
				case T_ROUND: newState = 20;
					break;
				case T_LPAREN: newState = 30;
					break;
				case T_TABLECOLUMN: newState = 33;
					break;
				case T_TONUMBER: newState = 21;
					break;
				case GROUP_FUNCTION_TYPE: newState = 18;
					break;
				case T_AVG: newState = 22;
					break;
				case T_COUNT: newState = 23;
					break;
				case T_FIRST: newState = 24;
					break;
				case T_LAST: newState = 25;
					break;
				case T_MIN: newState = 26;
					break;
				case T_MAX: newState = 27;
					break;
				case T_SUM: newState = 28;
					break;
				case TERM: newState = 19;
					break;
				case FACTOR: newState = 29;
					break;
			}
			break;
		case 40: switch(input)
			{
			}
			break;
		case 41: switch(input)
			{
				case T_IDENT: newState = 62;
					break;
			}
			break;
		case 42: switch(input)
			{
				case T_IDENT: newState = 64;
					break;
				case SOURCE: newState = 63;
					break;
				case T_TABLECOLUMN: newState = 65;
					break;
			}
			break;
		case 43: switch(input)
			{
			}
			break;
		case 44: switch(input)
			{
				case NUMBER_TYPE: newState = 31;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case T_IDENT: newState = 32;
					break;
				case T_LPAREN: newState = 30;
					break;
				case T_TABLECOLUMN: newState = 33;
					break;
				case TERM: newState = 66;
					break;
				case FACTOR: newState = 29;
					break;
			}
			break;
		case 45: switch(input)
			{
			}
			break;
		case 46: switch(input)
			{
			}
			break;
		case 47: switch(input)
			{
				case T_IDENT: newState = 64;
					break;
				case SOURCE: newState = 67;
					break;
				case T_TABLECOLUMN: newState = 65;
					break;
			}
			break;
		case 48: switch(input)
			{
				case T_IDENT: newState = 64;
					break;
				case SOURCE: newState = 68;
					break;
				case T_TABLECOLUMN: newState = 65;
					break;
			}
			break;
		case 49: switch(input)
			{
			}
			break;
		case 50: switch(input)
			{
				case NUMBER_TYPE: newState = 31;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case T_IDENT: newState = 32;
					break;
				case T_LPAREN: newState = 30;
					break;
				case T_TABLECOLUMN: newState = 33;
					break;
				case FACTOR: newState = 69;
					break;
			}
			break;
		case 51: switch(input)
			{
			}
			break;
		case 52: switch(input)
			{
			}
			break;
		case 53: switch(input)
			{
			}
			break;
		case 54: switch(input)
			{
				case T_RPAREN: newState = 70;
					break;
			}
			break;
		case 55: switch(input)
			{
			}
			break;
		case 56: switch(input)
			{
			}
			break;
		case 57: switch(input)
			{
				case OPTIONAL_JOIN: newState = 71;
					break;
				case JOIN_TYPE: newState = 72;
					break;
				case T_CROSS: newState = 73;
					break;
				case T_INNER: newState = 74;
					break;
				case OUTER_TYPE: newState = 75;
					break;
				case T_LEFT: newState = 76;
					break;
				case T_RIGHT: newState = 77;
					break;
				case T_FULL: newState = 78;
					break;
			}
			break;
		case 58: switch(input)
			{
				case T_IDENT: newState = 80;
					break;
				case OPTIONAL_ALIAS: newState = 79;
					break;
			}
			break;
		case 59: switch(input)
			{
				case T_IDENT: newState = 81;
					break;
			}
			break;
		case 60: switch(input)
			{
				case SELECTQUERY: newState = 82;
					break;
				case T_SELECT: newState = 3;
					break;
			}
			break;
		case 61: switch(input)
			{
				case REPEATED_COLUMN: newState = 83;
					break;
				case T_COMMA: newState = 39;
					break;
			}
			break;
		case 62: switch(input)
			{
			}
			break;
		case 63: switch(input)
			{
				case T_RPAREN: newState = 84;
					break;
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
				case ARITHMETIC1: newState = 85;
					break;
				case ARITHMETICOP1: newState = 44;
					break;
				case T_PLUS: newState = 45;
					break;
				case T_MINUS: newState = 46;
					break;
			}
			break;
		case 67: switch(input)
			{
				case T_COMMA: newState = 86;
					break;
			}
			break;
		case 68: switch(input)
			{
				case T_RPAREN: newState = 87;
					break;
			}
			break;
		case 69: switch(input)
			{
				case T_MODULO: newState = 53;
					break;
				case T_TIMES: newState = 51;
					break;
				case TERM1: newState = 88;
					break;
				case ARITHMETICOP2: newState = 50;
					break;
				case T_DIVIDE: newState = 52;
					break;
			}
			break;
		case 70: switch(input)
			{
			}
			break;
		case 71: switch(input)
			{
				case OPTIONAL_WHERE: newState = 89;
					break;
				case T_WHERE: newState = 90;
					break;
			}
			break;
		case 72: switch(input)
			{
				case T_JOIN: newState = 91;
					break;
			}
			break;
		case 73: switch(input)
			{
				case T_JOIN: newState = 92;
					break;
			}
			break;
		case 74: switch(input)
			{
			}
			break;
		case 75: switch(input)
			{
				case OPTIONAL_OUTER: newState = 93;
					break;
				case T_OUTER: newState = 94;
					break;
			}
			break;
		case 76: switch(input)
			{
			}
			break;
		case 77: switch(input)
			{
			}
			break;
		case 78: switch(input)
			{
			}
			break;
		case 79: switch(input)
			{
			}
			break;
		case 80: switch(input)
			{
			}
			break;
		case 81: switch(input)
			{
			}
			break;
		case 82: switch(input)
			{
				case T_RPAREN: newState = 95;
					break;
			}
			break;
		case 83: switch(input)
			{
			}
			break;
		case 84: switch(input)
			{
			}
			break;
		case 85: switch(input)
			{
			}
			break;
		case 86: switch(input)
			{
				case T_INT: newState = 96;
					break;
			}
			break;
		case 87: switch(input)
			{
			}
			break;
		case 88: switch(input)
			{
			}
			break;
		case 89: switch(input)
			{
				case OPTIONAL_GROUP: newState = 97;
					break;
				case T_GROUP: newState = 98;
					break;
			}
			break;
		case 90: switch(input)
			{
				case T_IDENT: newState = 101;
					break;
				case T_TABLECOLUMN: newState = 102;
					break;
				case SEARCH_CONDITION: newState = 99;
					break;
				case CONDVALUE: newState = 100;
					break;
			}
			break;
		case 91: switch(input)
			{
				case TABLEE: newState = 103;
					break;
				case T_IDENT: newState = 58;
					break;
				case T_LPAREN: newState = 60;
					break;
				case SUBQUERY: newState = 59;
					break;
			}
			break;
		case 92: switch(input)
			{
				case TABLEE: newState = 104;
					break;
				case T_IDENT: newState = 58;
					break;
				case T_LPAREN: newState = 60;
					break;
				case SUBQUERY: newState = 59;
					break;
			}
			break;
		case 93: switch(input)
			{
			}
			break;
		case 94: switch(input)
			{
			}
			break;
		case 95: switch(input)
			{
			}
			break;
		case 96: switch(input)
			{
				case T_RPAREN: newState = 105;
					break;
			}
			break;
		case 97: switch(input)
			{
				case OPTIONAL_HAVING: newState = 106;
					break;
				case T_HAVING: newState = 107;
					break;
			}
			break;
		case 98: switch(input)
			{
				case T_BY: newState = 108;
					break;
			}
			break;
		case 99: switch(input)
			{
			}
			break;
		case 100: switch(input)
			{
				case T_EQL: newState = 115;
					break;
				case CONDITION: newState = 109;
					break;
				case T_IS: newState = 110;
					break;
				case OPTIONAL_NOT: newState = 114;
					break;
				case EQUALITYOP1: newState = 111;
					break;
				case EQUALITYOP2: newState = 112;
					break;
				case T_LIKE: newState = 113;
					break;
				case T_NOT: newState = 121;
					break;
				case T_NEQ: newState = 116;
					break;
				case T_GEQ: newState = 117;
					break;
				case T_LEQ: newState = 118;
					break;
				case T_GTR: newState = 119;
					break;
				case T_LSS: newState = 120;
					break;
			}
			break;
		case 101: switch(input)
			{
			}
			break;
		case 102: switch(input)
			{
			}
			break;
		case 103: switch(input)
			{
				case T_ON: newState = 122;
					break;
			}
			break;
		case 104: switch(input)
			{
			}
			break;
		case 105: switch(input)
			{
			}
			break;
		case 106: switch(input)
			{
				case OPTIONAL_ORDER: newState = 123;
					break;
				case T_ORDER: newState = 124;
					break;
			}
			break;
		case 107: switch(input)
			{
				case T_IDENT: newState = 101;
					break;
				case T_TABLECOLUMN: newState = 102;
					break;
				case SEARCH_CONDITION: newState = 125;
					break;
				case CONDVALUE: newState = 100;
					break;
			}
			break;
		case 108: switch(input)
			{
				case T_IDENT: newState = 127;
					break;
				case T_TABLECOLUMN: newState = 128;
					break;
				case GROUPING: newState = 126;
					break;
			}
			break;
		case 109: switch(input)
			{
				case T_AND: newState = 131;
					break;
				case REPEATED_CONDITION: newState = 129;
					break;
				case LOGICALOP: newState = 130;
					break;
				case T_OR: newState = 132;
					break;
			}
			break;
		case 110: switch(input)
			{
				case OPTIONAL_NOT: newState = 133;
					break;
				case T_NOT: newState = 121;
					break;
			}
			break;
		case 111: switch(input)
			{
				case NUMBER_TYPE: newState = 134;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 112: switch(input)
			{
				case NUMBER_TYPE: newState = 136;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case GTR_LSS_RIGHT_CONDITION: newState = 135;
					break;
				case COUNT_COMP: newState = 137;
					break;
				case T_ALL: newState = 138;
					break;
				case T_ANY: newState = 139;
					break;
			}
			break;
		case 113: switch(input)
			{
				case LIKE_RIGHT_CONDITION: newState = 140;
					break;
				case T_VARCHAR: newState = 141;
					break;
			}
			break;
		case 114: switch(input)
			{
				case CONDITION_TYPE: newState = 142;
					break;
				case T_BETWEEN: newState = 143;
					break;
				case T_IN: newState = 144;
					break;
			}
			break;
		case 115: switch(input)
			{
			}
			break;
		case 116: switch(input)
			{
			}
			break;
		case 117: switch(input)
			{
			}
			break;
		case 118: switch(input)
			{
			}
			break;
		case 119: switch(input)
			{
			}
			break;
		case 120: switch(input)
			{
			}
			break;
		case 121: switch(input)
			{
			}
			break;
		case 122: switch(input)
			{
				case T_LPAREN: newState = 146;
					break;
				case JOIN_CONDITION: newState = 145;
					break;
			}
			break;
		case 123: switch(input)
			{
			}
			break;
		case 124: switch(input)
			{
				case T_BY: newState = 147;
					break;
			}
			break;
		case 125: switch(input)
			{
			}
			break;
		case 126: switch(input)
			{
				case T_COMMA: newState = 149;
					break;
				case REPEATED_GROUPING: newState = 148;
					break;
			}
			break;
		case 127: switch(input)
			{
			}
			break;
		case 128: switch(input)
			{
			}
			break;
		case 129: switch(input)
			{
			}
			break;
		case 130: switch(input)
			{
				case T_IDENT: newState = 101;
					break;
				case T_TABLECOLUMN: newState = 102;
					break;
				case CONDVALUE: newState = 150;
					break;
			}
			break;
		case 131: switch(input)
			{
			}
			break;
		case 132: switch(input)
			{
			}
			break;
		case 133: switch(input)
			{
				case T_NULL: newState = 151;
					break;
			}
			break;
		case 134: switch(input)
			{
			}
			break;
		case 135: switch(input)
			{
			}
			break;
		case 136: switch(input)
			{
			}
			break;
		case 137: switch(input)
			{
				case T_LPAREN: newState = 152;
					break;
			}
			break;
		case 138: switch(input)
			{
			}
			break;
		case 139: switch(input)
			{
			}
			break;
		case 140: switch(input)
			{
			}
			break;
		case 141: switch(input)
			{
			}
			break;
		case 142: switch(input)
			{
			}
			break;
		case 143: switch(input)
			{
				case NUMBER_TYPE: newState = 153;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 144: switch(input)
			{
				case T_LPAREN: newState = 154;
					break;
			}
			break;
		case 145: switch(input)
			{
			}
			break;
		case 146: switch(input)
			{
				case T_TABLECOLUMN: newState = 156;
					break;
				case COLUMN_PAIR: newState = 155;
					break;
			}
			break;
		case 147: switch(input)
			{
				case T_IDENT: newState = 158;
					break;
				case T_TABLECOLUMN: newState = 159;
					break;
				case ORDERING: newState = 157;
					break;
			}
			break;
		case 148: switch(input)
			{
			}
			break;
		case 149: switch(input)
			{
				case T_IDENT: newState = 127;
					break;
				case T_TABLECOLUMN: newState = 128;
					break;
				case GROUPING: newState = 160;
					break;
			}
			break;
		case 150: switch(input)
			{
				case T_EQL: newState = 115;
					break;
				case CONDITION: newState = 161;
					break;
				case T_IS: newState = 110;
					break;
				case OPTIONAL_NOT: newState = 114;
					break;
				case EQUALITYOP1: newState = 111;
					break;
				case EQUALITYOP2: newState = 112;
					break;
				case T_LIKE: newState = 113;
					break;
				case T_NOT: newState = 121;
					break;
				case T_NEQ: newState = 116;
					break;
				case T_GEQ: newState = 117;
					break;
				case T_LEQ: newState = 118;
					break;
				case T_GTR: newState = 119;
					break;
				case T_LSS: newState = 120;
					break;
			}
			break;
		case 151: switch(input)
			{
			}
			break;
		case 152: switch(input)
			{
				case NUMBER_TYPE: newState = 163;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case LISTED_NUMBER_TYPE: newState = 162;
					break;
			}
			break;
		case 153: switch(input)
			{
				case T_AND: newState = 164;
					break;
			}
			break;
		case 154: switch(input)
			{
				case NUMBER_TYPE: newState = 163;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
				case LIST_VALUE: newState = 165;
					break;
				case LISTED_NUMBER_TYPE: newState = 166;
					break;
				case T_VARCHAR: newState = 168;
					break;
				case LISTED_STRING: newState = 167;
					break;
			}
			break;
		case 155: switch(input)
			{
				case REPEATED_COLUMN_PAIR: newState = 169;
					break;
				case T_AND: newState = 170;
					break;
			}
			break;
		case 156: switch(input)
			{
				case T_EQL: newState = 171;
					break;
			}
			break;
		case 157: switch(input)
			{
				case T_COMMA: newState = 173;
					break;
				case REPEATED_ORDERING: newState = 172;
					break;
			}
			break;
		case 158: switch(input)
			{
			}
			break;
		case 159: switch(input)
			{
			}
			break;
		case 160: switch(input)
			{
				case T_COMMA: newState = 149;
					break;
				case REPEATED_GROUPING: newState = 174;
					break;
			}
			break;
		case 161: switch(input)
			{
				case T_AND: newState = 131;
					break;
				case REPEATED_CONDITION: newState = 175;
					break;
				case LOGICALOP: newState = 130;
					break;
				case T_OR: newState = 132;
					break;
			}
			break;
		case 162: switch(input)
			{
				case T_RPAREN: newState = 176;
					break;
			}
			break;
		case 163: switch(input)
			{
				case T_COMMA: newState = 178;
					break;
				case REPEATED_NUMBER: newState = 177;
					break;
			}
			break;
		case 164: switch(input)
			{
				case NUMBER_TYPE: newState = 179;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 165: switch(input)
			{
				case T_RPAREN: newState = 180;
					break;
			}
			break;
		case 166: switch(input)
			{
			}
			break;
		case 167: switch(input)
			{
			}
			break;
		case 168: switch(input)
			{
				case T_COMMA: newState = 182;
					break;
				case REPEATED_STRING: newState = 181;
					break;
			}
			break;
		case 169: switch(input)
			{
				case T_RPAREN: newState = 183;
					break;
			}
			break;
		case 170: switch(input)
			{
				case T_TABLECOLUMN: newState = 156;
					break;
				case COLUMN_PAIR: newState = 184;
					break;
			}
			break;
		case 171: switch(input)
			{
				case T_TABLECOLUMN: newState = 185;
					break;
			}
			break;
		case 172: switch(input)
			{
				case OPTIONAL_ORDERING: newState = 186;
					break;
				case T_ASC: newState = 187;
					break;
				case T_DESC: newState = 188;
					break;
			}
			break;
		case 173: switch(input)
			{
				case T_IDENT: newState = 158;
					break;
				case T_TABLECOLUMN: newState = 159;
					break;
				case ORDERING: newState = 189;
					break;
			}
			break;
		case 174: switch(input)
			{
			}
			break;
		case 175: switch(input)
			{
			}
			break;
		case 176: switch(input)
			{
			}
			break;
		case 177: switch(input)
			{
			}
			break;
		case 178: switch(input)
			{
				case NUMBER_TYPE: newState = 190;
					break;
				case T_FLOAT: newState = 34;
					break;
				case T_INT: newState = 35;
					break;
			}
			break;
		case 179: switch(input)
			{
			}
			break;
		case 180: switch(input)
			{
			}
			break;
		case 181: switch(input)
			{
			}
			break;
		case 182: switch(input)
			{
				case T_VARCHAR: newState = 191;
					break;
			}
			break;
		case 183: switch(input)
			{
			}
			break;
		case 184: switch(input)
			{
				case REPEATED_COLUMN_PAIR: newState = 192;
					break;
				case T_AND: newState = 170;
					break;
			}
			break;
		case 185: switch(input)
			{
			}
			break;
		case 186: switch(input)
			{
			}
			break;
		case 187: switch(input)
			{
			}
			break;
		case 188: switch(input)
			{
			}
			break;
		case 189: switch(input)
			{
				case T_COMMA: newState = 173;
					break;
				case REPEATED_ORDERING: newState = 193;
					break;
			}
			break;
		case 190: switch(input)
			{
				case T_COMMA: newState = 178;
					break;
				case REPEATED_NUMBER: newState = 194;
					break;
			}
			break;
		case 191: switch(input)
			{
				case T_COMMA: newState = 182;
					break;
				case REPEATED_STRING: newState = 195;
					break;
			}
			break;
		case 192: switch(input)
			{
			}
			break;
		case 193: switch(input)
			{
			}
			break;
		case 194: switch(input)
			{
			}
			break;
		case 195: switch(input)
			{
			}
			break;
	}

	return newState;
}

#endif