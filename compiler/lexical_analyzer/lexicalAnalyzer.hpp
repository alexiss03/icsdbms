#ifndef LEXICALANALYZER_HPP
#define LEXICALANALYZER_HPP

#define KEYWORDCOUNT 46

struct keyword
{
	int sym;
	string keywordName;
} keywordTab[KEYWORDCOUNT];

int charPos;

int get_symbol(string query, pair<string, string>& tokenText_lexeme, string *range)
{
	int startPos = charPos;	
	int endPos, sym;
	bool isFinal = false;

	switch (query[charPos])
	{
		case ' ':
		case '\t':
		case '\n': charPos++;
			while (query[charPos] == ' ' || query[charPos] == '\t' || query[charPos] == '\n')
				charPos++;	
			sym = T_WHITESPACE;
			break;
		case '*': charPos++;
			sym = T_TIMES;
			break;
		case '/': charPos++;
			sym = T_DIVIDE;
			break;
		case '%': charPos++;
			sym = T_MODULO;
			break;
		case '+': charPos++;
			sym = T_PLUS;
			break;
		case '-': charPos++;
			 sym = T_MINUS;
			break;
		case '=': charPos++;
			 sym = T_EQL;
			break;
		case '#': charPos++;
			 sym = T_NEQ;
			break;
		case '<': charPos++;	
			 if (query[charPos] == '='){
			 	charPos++;
			 	sym = T_LEQ;
			 }
			 else 
			 	sym = T_LSS;
			break;
		case '>': charPos++; 		
			 if (query[charPos] == '='){
			 	charPos++;
			 	sym = T_GEQ;
			 }
			 else 
			 	sym = T_GTR;
			break;
		case ',': charPos++;
			 sym = T_COMMA;
			break;
		case ':': charPos++;
			 sym = T_COLON;
			break;
		case ';': charPos++;
			 sym = T_SEMICOLON;
			break;
		case '(': charPos++;
			 sym = T_LPAREN;
			break;
		case ')': charPos++;
			 sym = T_RPAREN;
			break;
		case '[': sym = check_keyword(query, &charPos, tokenText_lexeme, range);
			isFinal = true;
			break;
		case '\'': sym = check_string(query, &charPos, tokenText_lexeme, range);
			isFinal = true;
			break;
		default: sym = -1;
	}

	endPos = charPos;

	if (sym == -1)
	{
		if (isalpha(query[charPos]))
			sym = check_word(query, &charPos, tokenText_lexeme, range);
		else if (isdigit(query[charPos]))
			sym = check_number(query, &charPos, tokenText_lexeme, range);
		else
		{
			sym = T_ERROR;
			endPos = charPos;
			display_error("invalid character", startPos, endPos);
		}
	}
	else
	{	
		if (!isFinal)	
			tokenText_lexeme.first = query.substr(startPos, endPos-startPos);
		tokenText_lexeme.second = query.substr(startPos, endPos-startPos);

		stringstream ss1, ss2;
		ss1 << startPos;
		ss2 << endPos-1;
		(*range) = "@" + ss1.str() + "-" + ss2.str();
	}

	return sym;
}

void add_keyword(int sym, string keywordName)
{
	static int keywordCounter = 0;

	keywordTab[keywordCounter].sym = sym;
	keywordTab[keywordCounter].keywordName = keywordName;
	keywordCounter++;
}

void init_keyword()
{
	add_keyword(T_ALL, "all");
	add_keyword(T_AND, "and");
	add_keyword(T_ANY, "any");
	add_keyword(T_AS, "as");
	add_keyword(T_ASC, "asc");
	add_keyword(T_AVG, "avg");
	add_keyword(T_BETWEEN, "between");
	add_keyword(T_BY, "by");
	add_keyword(T_CROSS, "cross");
	add_keyword(T_COUNT, "count");
	add_keyword(T_DELETE, "delete");
	add_keyword(T_DESC, "desc");

	add_keyword(T_DISTINCT, "distinct");
	add_keyword(T_FIRST, "first");
	add_keyword(T_FROM, "from");
	add_keyword(T_FULL, "full");
	add_keyword(T_GROUP, "group");
	add_keyword(T_HAVING, "having");
	add_keyword(T_IN, "in");
	add_keyword(T_INNER, "inner");
	add_keyword(T_INSERT, "insert");
	add_keyword(T_INTO, "into");
	add_keyword(T_IS, "is");

	add_keyword(T_JOIN, "join");
	add_keyword(T_LAST, "last");
	add_keyword(T_LEFT, "left");
	add_keyword(T_LIKE, "like");
	add_keyword(T_MAX, "max");
	add_keyword(T_MIN, "min");
	add_keyword(T_NOT, "not");
	add_keyword(T_NULL, "null");
	add_keyword(T_ON, "on");
	add_keyword(T_OR, "or");
	add_keyword(T_ORDER, "order");
	add_keyword(T_OUTER, "outer");

	add_keyword(T_PERCENT, "percent");
	add_keyword(T_RIGHT, "right");
	add_keyword(T_ROUND, "round");
	add_keyword(T_SELECT, "select");
	add_keyword(T_SET, "set");
	add_keyword(T_SUM, "sum");
	add_keyword(T_TONUMBER, "to_number");
	add_keyword(T_TOP, "top");
	add_keyword(T_UPDATE, "update");
	add_keyword(T_VALUES, "values");
	add_keyword(T_WHERE, "where");
}

