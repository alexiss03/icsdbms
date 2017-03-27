#ifndef PROTOTYPE_HPP
#define PROTOTYPE_HPP

//function prototype: sql compiler
string get_query();

//function prototype: lexical analyzer
void init_keyword();
void add_keyword(int sym, string keywordName);
string get_tokens(string query, vector<pair<int,pair<string, string> > >& token_lexeme);
int get_symbol(string query, pair<string,string>& tokenText_lexeme, string *range);
int check_keyword(string query, int *charPos, pair<string,string>& tokenText_lexeme, string *range);
int check_string(string query, int *charPos, pair<string,string>& tokenText_lexeme, string *range);
int check_word(string query, int *charPos, pair<string,string>& tokenText_lexeme, string *range);
int check_number(string query, int *charPos, pair<string,string>& tokenText_lexeme, string *range);
int get_word(string query, int **charPos, pair<string,string>& tokenText_lexeme, string *range);
void display_error(string message, int startPos, int endPos);

//function prototype: syntax analyzer
void display_error(string message, string range);

//function prototype: syntax analyzer [delete]
void checkDeleteSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme);
pair<int, int> deleteGetAction(int state, int input, string lexeme, DELETEQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, string range);
pair<int, int> deleteGetLHS_RHS(int rule);
int deleteGetGoTo(int state, int input);

//function prototype: syntax analyzer [update]
void checkUpdateSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme);
pair<int, int> updateGetAction(int state, int input, string lexeme, UPDATEQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, string range);
pair<int, int> updateGetLHS_RHS(int rule);
int updateGetGoTo(int state, int input);

//function prototype: syntax analyzer [insert]
void checkInsertSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme);
pair<int, int> insertGetAction(int state, int input, string lexeme, INSERTQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, string range, bool *isSupported);
pair<int, int> insertGetLHS_RHS(int rule);
int insertGetGoTo(int state, int input);

//function prototype: syntax analyzer [select]
void checkSelectSyntax(vector<pair<pair<int, string>, pair<string, string> > >& token_lexeme);
pair<int, int> selectGetAction(int state, int input, string lexeme, SELECTQUERYPARSE *& query, string token, vector<vector<string> >& tables, vector<vector<string> >& columns, AST *& curEquation, string range, bool *isSupported);
pair<int, int> selectGetLHS_RHS(int rule);
int selectGetGoTo(int state, int input);

//function prototype: semantics analyzer
void instantiateTableAlias(vector<vector<string> >& tables, string lexeme);

//function prototype: semantics analyzer [delete]
void instantiateQuery(DELETEQUERYPARSE *& query);
bool instantiateTable(DELETEQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables);
bool instantiateCondition(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
void updateCompCondition(DELETEQUERYPARSE *& query, string lexeme);
bool updateRightCondition(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool updateRightList(DELETEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);

//function prototype: semantics analyzer [update]
void instantiateQuery(UPDATEQUERYPARSE *& query);
bool instantiateTable(UPDATEQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables);
bool instantiatePair(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool updateValue(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool instantiateCondition(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
void updateCompCondition(UPDATEQUERYPARSE *& query, string lexeme);
bool updateRightCondition(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool updateRightList(UPDATEQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);

//function prototype: semantics analyzer [insert]
void instantiateQuery(INSERTQUERYPARSE *& query);
bool instantiateTable(INSERTQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables);
bool instantiateColumn(INSERTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
void updateInsertType(INSERTQUERYPARSE *& query, string lexeme);
void autoInstantiateColumn(INSERTQUERYPARSE *& query, vector<vector<string> >& tables);
bool updateValue(INSERTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool checkCount(INSERTQUERYPARSE *& query);
bool checkNotNullableColumns(INSERTQUERYPARSE *& query, vector<vector<string> >& tables);
void instantiateColumnAlias(SELECTQUERYPARSE *& query, vector<vector<string> >& tables, string lexeme);

//function prototype: semantics analyzer [select]
void instantiateQuery(SELECTQUERYPARSE *& query);
void instantiateDisplay(SELECTQUERYPARSE *& query, string lexeme, string token);
void instantiateColumn(SELECTQUERYPARSE *& query, string lexeme, string token, AST *& curEquation, vector<vector<string> >& columns);
void updateColumn(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& columns);
void updateAST(SELECTQUERYPARSE *& query, string lexeme, string token, AST *& curEquation, vector<vector<string> >& columns);
bool instantiateTable(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
void updateTable(SELECTQUERYPARSE *& query, string lexeme);
bool updateOn(SELECTQUERYPARSE *& query, string lexeme, vector<vector<string> >& tables);
bool instantiateCondition(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns);
void updateCompCondition(SELECTQUERYPARSE *& query, string lexeme);
bool updateRightCondition(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool updateRightList(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables);
bool instantiateGroup(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns);
bool instantiateOrder(SELECTQUERYPARSE *& query, string lexeme, string token, vector<vector<string> >& tables, vector<vector<string> >& columns);
bool checkColumns(SELECTQUERYPARSE *& query, vector<vector<string> >& tables, vector<vector<string> >& columns);

#endif
