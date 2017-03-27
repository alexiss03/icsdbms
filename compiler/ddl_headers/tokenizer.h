#include <vector>
#include <map>
#include <cctype>
//#include <algorithm>
//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <iterator>
#define RESWORDCOUNT 100

bool isArithmeticOperationByCharacter(std::string symbol);
bool isArithmeticOperationBySymbol(int symbol);
std::string getReserveCharacter(int DDL_sym);
void DDL_getString(std::string query);
void DDL_getCharac(std::string query);
int DDL_getSymbol(std::string query);
int determineSymbol(std::string query);

struct resWord {		//define structure of reserved word
	int DDL_sym;
	std::string resWordID;
} resWordTab[RESWORDCOUNT];


static int ResWordCounter = 0;
int DDL_sym = 0, prevSym = 0;
bool error = false, multipleTablesUsed = false, isTable = false;
std:: string tableName, identName, numberValue, realIdentName, arithSymbol, tempString;

std::string getReserveCharacter(int DDL_sym){
	if(DDL_sym == T_EQL){
		return "=";
	}else if(DDL_sym == T_NEQ){
		return "!=";
	}else if(DDL_sym == T_LSS){
		return "<";
	}else if(DDL_sym == T_GTR){
		return ">";
	}else if(DDL_sym == T_LEQ){
		return ">=";
	}else if(DDL_sym == T_GEQ){
		return "<=";
	}else if(DDL_sym == T_ANY){
		return "any";
	}else if(DDL_sym == T_IN){
		return "in";
	}else if(DDL_sym == T_OR){
		return "or";
	}else if(DDL_sym == T_AND){
		return "and";
	}
}

bool isArithmeticOperationByCharacter(std::string symbol){
	if(symbol == "+" || symbol == "-" || symbol == "*" || symbol == "/" || symbol == "modulo"){
		return true;
	}else{
		return false;
	}
}

bool isArithmeticOperationBySymbol(int symbol){
	if(symbol == T_PLUS || symbol == T_MINUS || symbol == T_TIMES || symbol == T_DIVIDE || symbol == T_MODULO){
		return true;
	}else{
		return false;
	}
}

std::string getReserveWord(int DDL_sym){
	std::string reserveWord = "null";
	for(int i=0;i<ResWordCounter;i++){
		if(DDL_sym == resWordTab[i].DDL_sym){
			reserveWord = resWordTab[i].resWordID;
			break;
		}
	}
	return reserveWord;
}