string get_tokens(string query, vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme)
{
	string tokens, range;
	stringstream ss;
	int sym;

	charPos = 0;

	do
	{
		pair<string, string> tokenText_lexeme;
		sym = get_symbol(query, tokenText_lexeme, &range);

		if (sym != T_WHITESPACE)
		{
			ss << sym << ' ';			
			tokens += ss.str();
			ss.str("");
			ss.clear();
			token_lexeme.push_back(make_pair(make_pair(sym, range), tokenText_lexeme));
		}
	} while ((sym != T_SEMICOLON) && (sym != T_ERROR));
	
	if (sym == T_ERROR)
		tokens = "T_ERROR";

	return tokens;
}

int check_number(string query, int *charPos, pair<string, string>& tokenText_lexeme, string *range)
{
	int startPos = *charPos;	
	int endPos, sym;	
	
	do
	{	
		(*charPos)++;
	} while (isdigit(query[*charPos]));

	if (query[*charPos] == '.')
	{
		(*charPos)++;

		if (isdigit(query[*charPos]))
		{
			do
			{
				(*charPos)++;
			} while (isdigit(query[*charPos]));

			sym = T_FLOAT;
			tokenText_lexeme.first = "T_FLOAT";
		}
		else
		{
			sym = T_ERROR;
			endPos = (*charPos) - 1;
			display_error("expected number after '.'", startPos, endPos);
		}
	}
	else
	{
		sym = T_INT;
		tokenText_lexeme.first = "T_INT";
	}

	endPos = (*charPos);
	tokenText_lexeme.second = query.substr(startPos, endPos-startPos);

	stringstream ss1, ss2;
	ss1 << startPos;
	ss2 << endPos-1;
	(*range) = "@" + ss1.str() + "-" + ss2.str();

	return sym;
}

int check_word(string query, int *charPos, pair<string, string>& tokenText_lexeme, string *range)
{
	int startPos = *charPos;
	int endPos, sym;
	string lexeme;

	sym = get_word(query, &charPos, tokenText_lexeme, range);

	if (sym == T_IDENT)
	{
		if (query[*charPos] == '.')
		{
			(*charPos)++;

			if (isalpha(query[*charPos]))
			{
				sym = get_word(query, &charPos, tokenText_lexeme, range);

				if (sym == T_IDENT)
				{
					sym = T_TABLECOLUMN;
					tokenText_lexeme.first = "T_TABLECOLUMN";
				}
				else
				{
					sym = T_ERROR;
					endPos = (*charPos) - 1;
					display_error("cannot use keyword as an identifier after '.' if not enclosed in '[' and ']'", startPos, endPos);
				}
			}
			else if (query[*charPos] == '*')
			{
				(*charPos)++;
				sym = T_TABLETIMES;
				tokenText_lexeme.first = "T_TABLETIMES";
			}
			else if (query[*charPos] == '[')
			{
				(*charPos)++;

				if (isalpha(query[*charPos]))
				{
					sym = get_word(query, &charPos, tokenText_lexeme, range);

					if (sym != T_IDENT)
					{
						if (query[*charPos] == ']')
						{
							(*charPos)++;
							sym = T_TABLECOLUMN;
							tokenText_lexeme.first = "T_TABLECOLUMN";
						}
						else
						{
							sym = T_ERROR;
							endPos = (*charPos) - 1;
							display_error("expected ']' after keyword", startPos, endPos);						
						}
					}
					else
					{
						sym = T_ERROR;
						endPos = (*charPos) - 1;
						display_error("only keyword can be enclosed in '[' and ']' to be used as an identifier", startPos, endPos);
					}
				}
				else
				{
					sym = T_ERROR;
					endPos = (*charPos) - 1;
					display_error("expected keyword after '['", startPos, endPos);
				}
			}
			else
			{
				sym = T_ERROR;
				endPos = (*charPos) - 1;
				display_error("expected identifier or '*' after '.'", startPos, endPos);
			}
		}
	}

	endPos = (*charPos);

	lexeme = query.substr(startPos, endPos-startPos);
	transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
	tokenText_lexeme.second = lexeme;

	stringstream ss1, ss2;
	ss1 << startPos;
	ss2 << endPos-1;
	(*range) = "@" + ss1.str() + "-" + ss2.str();

	return sym;
}