void DDL_isIdent(std::string query){			//determine the identifier
	std::vector<int> identChar, identChar2;
	int resWordIterator = 0;

	do{		//get successive alphabet
		identChar.push_back(query[charPos]);
		charPos++;
	}while(isalpha(query[charPos]) || isdigit(query[charPos]) || (query[charPos]) == '_');

	std::string identString(identChar.begin(), identChar.end());		//convert vector of characters to string
	realIdentName = identString;
	std::transform(identString.begin(), identString.end(), identString.begin(), (int(&)(int))std::tolower);	//convert string to lowercase

	while((resWordIterator < ResWordCounter) && (identString != resWordTab[resWordIterator].resWordID)){	//determine if string is a keyword
		resWordIterator++;
	};
	
	//(resWordIterator < ResWordCounter)? DDL_sym = resWordTab[resWordIterator].DDL_sym : ((prevSym == T_TABLE)? DDL_sym = table_name: DDL_sym = T_IDENT);
	if(resWordIterator < ResWordCounter){
		DDL_sym = resWordTab[resWordIterator].DDL_sym;
	}else if(prevSym == T_TABLE){
		DDL_sym = T_TABLE_NAME;
		tableName = realIdentName;
	}else{
		DDL_sym = T_IDENT;
	}

        if(DDL_sym == T_TABLE_NAME){
            if(query[charPos] == '.'){		//determine the identifier if containing period
				charPos++;
			if(isalpha(query[charPos])){		//determine the identifier if used in join clause and if selecting one column at a time
				do{
					identChar2.push_back(query[charPos]);
					charPos++;
				}while(isalpha(query[charPos]) || isdigit(query[charPos]) || (query[charPos]) == '_');
			
				resWordIterator = 0;
					
				std::string identString2(identChar2.begin(), identChar2.end());		//convert vector of characters to string
				std::transform(identString2.begin(), identString2.end(), identString2.begin(), (int(&)(int))std::tolower);	//convert string to lowercase

				while((resWordIterator < ResWordCounter) && (identString2 != resWordTab[resWordIterator].resWordID)){	//determine if string is a keyword
					resWordIterator++;
				};

				//(resWordIterator < ResWordCounter)? DDL_sym = -1 : DDL_sym = database_table_name;
				if(resWordIterator < ResWordCounter){
					DDL_sym = -1;
				}else{
					DDL_sym = T_DATABASE_TABLE_NAME;
					tableName = identString2;
				}
				
				if(query[charPos] == '.'){		//determine the identifier if containing period
			                charPos++;
			                if(isalpha(query[charPos])){		//determine the identifier if used in join clause and if selecting one column at a time
				                identChar2.clear();
							 do{
					                identChar2.push_back(query[charPos]);
					                charPos++;
				                }while(isalpha(query[charPos]) || isdigit(query[charPos]) || (query[charPos]) == '_');
			
				                resWordIterator = 0;
							
				                std::string identString2(identChar2.begin(), identChar2.end());		//convert vector of characters to string
				                std::transform(identString2.begin(), identString2.end(), identString2.begin(), (int(&)(int))std::tolower);	//convert string to lowercase

				                while((resWordIterator < ResWordCounter) && (identString2 != resWordTab[resWordIterator].resWordID)){	//determine if string is a keyword
					                resWordIterator++;
				                };

				                //(resWordIterator < ResWordCounter)? DDL_sym = -1 : DDL_sym = user_database_table_name;
							 if(resWordIterator < ResWordCounter){
								DDL_sym = -1;
							}else{
								DDL_sym = T_USER_DATABASE_TABLE_NAME;
								tableName = identString2;
							}
			                }
			                else{
				                DDL_sym = -1;
			                }
		                } 
			}
			else{
				DDL_sym = -1;
			}
			} 
        }
	else if(DDL_sym == T_IDENT){
		identName = realIdentName;
		if(query[charPos] == '.' && !isTable){		//determine the identifier if containing period
			charPos++;
			if(isalpha(query[charPos])){		//determine the identifier if used in join clause and if selecting one column at a time
				do{
					identChar2.push_back(query[charPos]);
					charPos++;
				}while(isalpha(query[charPos]) || isdigit(query[charPos]) || (query[charPos]) == '_');
			
				resWordIterator = 0;
					
				std::string identString2(identChar2.begin(), identChar2.end());		//convert vector of characters to string
				std::transform(identString2.begin(), identString2.end(), identString2.begin(), (int(&)(int))std::tolower);	//convert string to lowercase

				while((resWordIterator < ResWordCounter) && (identString2 != resWordTab[resWordIterator].resWordID)){	//determine if string is a keyword
					resWordIterator++;
				};

				(resWordIterator < ResWordCounter)? DDL_sym = -1 : DDL_sym = T_TABLECOLUMN;
			}
			else if(query[charPos] == '*'){		//determine the identifier if used in join clause and if selecting all columns
				charPos++;
				DDL_sym = T_TABLETIMES;	
			}
			else{
				DDL_sym = -1;
			}
		}
	}
}

void DDL_getString(std::string query){
	tempString = "";
	do{
		tempString = tempString + query[charPos];
		charPos++;
	}while(query[charPos] != '\'' && charPos < query.length());

	if(query[charPos] == '\''){
		DDL_sym = T_STRING;
	}else{
		DDL_sym = -1;
	}
}

void DDL_getFileName(std::string query){
	while((query[charPos] <= ' ') && (query[charPos] != '\0')){		//add: eof
		charPos++;
	}

	if(isalpha(query[charPos])){
	
		do{
			tempString = tempString + query[charPos];
			charPos++;
		}while(isalpha(query[charPos]));

		if(query[charPos] == '.'){
			charPos++;
			if(query[charPos] == 's' || query[charPos] == 'S'){
				charPos++;
				if(query[charPos] == 'q' || query[charPos] == 'Q'){
					charPos++;
					if(query[charPos] == 'l' || query[charPos] == 'L'){
						charPos++;
						tempString = tempString + ".sql";
						DDL_sym = T_FILENAME;
					}else{
						DDL_sym = -1;
					}
				}else{
					DDL_sym = -1;
				}
			}else{
				DDL_sym = -1;
			}
		}else{
			DDL_sym = -1;
		}

	}else{
		DDL_sym = -1;
	}
}

void DDL_getCharac(std::string query){
	tempString = "";
	if(isalpha(query[charPos])){
		tempString = query[charPos];
		charPos++;
		if(query[charPos] == '\''){
			DDL_sym = T_CHARACTER;
		}else{
			DDL_sym = -1;
		}
	}else{
		DDL_sym = -1;
	}
}

void DDL_isNumber(std::string query){		//determine the T_NUMBER
	numberValue = "";
	do{
		numberValue = numberValue + query[charPos]; 
		charPos++;
	}while(isdigit(query[charPos]));

	if(query[charPos] == '.'){		//determine the T_NUMBER if T_DECIMAL
		numberValue = numberValue + query[charPos]; 
		charPos++;
		if(isdigit(query[charPos])){
			do{
				numberValue = numberValue + query[charPos]; 
				charPos++;
			}while(isdigit(query[charPos]));
			DDL_sym = T_DECIMAL;
		}
		else{
			DDL_sym = -1;
		}
	}
	else{
		DDL_sym = T_NUMBER;
	}
}
int checker = 0;
int determineSymbol(std::string query){
	int charCurrentPos = charPos;
	int currentSym = T_MINUS;
	int prevSymHere = prevSym;
	//cout << prevSymHere << " " << currentSym << "\n";
	//cout << "Determine ";
	checker++;
	DDL_getSymbol(query);
	int nextSym = DDL_sym;

	//cout << prevSymHere << " " << currentSym << " " << nextSym << "\n";

	if((prevSymHere != T_DECIMAL && prevSymHere != T_NUMBER) && (nextSym == T_NUMBER || nextSym == T_DECIMAL)){
		numberValue = "-" + numberValue;
		//cout << numberValue << "\n";
		return nextSym;
	}else if((prevSymHere == T_DECIMAL || prevSymHere == T_NUMBER) && (nextSym == T_NUMBER || nextSym == T_DECIMAL)){
		charPos = charCurrentPos;
		return currentSym;
	}else if((prevSymHere == T_DECIMAL || prevSymHere == T_NUMBER) && (nextSym != T_NUMBER && nextSym != T_DECIMAL)){
		charPos = charCurrentPos;
		return currentSym;
	}else if((prevSymHere != T_DECIMAL && prevSymHere != T_NUMBER) && (nextSym != T_NUMBER && nextSym != T_DECIMAL)){
		charPos = charCurrentPos;
		return currentSym;
	}
}

int DDL_getSymbol(std::string query){
	while((query[charPos] <= ' ') && (query[charPos] != '\0')){		//add: eof
		charPos++;
	}	
		//if eof DDL_sym = eof
	switch (query[charPos]){
		case '*': charPos++; 		//T_TIMES
			 DDL_sym = T_TIMES;
			 arithSymbol = "*";
			 break;
		case '/': charPos++; 		//T_DIVIDE
			 DDL_sym = T_DIVIDE;
			 arithSymbol = "/";
			 break;
		case '%': charPos++; 		//T_MODULO
			 DDL_sym = T_MODULO;
			 arithSymbol = "modulo";
			 break;
		case '+': charPos++; 		//T_PLUS
			 DDL_sym = T_PLUS;
			 arithSymbol = "+";
			 break;
		case '-': charPos++; 		//T_MINUS
			 if(checker < 1)
			 	DDL_sym = determineSymbol(query);
			 else
			 	DDL_sym =T_MINUS;
			 arithSymbol = "-";
			 break;		
		case '=': charPos++; 		//equals
			 DDL_sym = T_EQL;
			 break;
		case '#': charPos++; 		//not equal
			 DDL_sym = T_NEQ;
			 break;
		case '<': charPos++; 		
			 if (query[charPos] == '='){		//less than or equal
			 	charPos++;
			 	DDL_sym = T_LEQ;
			 }
			 else 
			 	DDL_sym = T_LSS;			//less than
			 break;
		case '>': charPos++; 		
			 if (query[charPos] == '='){		//greater than or equal
			 	charPos++;
			 	DDL_sym = T_GEQ;
			 }
			 else 
			 	DDL_sym = T_GTR;			//greater than
			 break;
		case '!': charPos++; 		
			 if (query[charPos] == '='){		//not equal
			 	charPos++;
			 	DDL_sym = T_NEQ;
			 }
			 break;
		case '.': charPos++; 		//period
			 DDL_sym = T_PERIOD;
			 break;				 	 			 			 
		case ',': charPos++;		//comma
			 DDL_sym = T_COMMA;
			 break;
		case ':': charPos++;		//colon
			 DDL_sym = T_COLON;
			 break;
		case ';': charPos++;		//semicolon
			 DDL_sym = T_SEMICOLON;
			 break;
		case '(': charPos++; 		//period
			 DDL_sym = T_LPAREN;
			 break;				 	 			 			 
		case '[': charPos++;		//comma
			 DDL_sym = T_LBRACK;
			 break;
		case ')': charPos++;		//colon
			 DDL_sym = T_RPAREN;
			 break;
		case ']': charPos++;		//semicolon
			 DDL_sym = T_RBRACK;
			 break;	
		case '\'':charPos++;
			DDL_sym = T_APOSTROPHE;
			break;		 
		default: DDL_sym = -1;
	}
	
	if(DDL_sym == -1){
		if(isdigit(query[charPos]))		//digit
			DDL_isNumber(query);
		else if(isalpha(query[charPos]))		//alphabetic
			DDL_isIdent(query);
		else;
	}
	checker = 0;
	//printf("DDL_sym: %d\n", DDL_sym);
	prevSym = DDL_sym;

}