int get_word(string query, int **charPos, pair<string, string>& tokenText_lexeme, string *range)
{
	string word;
	int sym;
	int keywordIter = 0;

	do
	{
		word += query[**charPos];
		(**charPos)++;
	} while (isalpha(query[**charPos]) || isdigit(query[**charPos]) || query[**charPos] == '_');

	transform(word.begin(), word.end(), word.begin(), ::tolower);
	while ((keywordIter < KEYWORDCOUNT) && word != keywordTab[keywordIter].keywordName)
	{
		keywordIter++;
	}

	if (keywordIter < KEYWORDCOUNT)
	{
		sym = keywordTab[keywordIter].sym;
		tokenText_lexeme.first = keywordTab[keywordIter].keywordName;
	}
	else
	{
		sym = T_IDENT;
		tokenText_lexeme.first = "T_IDENT";
	}

	return sym;
}

int check_string(string query, int *charPos, pair<string, string>& tokenText_lexeme, string *range)
{
	int startPos = *charPos;	
	int endPos, sym;

	do
	{
		(*charPos)++;
	} while (query[*charPos] != '\'' && query[*charPos] != ';');

	if (query[*charPos] == '\'')
	{
		(*charPos)++;
		sym = T_VARCHAR;
		tokenText_lexeme.first = "T_VARCHAR";
	}
	else
	{
		sym = T_ERROR;
		endPos = (*charPos) - 1;
		display_error("expected ''' before ';'", startPos, endPos);
	}

	endPos = (*charPos);
	tokenText_lexeme.second = query.substr(startPos, endPos-startPos);

	stringstream ss1, ss2;
	ss1 << startPos;
	ss2 << endPos-1;
	(*range) = "@" + ss1.str() + "-" + ss2.str();

	return sym;
}

int check_keyword(string query, int *charPos, pair<string, string>& tokenText_lexeme, string *range)
{
	int startPos = *charPos;	
	int endPos, sym;

	(*charPos)++;
	if (isalpha(query[*charPos]))
	{
		sym = get_word(query, &charPos, tokenText_lexeme, range);

		if (sym != T_IDENT)
		{
			if (query[*charPos] == ']')
			{
				(*charPos)++;
				sym = T_IDENT;
				tokenText_lexeme.first = "T_IDENT";

				if (query[*charPos] == '.')
				{
					(*charPos)++;

					if (isalpha(query[*charPos]))
					{
						sym = get_word(query, &charPos, tokenText_lexeme, range);

						if (sym == T_IDENT)
						{
							sym = T_TABLECOLUMN;
							tokenText_lexeme.first = "T_TABLECOLUMN";
						}
						else
						{
							sym = T_ERROR;
							endPos = (*charPos) - 1;
							display_error("cannot use keyword as an identifier after '.' if not enclosed in '[' and ']'", startPos, endPos);
						}
					}
					else if (query[*charPos] == '*')
					{
						(*charPos)++;
						sym = T_TABLETIMES;
						tokenText_lexeme.first = "T_TABLETIMES";
					}
					else if (query[*charPos] == '[')
					{
						(*charPos)++;

						if (isalpha(query[*charPos]))
						{
							sym = get_word(query, &charPos, tokenText_lexeme, range);

							if (sym != T_IDENT)
							{
								if (query[*charPos] == ']')
								{
									(*charPos)++;
									sym = T_TABLECOLUMN;
									tokenText_lexeme.first = "T_TABLECOLUMN";
								}
								else
								{
									sym = T_ERROR;
									endPos = (*charPos) - 1;
									display_error("expected ']' after keyword", startPos, endPos);						
								}
							}
							else
							{
								sym = T_ERROR;
								endPos = (*charPos) - 1;
								display_error("only keyword can be enclosed in '[' and ']' to be used as an identifier", startPos, endPos);
							}
						}
						else
						{
							sym = T_ERROR;
							endPos = (*charPos) - 1;
							display_error("expected keyword after '['", startPos, endPos);
						}
					}
					else
					{
						sym = T_ERROR;
						endPos = (*charPos) - 1;
						display_error("expected identifier or '*' after '.'", startPos, endPos);
					}
				}
			}
			else
			{
				sym = T_ERROR;
				endPos = (*charPos) - 1;
				display_error("expected ']' after keyword", startPos, endPos);						
			}	
		}
		else
		{
			sym = T_ERROR;
			endPos = (*charPos) - 1;
			display_error("expected keyword after '['", startPos, endPos);
		}
	}
	else
	{
		sym = T_ERROR;
		endPos = (*charPos) - 1;
		display_error("expected keyword after '['", startPos, endPos);
	}

	endPos = (*charPos);
	tokenText_lexeme.second = query.substr(startPos, endPos-startPos);

	stringstream ss1, ss2;
	ss1 << startPos;
	ss2 << endPos-1;
	(*range) = "@" + ss1.str() + "-" + ss2.str();

	return sym;
}

void display_error(string message, int startPos, int endPos)
{
	cout << "@" << startPos << "-" << endPos << ": " << "lexical error: " << message << endl;
}

#endif