void displayError(std::string message){		//output the error
	std::cout << "\n" << message << "\n";
}

void DDL_addResWord(int DDL_sym, std::string keyWordName){		//add a reserved word
	resWordTab[ResWordCounter].DDL_sym = DDL_sym;
	resWordTab[ResWordCounter].resWordID = keyWordName;
	ResWordCounter++;
}

void DDL_initResword(){		//identify all reserved words
	
	DDL_addResWord(T_TABLETIMES, "table_times");
	DDL_addResWord(T_TABLECOLUMN, "table_column");

	DDL_addResWord(T_CREATE, "create");
	DDL_addResWord(T_TABLE, "table");
	
	DDL_addResWord(T_RES_NUMBER, "number");
	DDL_addResWord(T_RES_DECIMAL, "decimal");
	DDL_addResWord(T_CHARAC, "char");
	DDL_addResWord(T_DATE, "date");
	DDL_addResWord(T_VARCHAR, "varchar");
	DDL_addResWord(T_RES_BOOLEAN, "boolean");
	DDL_addResWord(T_FLOAT, "float");
	DDL_addResWord(T_INT, "int");
	
	DDL_addResWord(T_RES_NULL, "null");
	DDL_addResWord(T_NOT, "not");
	
	DDL_addResWord(T_CONSTRAINT, "constraint");
	DDL_addResWord(T_RES_UNIQUE, "unique");
	DDL_addResWord(T_CHECK, "check");
	DDL_addResWord(T_PRIMARY, "primary");
	DDL_addResWord(T_FOREIGN, "foreign");
	DDL_addResWord(T_REFERENCES, "references");
	DDL_addResWord(T_KEY, "key");
	DDL_addResWord(T_WITH, "with");
	DDL_addResWord(T_NO, "no");
	DDL_addResWord(T_OPTION, "option");

	DDL_addResWord(T_ALTER,"alter");
	DDL_addResWord(T_ADD,"add");
	DDL_addResWord(T_DROP,"drop");
	DDL_addResWord(T_RES_RENAME, "rename");
	DDL_addResWord(T_MODIFY, "modify");
	DDL_addResWord(T_TO, "to");

	DDL_addResWord(T_IN, "in");
	DDL_addResWord(T_ANY, "any");
	DDL_addResWord(T_OR, "or");
	DDL_addResWord(T_AND, "and");

	DDL_addResWord(T_PROCEDURE, "procedure");
	DDL_addResWord(T_REPLACE, "replace");
	DDL_addResWord(T_VIEW, "view");
	DDL_addResWord(T_QUIT, "quit");
	DDL_addResWord(T_SHOWDBS, "showdbs");
	DDL_addResWord(T_DESC, "desc");
	DDL_addResWord(T_BEGIN, "begin");
	DDL_addResWord(T_END, "end");

	DDL_addResWord(T_SELECT, "select");
	DDL_addResWord(T_ALL, "all");
	DDL_addResWord(T_AS, "as");
	DDL_addResWord(T_FROM, "from");
	DDL_addResWord(T_INSERT, "insert");
	DDL_addResWord(T_UPDATE, "update");
	DDL_addResWord(T_DELETE, "delete");
	DDL_addResWord(T_SET, "set");
	DDL_addResWord(T_VALUES, "values");
	DDL_addResWord(T_INTO, "into");
	DDL_addResWord(T_WHERE, "where");

	DDL_addResWord(T_COMMIT, "commit");
	DDL_addResWord(T_ROLLBACK, "rollback");
	DDL_addResWord(T_EXPORT, "export");
	DDL_addResWord(T_IMPORT, "import");
}
