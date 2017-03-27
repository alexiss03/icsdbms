#include <stdlib.h>
void createTableClause(std::string query);
void nullClause(std::string query);
void constraintClause(std::string query);
void referencesClauses(std::string query);
void functionDataType(std::string query, std::string dataType);
void functionConstraintType(std::string query, std::string constType);
void multipleColumnName(std::string query);
void columnNameClause(std::string query);
void functionCheckConstraint(std::string query);
void checkConstraintClause(std::string query);
std::string parseArithmeticExpression(std:: string query, std::string dataType);
std::string generateCreateParsedString();
std::string convertIntToString(int value);
std::string convertFloatToString(float value);
std::string retrieveDataType(std::string column);
bool isNotNull(std::string columnName);


bool isMultipleColumn = false;
bool isConstraintTable = false;
bool isWithArithGroup = false;
bool isMultipleCheckCondition = false;
bool isToAdd = false;
bool isToModify = false;
int openParen = 0;
int prevCharPos = 0;
int prevSymbol = 0;
std::string checkCond = "";
std::string checkColumnName = "";

vector<string> columnNameList;
vector<string> columnNameDataType;
vector<string> columnNameNotNull;

vector<string> constraintColumnName; 
vector<string> constraintName;
vector<string> constraintType;
vector<string> referenceTable;
vector<string> referenceColumn;
vector<string> checkCondition;

vector<string> checkColumnNameList;

std::vector<std::string> queryActionColumn;
std::vector<std::string> queryActionConstraint;

/*std::string writeToFile(){
	std::ofstream output_file("tableColumns.txt");
	std::ostream_iterator<std::string> output_iterator(output_file, "\n");
	std::copy(columnNameVector.begin(), columnNameVector.end(), output_iterator);
}*/

std::string generateCreateParsedString(){
	
	int countPrimaryKey = 0;
	int countUniqueKey = 0;
	int countNotNull = 0;
	int countVarChar = 0;
	int countConstraintName = 0;
	int noOfTokens = 0;
	std::string parsedString;
	
	countPrimaryKey = std::count (constraintType.begin(), constraintType.end(), "_pk");
	countUniqueKey = std::count (constraintType.begin(), constraintType.end(), "_uk");
	countNotNull = std::count (columnNameNotNull.begin(), columnNameNotNull.end(), "Not Null");
	
	for(int i=0;i < (int)columnNameDataType.size();i++){
		std::size_t found = columnNameDataType[i].find("varchar");
		if(found!=std::string::npos){
			countVarChar++;
		}
	}

	for(int i=0;i < (int)constraintName.size();i++){
		if(constraintType[i] == "_pk"){
			if(constraintName[i] != "NULL")
				countConstraintName++;
		}else if(constraintType[i] == "_uk"){
			if(constraintName[i] != "NULL")
				countConstraintName++;
		}
	}
	
	noOfTokens = countPrimaryKey+countUniqueKey+countNotNull+countConstraintName+((int)columnNameList.size()*2)+(int)columnNameDataType.size()+countVarChar;
	
	char intStr[100];
	sprintf(intStr,"%d",noOfTokens);
	std::string strOfTokens = std::string(intStr);

	parsedString = "create " + tableName + " " + strOfTokens + " ";
	
	for(int i=0;i < (int)columnNameList.size();i++){
		
		parsedString = parsedString + columnNameList[i] + " " + columnNameDataType[i] + " ";
		
		if(columnNameNotNull[i] == "Not Null"){
			parsedString = parsedString + "_nn ";
		}
		
		for(int j=0;j < (int)constraintColumnName.size();j++){
			if(columnNameList[i] == constraintColumnName[j] && (constraintType[j] == "_pk" || constraintType[j] == "_uk")){
				parsedString = parsedString + constraintType[j] + " "; 
				if(constraintName[j] != "NULL")
					parsedString = parsedString + constraintName[j] + " ";
			}
		}

		parsedString = parsedString + "| ";
		
	}
	
	return parsedString;
}

bool isNotNull(std::string columnName){
	for(int j=0;j<(int)columnNameList.size();j++){
		if(columnNameList[j] == identName)
			if(columnNameNotNull[j] == "Not Null")
				return true;
	}

	return false;
}

std::string retrieveDataType(std::string column){

	for(int i=0;i<(int)columnNameList.size();i++){
		if(column == columnNameList[i])
			return (columnNameDataType[i].find("varchar") != std::string::npos? "varchar":columnNameDataType[i]);
	}
	return "NULL";
}

std::string convertIntToString(int value){
	char intStr[1000];
	sprintf(intStr,"%d",value);
	return std::string(intStr);
}

std::string convertFloatToString(float value){
	char intStr[1000];
	sprintf(intStr,"%.10f",value);
	return std::string(intStr);
}

std::string parseArithmeticExpression(std::string query, std::string dataType){
	vector<string> arithmeticExpression;
	int arithValueInt, operand1Int, operand2Int;	
	float arithValueFloat, operand1Float, operand2Float;
	
	if(!error && ((dataType == "int"? (DDL_sym == T_NUMBER?true:false) : (dataType == "float"?(DDL_sym == T_DECIMAL?true:false):false) ) || DDL_sym == T_LPAREN)){
		do{
			
			if((dataType == "int"? (DDL_sym == T_NUMBER?true:false) : (dataType == "float"?(DDL_sym == T_DECIMAL?true:false):false) )){
				
				arithmeticExpression.push_back(numberValue);
			}else if((DDL_sym == T_PLUS || DDL_sym == T_MINUS) && !isArithmeticOperationByCharacter(arithmeticExpression.back())){
				
				arithmeticExpression.push_back(arithSymbol);
			}else if((DDL_sym == T_TIMES || DDL_sym == T_DIVIDE || DDL_sym == T_MODULO) && !isArithmeticOperationByCharacter(arithmeticExpression.back())){
				
				arithmeticExpression.push_back(arithSymbol);
				/*cout << "\n Vector: ";
				for(int i=0;i < (int)arithmeticExpression.size();i++)
					cout << arithmeticExpression[i] << " ";
				cout << "\n";*/
				DDL_getSymbol(query);
				if((dataType == "int"? (DDL_sym == T_NUMBER?true:false) : (dataType == "float"?(DDL_sym == T_DECIMAL?true:false):false) )){
					arithmeticExpression.push_back(numberValue);
				}else if(DDL_sym == T_LPAREN){
					openParen++;
					/*cout << "\nOpenParenSize: " << openParen << "\n";
					cout << "\nhoy\n";*/
					DDL_getSymbol(query);
					arithmeticExpression.push_back(parseArithmeticExpression(query, dataType));
					//cout << "\nhoy Im Back\n";
				}else{
					error = true;
					displayError("PARSING ERROR: Invalid arithemtic expression for CHECK constraint");
					break;
				}
				std::string tempOperand1 = arithmeticExpression[(int)arithmeticExpression.size()-3];
				std::string tempOperand2 = arithmeticExpression[(int)arithmeticExpression.size()-1];
				if(dataType == "int"){
					operand1Int = atoi(tempOperand1.c_str());
					operand2Int = atoi(tempOperand2.c_str());	
				}else if(dataType == "float"){
					operand1Float = atof(tempOperand1.c_str());
					operand2Float = atof(tempOperand2.c_str());
				}
				//cout << "\nOperands: " << operand1 << " " << arithmeticExpression[(int)arithmeticExpression.size()-2] << " " << operand2;
				if(arithmeticExpression[(int)arithmeticExpression.size()-2] == "*"){
					if(dataType == "int"){
						arithValueInt = operand1Int * operand2Int; 	
					}else if(dataType == "float"){
						arithValueFloat = operand1Float * operand2Float; 
					}
				}else if(arithmeticExpression[(int)arithmeticExpression.size()-2] == "/"){
					
					if(dataType == "int"){
						if(operand2Int != 0){
							arithValueInt = operand1Int / operand2Int; 	
						}else if(operand2Int == 0){
							error = true;
							displayError("PARSING ERROR: Floating Point Exception, division by zero");
							break;
						}
					}else if(dataType == "float"){
						if(operand2Float != 0){
							arithValueFloat = operand1Float / operand2Float; 
						}else if(operand2Float == 0){
							error = true;
							displayError("PARSING ERROR: Floating Point Exception, division by zero");
							break;
						}
					}
				}else if(arithmeticExpression[(int)arithmeticExpression.size()-2] == "modulo"){
					if(dataType == "int"){
						arithValueInt = operand1Int % operand2Int; 	
					}else if(dataType == "float"){
						error = true;
						displayError("PARSING ERROR: Modulo operation is not allowed in Float data types.");
						break;
					} 
				}
				arithmeticExpression.pop_back();
				arithmeticExpression.pop_back();
				arithmeticExpression.pop_back();
				//cout << "\nSize: " << (int)arithmeticExpression.size() << " " << arithValue;
				if(dataType == "int"){
					arithmeticExpression.push_back(convertIntToString(arithValueInt));
				}else if(dataType == "float"){
					arithmeticExpression.push_back(convertFloatToString(arithValueFloat));
				} 
				
			}else if(DDL_sym == T_LPAREN){
				openParen++;
				/*cout << "\nOpenParenSize: " << openParen << "\n";
				cout << "\nhey\n";*/
				DDL_getSymbol(query);
				arithmeticExpression.push_back(parseArithmeticExpression(query, dataType));
				//cout << "\nhey Im Here\n";
			}else if(DDL_sym == T_RPAREN && arithmeticExpression.size() > 0){
				openParen--;
				//cout << "\nOpenParenSize: " << openParen << "\n";
				break;
			}else{
				error = true;
				displayError("PARSING ERROR: Invalid arithemtic expression for CHECK constraint");
				break;
			}
			/*cout << "\n Vector: ";
			for(int i=0;i < (int)arithmeticExpression.size();i++)
					cout << arithmeticExpression[i] << " ";
			cout << "\n";*/

			if(!error){
				prevCharPos = charPos;
				DDL_getSymbol(query);
			}else{
				break;
			}	
		}while((DDL_sym == T_NUMBER || DDL_sym == T_DECIMAL || isArithmeticOperationBySymbol(DDL_sym) || (isWithArithGroup && (DDL_sym == T_LPAREN || DDL_sym == T_RPAREN))) && ((isWithArithGroup && openParen > 0) || !isWithArithGroup) && !error);

		if(!error){
			if(!arithmeticExpression.empty() && !isArithmeticOperationByCharacter(arithmeticExpression.back())){
				while(arithmeticExpression.size() != 1 && (arithmeticExpression.size() % 2 == 1) && !error ){
					std::string tempOperand1 = arithmeticExpression[0];
					std::string tempOperand2 = arithmeticExpression[2];
					if(dataType == "int"){
						operand1Int = atoi(tempOperand1.c_str());
						operand2Int = atoi(tempOperand2.c_str());	
					}else if(dataType == "float"){
						operand1Float = atof(tempOperand1.c_str());
						operand2Float = atof(tempOperand2.c_str());
					}
					//cout << "\nOperands: " << operand1 << " " << arithmeticExpression[1] << " " << operand2;
					if(arithmeticExpression[1] == "+"){
						if(dataType == "int"){
							arithValueInt = operand1Int + operand2Int; 	
						}else if(dataType == "float"){
							arithValueFloat = operand1Float + operand2Float; 
						}
					}else if(arithmeticExpression[1] == "-"){
						if(dataType == "int"){
							arithValueInt = operand1Int - operand2Int; 	
						}else if(dataType == "float"){
							arithValueFloat = operand1Float - operand2Float; 
						}
					}
					arithmeticExpression.erase(arithmeticExpression.begin());
					arithmeticExpression.erase(arithmeticExpression.begin());
					arithmeticExpression.erase(arithmeticExpression.begin());
					if(dataType == "int"){
						arithmeticExpression.insert(arithmeticExpression.begin(), convertIntToString(arithValueInt));
					}else if(dataType == "float"){
						arithmeticExpression.insert(arithmeticExpression.begin(), convertFloatToString(arithValueFloat));
					} 
					
					/*cout << "\n Vector: ";
					for(int i=0;i < (int)arithmeticExpression.size();i++)
							cout << arithmeticExpression[i] << " ";
					cout << "\n";*/
				}
			}else if(!error){
				error = true;
				displayError("PARSING ERROR: Invalid arithemtic expression for CHECK constraint");
			}
		}
		
		//cout << "\nValue: " << arithValue << " " << arithmeticExpression[0] << "\n";
		if(!error){
			return arithmeticExpression[0];
		}else{
			return "NULL";
		}
	}else{
		error = true;
		displayError("PARSING ERROR: Invalid arithemtic expression for CHECK constraint");
		return "NULL";
	}
}

void columnNameClause(std::string query){
	if(DDL_sym == T_IDENT){
		if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
			displayError("PARSING ERROR: Column " +identName+ " is declared twice");
			error = true;
		}else{
			isConstraintTable = false;
			
			if(isToAdd || isToModify){
				if(isToAdd){
					const char* table_name = tableName.c_str();
					const char* column_name = identName.c_str();
					if(check_column_name(table_name, column_name)){
						error = true;
						displayError("PARSING ERROR: Column " +identName+ " is already declared");
					}else{
						queryActionColumn.push_back("add");
						columnNameList.push_back(identName);
						DDL_getSymbol(query);
					}
				}else if(isToModify){
					const char* table_name = tableName.c_str();
					const char* column_name = identName.c_str();
					if(check_column_name(table_name, column_name)){
						queryActionColumn.push_back("modify");
						columnNameList.push_back(identName);
						DDL_getSymbol(query);
					}else{
						error = true;
						displayError("PARSING ERROR: Column " +identName+ " is not yet declared");
					}
				}
			}else{
				columnNameList.push_back(identName);
				DDL_getSymbol(query);
			}

			if((DDL_sym == T_DATE || DDL_sym == T_FLOAT || DDL_sym == T_INT || DDL_sym == T_RES_BOOLEAN || DDL_sym == T_CHARAC) && !error){
				columnNameDataType.push_back(getReserveWord(DDL_sym));
				DDL_getSymbol(query);
				nullClause(query);
			}else if(DDL_sym == T_VARCHAR && !error){
				functionDataType(query,getReserveWord(DDL_sym));
				DDL_getSymbol(query);
				nullClause(query);
			}else if(!error){
				error = true;
				displayError("PARSING ERROR: A valid data type should be followed by the column name " +columnNameList.back());
			}
		}
	}else if(DDL_sym == T_CONSTRAINT){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			if(std::find(constraintName.begin(), constraintName.end(), identName) != constraintName.end()) {
				displayError("Constraint Name " +identName+ " is declared twice!");
				error = true;
			}else{
				if(isToAdd || isToModify){
					if(isToAdd){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name, column_name)){
							error = true;
							displayError("PARSING ERROR: Constraint " +identName+ " is already declared");
						}else{
							constraintName.push_back(identName);
							isConstraintTable = true;
							DDL_getSymbol(query);
							if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_FOREIGN || DDL_sym == T_CHECK){
								constraintClause(query);
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, FOREIGN KEY or CHECK is expected after the constraint name "+constraintName.back());
							}
						}
					}else if(isToModify){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name, column_name)){
							constraintName.push_back(identName);
							isConstraintTable = true;
							DDL_getSymbol(query);
							if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_FOREIGN || DDL_sym == T_CHECK){
								constraintClause(query);
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, FOREIGN KEY or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
							}
						}else{
							error = true;
							displayError("PARSING ERROR: Constraint " +identName+ " is not yet declared");
						}
					}
				}else{
					constraintName.push_back(identName);
					isConstraintTable = true;
					DDL_getSymbol(query);
					if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_FOREIGN || DDL_sym == T_CHECK){
						constraintClause(query);
					}else{
						error = true;
						displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
					}
				}
			}
		}else{
			error = true;
			displayError("Error near 'constraint'.");
		}
	}else if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_FOREIGN || T_CHECK){
		constraintName.push_back("NULL");
		isConstraintTable = true;
		constraintClause(query);
	}else{
		error = true;
		displayError("PARSING ERROR: Valid column name format, constraint name or constraiont is expected after ','.");
	}
}

void multipleColumnName(std::string query){
	DDL_getSymbol(query);
	if(DDL_sym != T_IDENT){
		error = true;
		displayError("PARSING ERROR: A valid column name should be followed after '('.");
	}else{
	
	do{
		if(isMultipleColumn && !error){
			DDL_getSymbol(query);
		}
		columnNameClause(query);
		
		isMultipleColumn = true;
	}while(DDL_sym == T_COMMA && !error);
	
	}
	if(DDL_sym == T_RPAREN && !error){
		DDL_getSymbol(query);
		if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
			std::cout << "Table Created!\n";
		}else{
			error = true;
			displayError("PARSING ERROR: Error near ')'.");
		}	
	}else if(DDL_sym != T_RPAREN && !error){
		error = true;
		displayError("PARSING ERROR: Error near column/constraint declaration.");
	}
}

void functionDataType(std::string query, std::string dataType){
	DDL_getSymbol(query);
	if(DDL_sym == T_LPAREN){
		DDL_getSymbol(query);
		if(DDL_sym == T_NUMBER){
			DDL_getSymbol(query);
			if(DDL_sym == T_RPAREN){
				columnNameDataType.push_back(dataType + " " +numberValue);
			}else{
				error = true;
				displayError("PARSING ERROR: Error near column name " + columnNameList.back());
			}
		}else{
			error = true;
			displayError("PARSING ERROR: A valid character size is expected in varchar datatype in column " + columnNameList.back());
		}
	}else{
		error = true;
		displayError("PARSING ERROR: Error near column name " + columnNameList.back());
	}
}

void functionConstraintType(std::string query, std::string constType){
	if(DDL_sym == T_LPAREN){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			DDL_getSymbol(query);
			if(DDL_sym == T_RPAREN){
				int i = 0;
				for(i=0;i<(int)constraintColumnName.size();i++){
					if(constraintColumnName[i] == identName){
						//cout << constraintType[i] << " " << constType << "\n";
						std::size_t found = constraintType[i].find("Foreign Key");
						if(constraintType[i] == constType){
							error = true;
							displayError("PARSING ERROR: Constraint type "+(constType == "_uk"?"unique":(constType == "_pk"?"primary key":constType)) +" is declared twice in column " + identName);
							break;
						}else if(found!=std::string::npos){
							error = true;
							displayError("PARSING ERROR: Constraint type "+ (constType == "_uk"?"unique":(constType == "_pk"?"primary key":constType))+" is declared twice in column " + identName);
							break;
						}
					}
				}
				if(i >= (int) constraintColumnName.size() && !error){
					
					if((isToAdd || isToModify) && isConstraintTable){
						if(isToAdd){
							if(is_unique_key(tableName.c_str(), identName.c_str())){
								error = true;
								displayError("PARSING ERROR: The column " + identName + " is already declared as unique.");
							}else if(is_primary_key(tableName.c_str(), identName.c_str())){
								error = true;
								displayError("PARSING ERROR: The column " + identName + " is already declared as primary key.");
							}
						}
						if(!error){
							constraintType.push_back(constType);
							if(check_column_name(tableName.c_str(),identName.c_str()) || (std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end())){
								if(constType != "_pk" || (is_not_null(tableName.c_str(), identName.c_str()) && constType == "_pk")){
									constraintColumnName.push_back(identName);
								}else{
									error = true;
									displayError("PARSING ERROR: Primary key cannot be declare in a nullable column.");
								}
							}else{
								error = true;
								displayError("Column name " +identName+ " should be declare first.");
							}
						}
					}else{
						constraintType.push_back(constType);
						if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
							if(constType != "_pk" || (isNotNull(identName) && constType == "_pk")){
								constraintColumnName.push_back(identName);
							}else{
								error = true;
								displayError("PARSING ERROR: Primary key cannot be declare in a nullable column.");
							}
						}else{
							error = true;
							displayError("Column name " +identName+ " should be declare first.");
						}
					}
					
				}
			}else{
				error = true;
				displayError("PARSING ERROR: Error near constraint " + (constType == "_uk"?"unique":(constType == "_pk"?"primary key":constType)));
			}
		}else{
			error = true;
			displayError("PARSING ERROR: Invalid column name format inside the constriant type " + (constType == "_uk"?"unique":(constType == "_pk"?"primary key":constType)));
		}
	}else{
		error = true;
		displayError("PARSING ERROR: Error near constraint " + (constType == "_uk"?"unique":(constType == "_pk"?"primary key":constType)));
	}
}

void checkConstraintClause(std::string query){
	
	if(DDL_sym == T_IDENT && !error){
		if(identName != columnNameList.back() && !isConstraintTable){
			error = true;
			displayError("PARSING ERROR: Column name in the check constraint should be the same as the column " + columnNameList.back());
		}else{
			if(isConstraintTable){
				if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
					if(!(std::find(checkColumnNameList.begin(), checkColumnNameList.end(), identName) != checkColumnNameList.end())){
						checkColumnNameList.push_back(identName);
					}
				}else{
					displayError("Column name " +identName+ " should be declare first.");
					error = true;
				}
			}
			checkCond = checkCond + identName;
			DDL_getSymbol(query);
			if((DDL_sym == T_NEQ || DDL_sym == T_EQL || DDL_sym == T_LSS || DDL_sym == T_GTR || DDL_sym == T_LEQ || DDL_sym == T_GEQ) && !error){
				prevSymbol = DDL_sym;
				checkCond = checkCond + " " + getReserveCharacter(DDL_sym);
				DDL_getSymbol(query);
				int pos = std::find(columnNameList.begin(), columnNameList.end(), identName) - columnNameList.begin();
				std::size_t foundChar = columnNameDataType[pos].find("char");
				std::size_t foundVarChar = columnNameDataType[pos].find("varchar");
				if((DDL_sym == T_NUMBER && columnNameDataType[pos] == "int")|| (DDL_sym == T_DECIMAL && columnNameDataType[pos] == "float")){
					isWithArithGroup = false;
					checkCond = checkCond + " " + parseArithmeticExpression(query, columnNameDataType[pos]);
					if(!error){
						charPos = prevCharPos;
						DDL_getSymbol(query);
					}
				}else if(DDL_sym == T_LPAREN){	
					isWithArithGroup = true;
					openParen = 0;
					checkCond = checkCond + " " + parseArithmeticExpression(query, columnNameDataType[pos]);
					if(!error){
						charPos = prevCharPos;
						DDL_getSymbol(query);
					}
				}else if(DDL_sym == T_APOSTROPHE && foundVarChar != std::string::npos){
					DDL_getString(query);
					if(DDL_sym == T_STRING){
						DDL_getSymbol(query);
						if(DDL_sym == T_APOSTROPHE){
							checkCond = checkCond + " '" + tempString + "'";
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: A end quote is expected after the string in the check condition");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: A string is expected in the check condition");
					}
				}else if(DDL_sym == T_APOSTROPHE && foundChar != std::string::npos ){
					DDL_getCharac(query);
					if(DDL_sym == T_CHARACTER){
						DDL_getSymbol(query);
						if(DDL_sym == T_APOSTROPHE){
							checkCond = checkCond + " '" + tempString + "'";
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: A end quote is expected after the character in the check condition");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: A character is expected in the check condition");
					}
				}else if(DDL_sym == T_NUMBER && (numberValue == "0" || numberValue == "1") &&  columnNameDataType[pos] == "boolean"){
					if(prevSymbol == T_EQL || prevSymbol == T_NEQ){
						checkCond = checkCond + numberValue;
						DDL_getSymbol(query);
					}else{
						error = true;
						displayError("PARSING ERROR: Invalid operator for boolean data type");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: A arithmetic expression, string or boolean should be followed after the opertor and type should be compatible in the check condition.");
				}
			}else if((DDL_sym == T_IN || DDL_sym == T_ANY) && !error){
				checkCond = checkCond + " " + getReserveCharacter(DDL_sym);
				DDL_getSymbol(query);
				if(DDL_sym == T_LPAREN){
					std::string checkValues = "";
					do{
						DDL_getSymbol(query);
						int pos = std::find(columnNameList.begin(), columnNameList.end(), identName) - columnNameList.begin();
						std::size_t foundChar = columnNameDataType[pos].find("char");
						std::size_t foundVarChar = columnNameDataType[pos].find("varchar");
						if((DDL_sym == T_NUMBER && columnNameDataType[pos] == "int") || (DDL_sym == T_DECIMAL && columnNameDataType[pos] == "float")){
							checkValues = checkValues + numberValue;
						}else if(DDL_sym == T_APOSTROPHE && foundVarChar != std::string::npos){
							DDL_getString(query);
							if(DDL_sym == T_STRING){
								DDL_getSymbol(query);
								if(DDL_sym == T_APOSTROPHE){
									checkValues = checkValues + " '" + tempString + "'";
								}else{
									error = true;
									displayError("PARSING ERROR: A end quote is expected after the string in the check condition");
								}
							}else{
								error = true;
								displayError("PARSING ERROR: A string is expected in the check condition");
							}
						}else if(DDL_sym == T_APOSTROPHE && foundChar != std::string::npos){
							DDL_getCharac(query);
							if(DDL_sym == T_CHARACTER){
								DDL_getSymbol(query);
								if(DDL_sym == T_APOSTROPHE){
									checkValues = checkValues + " '" + tempString + "'";
								}else{
									error = true;
									displayError("PARSING ERROR: A end quote is expected after the character in the check condition");
								}
							}else{
								error = true;
								displayError("PARSING ERROR: A character is expected in the check condition");
							}
						}else{
							error = true;
							displayError("PARSING ERROR: A arithmetic expression or string should be followed after the opertor in column " + columnNameList.back());
						}
						DDL_getSymbol(query);
						if(DDL_sym == T_COMMA){
							checkValues = checkValues + ", ";
						}
					}while(DDL_sym == T_COMMA && !error);
	
					if(DDL_sym == T_RPAREN && !error){
						checkCond = checkCond + " (" + checkValues + ")";
						DDL_getSymbol(query);
					}else if(!error){
						error = true;
						displayError("PARSING ERROR: ')' is expected after listing the values in CHECK constraint in column ." + columnNameList.back());
					}
				}	
			}else if(!error){
				error = true;
				displayError("PARSING ERROR: A correct operator should be followed after the " + checkCond);
			}
		}
	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name should be followed after the '(' in column " + columnNameList.back());
	}
}

void functionCheckConstraint(std::string query){
	isMultipleCheckCondition = false;
	checkCond = "";
	checkColumnName = "";
	checkColumnNameList.clear();
	if(DDL_sym == T_LPAREN && !error){
		DDL_getSymbol(query);
		if(DDL_sym != T_IDENT){
			error = true;
			displayError("PARSING ERROR: A valid column name should be followed after '('.");
		}else{

			do{
				if(isMultipleCheckCondition && !error){
					DDL_getSymbol(query);
				}
				checkConstraintClause(query);
				isMultipleCheckCondition = true;
				if(DDL_sym == T_AND || DDL_sym == T_OR){
					checkCond = checkCond + " " + getReserveCharacter(DDL_sym) + " ";
				}
			}while((DDL_sym == T_AND || DDL_sym == T_OR) && !error);
		
			if(DDL_sym == T_RPAREN && !error){
				if(isConstraintTable){
					checkColumnName = checkColumnNameList[0];
					for(int i=1;i<(int)checkColumnNameList.size();i++)
						checkColumnName = checkColumnName + " " + checkColumnNameList[i];
					constraintColumnName.push_back(checkColumnName);
				}
				checkCondition.push_back(checkCond);
				DDL_getSymbol(query);
			}else if(!error){
				error = true;
				displayError("PARSING ERROR: A ')' should be followed by after the check condition in column " + columnNameList.back());
			}
		}

	}else{
		error = true;
		displayError("PASRING ERROR: CHECK should be followed by a '(' in column " + columnNameList.back());
	}
}

void referencesClauses(std::string query){
	if(DDL_sym == T_WITH && !error){
		DDL_getSymbol(query);
		if(DDL_sym == T_NO){
			DDL_getSymbol(query);
               if(DDL_sym == T_CHECK){
				DDL_getSymbol(query);
				if(DDL_sym == T_OPTION){
					DDL_getSymbol(query);
					if(DDL_sym == T_IDENT){
						constraintType.push_back( constraintType.back() + " with no check option" );
						constraintType.erase(constraintType.end()-2);
						const char* table_name = identName.c_str();
						if(check_exist(table_name)){
							referenceTable.push_back(identName);
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: The table " +identName+ " being refer in column " +columnNameList.back()+ " is not yet created.");
						}
						if(DDL_sym == T_LPAREN && !error){
							DDL_getSymbol(query);
							if(DDL_sym == T_IDENT){
								DDL_getSymbol(query);
								if(DDL_sym == T_RPAREN){
									std::string tempTableName = referenceTable.back();
									const char* table_name = tempTableName.c_str();
									const char* column_name = identName.c_str();
									if(check_column_name(table_name, column_name)){
										if(check_data_type(table_name,identName,retrieveDataType(constraintColumnName.back()))){
											referenceColumn.push_back(identName);
											DDL_getSymbol(query);
										}else{
											error = true;
											displayError("PARSING ERROR:  Column " + constraintColumnName.back() + " cannot be references with "+ identName +". Data type is incompatible");
										}
									}else{
										error = true;
										displayError("PARSING ERROR: The column " +identName+ " in table " +referenceTable.back()+ " and being refer in column " +columnNameList.back()+ " is not declared.");
									}
								}else{
									error = true;
									displayError("Error near constraint name.");
								}
							}else{
								error = true;
								displayError("Error near '('.");
							}
						}else{
							referenceColumn.push_back("NULL");
						}
					}else{
						error = true;
						displayError("Error near 'option'.");
					}
				}else{
					error = true;
					displayError("Error near 'check'.");
				}	
			}else{
				error = true;
				displayError("Error near 'no'.");
			}
		}else if(DDL_sym == T_CHECK){
			DDL_getSymbol(query);
			if(DDL_sym == T_OPTION){
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT){
					constraintType.push_back( constraintType.back() + " with check option" );
					constraintType.erase(constraintType.end()-2);
					const char* table_name = identName.c_str();
					if(check_exist(table_name)){
						referenceTable.push_back(identName);
						DDL_getSymbol(query);
					}else{
						error = true;
						displayError("PARSING ERROR: The table " +identName+ " being refer in column " +columnNameList.back()+ " is not yet created.");
					}
					if(DDL_sym == T_LPAREN && !error){
						DDL_getSymbol(query);
						if(DDL_sym == T_IDENT){
							DDL_getSymbol(query);
							if(DDL_sym == T_RPAREN){
								const char* table_name = referenceTable.back().c_str();
								const char* column_name = identName.c_str();
								if(check_column_name(table_name, column_name)){
									if(check_data_type(table_name,identName,retrieveDataType(constraintColumnName.back()))){
										referenceColumn.push_back(identName);
										DDL_getSymbol(query);
									}else{
										error = true;
										displayError("PARSING ERROR:  Column " + constraintColumnName.back() + " cannot be references with "+ identName +". Data type is incompatible");
									}
								}else{
									error = true;
									displayError("PARSING ERROR: The column " +identName+ " in table " +referenceTable.back()+ " and being refer in column " +columnNameList.back()+ " is not declared.");
								}
							}else{
								error = true;
								displayError("Error near constraint name.");
							}
						}else{
							error = true;
							displayError("Error near '('.");
						}
					}else{
						referenceColumn.push_back("NULL");
					}
				}else{
					error = true;
					displayError("Error near 'option'.");
				}
			}else{
				error = true;
				displayError("Error near 'check'.");
			}
		}
	}else if(DDL_sym == T_IDENT){
		const char* table_name = identName.c_str();
		if(check_exist(table_name)){
			referenceTable.push_back(identName);
			DDL_getSymbol(query);
		}else{
			error = true;
			displayError("PARSING ERROR: The table " +identName+ " being refer in column " +columnNameList.back()+ " is not yet created.");
		}
		if(DDL_sym == T_LPAREN && !error){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				DDL_getSymbol(query);
				if(DDL_sym == T_RPAREN){
					const char* table_name = referenceTable.back().c_str();
					const char* column_name = identName.c_str();
					if(check_column_name(table_name, column_name)){
						if(check_data_type(table_name,identName,retrieveDataType(constraintColumnName.back()))){
							referenceColumn.push_back(identName);
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR:  Column " + constraintColumnName.back() + " cannot be references with "+ identName +". Data type is incompatible");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: The column " +identName+ " in table " +referenceTable.back()+ " and being refer in column " +columnNameList.back()+ " is not declared.");
					}
				}else{
					error = true;
					displayError("Error near constraint name.");
				}
			}else{
				error = true;
				displayError("Error near '('.");
			}
		}else{
			referenceColumn.push_back("NULL");
		}
	}else{
		error = true;
		displayError("Error near 'references'.");
	}

}

void constraintClause(std::string query){
	if(DDL_sym == T_RES_UNIQUE){
		
		if(isToAdd){
			queryActionConstraint.push_back("add");
			if(!isConstraintTable){
				if(is_unique_key(tableName.c_str(), constraintColumnName.back().c_str())){
					error = true;
					displayError("PARSING ERROR: The column " + constraintColumnName.back() + " is already declared as unique.");
				}
			}
		}else if(isToModify){
			queryActionConstraint.push_back("modify");
		}

		referenceTable.push_back("NULL");
		referenceColumn.push_back("NULL");
		checkCondition.push_back("NULL");
		if(isConstraintTable && !error){
			DDL_getSymbol(query);
			functionConstraintType(query, "_uk");
			DDL_getSymbol(query);
		}else if(!error){
			constraintType.push_back("_uk");
			DDL_getSymbol(query);
		}
	}else if(DDL_sym == T_PRIMARY){
		DDL_getSymbol(query);
		if(DDL_sym == T_KEY){
			if(isToAdd){
				queryActionConstraint.push_back("add");
				if(!isConstraintTable){
					if(is_primary_key(tableName.c_str(), constraintColumnName.back().c_str())){
						error = true;
						displayError("PARSING ERROR: The column " + constraintColumnName.back() + " is already declared as primary key.");
					}
				}
			}else if(isToModify){
				queryActionConstraint.push_back("modify");
			}

			referenceTable.push_back("NULL");
			referenceColumn.push_back("NULL");
			checkCondition.push_back("NULL");
			if(isConstraintTable){
				DDL_getSymbol(query);
				functionConstraintType(query, "_pk");
				DDL_getSymbol(query);
			}else{
				constraintType.push_back("_pk");
				DDL_getSymbol(query);
			}
		}else{
			error = true;
			displayError("Error near primary.");
		}
	}else if(DDL_sym == T_REFERENCES){
		if(isToAdd){
			queryActionConstraint.push_back("add");
		}else if(isToModify){
			queryActionConstraint.push_back("modify");
		}
		constraintType.push_back("References");
		checkCondition.push_back("NULL");
		DDL_getSymbol(query);
		referencesClauses(query);
	}else if(DDL_sym == T_FOREIGN && isConstraintTable){
		DDL_getSymbol(query);
		if(DDL_sym == T_KEY){
			if(isToAdd){
				queryActionConstraint.push_back("add");
			}else if(isToModify){
				queryActionConstraint.push_back("modify");
			}
			checkCondition.push_back("NULL");
			DDL_getSymbol(query);
			functionConstraintType(query, "Foreign Key");
			DDL_getSymbol(query);
			if(DDL_sym == T_REFERENCES && !error){
				DDL_getSymbol(query);
				referencesClauses(query);
			}else{
				error = true;
				displayError("PARSING ERROR: REFERENCES should be followed after the FOREIGN KEY.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: Error near FOREIGN.");
		}
	}else if(DDL_sym == T_CHECK){
		if(isToAdd){
			queryActionConstraint.push_back("add");
		}else if(isToModify){
			queryActionConstraint.push_back("modify");
		}
		constraintType.push_back("Check");
		referenceTable.push_back("NULL");
		referenceColumn.push_back("NULL");
		DDL_getSymbol(query);
		functionCheckConstraint(query);
	}
}

void nullClause(std::string query){
	isConstraintTable = false;
	
	if(DDL_sym == T_NOT && !error){
		DDL_getSymbol(query);
		if(DDL_sym == T_RES_NULL){
			DDL_getSymbol(query);
			columnNameNotNull.push_back("Not Null");
			if(DDL_sym == T_CONSTRAINT){
				constraintColumnName.push_back(identName);
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT && !error){
					
					if(std::find(constraintName.begin(), constraintName.end(), identName) != constraintName.end()) {
						displayError("Constraint Name " +identName+ " is declared twice!");
						error = true;
					}else{
						if(isToAdd || isToModify){
							if(isToAdd){
								const char* table_name = tableName.c_str();
								const char* column_name = identName.c_str();
								if(check_constraint_name(table_name, column_name)){
									error = true;
									displayError("PARSING ERROR: Constraint " +identName+ " is already declared");
								}else{
									constraintName.push_back(identName);
									DDL_getSymbol(query);
									if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
										constraintClause(query);
									}else{
										error = true;
										displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
									}
								}
							}else if(isToModify){
								const char* table_name = tableName.c_str();
								const char* column_name = identName.c_str();
								if(check_constraint_name(table_name, column_name)){
									constraintName.push_back(identName);
									DDL_getSymbol(query);
									if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
										constraintClause(query);
									}else{
										error = true;
										displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
									}
								}else{
									error = true;
									displayError("PARSING ERROR: Constraint " +identName+ " is not yet declared");
								}
							}
						}else{
							constraintName.push_back(identName);
							DDL_getSymbol(query);
							if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
								constraintClause(query);
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, REFERENCES, or CHECK is expected after the column name declaration in column " + columnNameList.back());
							}
						}
					}
				}else{
					error = true;
					displayError("Error near 'constraint'.");
				}
			}else if(DDL_sym != T_COMMA && DDL_sym != T_RPAREN && DDL_sym != T_SEMICOLON){
				constraintColumnName.push_back(identName);
				constraintName.push_back("NULL");
				if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_PRIMARY || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
					constraintClause(query);
				}else{
					error = true;
					displayError("PARSING ERROR: Constraint UNIQUE, PRIMARY KEY, REFERENCES or CHECK is expected after the column name declaration in column " + columnNameList.back());
				}
			}
		}else if(DDL_sym != T_RES_NULL){
			error = true;
			displayError("Error near not.");  
		}
     }else if(DDL_sym == T_RES_NULL){
		columnNameNotNull.push_back("Null");
		DDL_getSymbol(query);
		if(DDL_sym == T_CONSTRAINT){
			constraintColumnName.push_back(identName);
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				
				if(std::find(constraintName.begin(), constraintName.end(), identName) != constraintName.end()) {
					displayError("Constraint Name " +identName+ " is declared twice!");
					error = true;
				}else{
					if(isToAdd || isToModify){
						if(isToAdd){
							const char* table_name = tableName.c_str();
							const char* column_name = identName.c_str();
							if(check_constraint_name(table_name, column_name)){
								error = true;
								displayError("PARSING ERROR: Constraint " +identName+ " is already declared");
							}else{
								constraintName.push_back(identName);
								DDL_getSymbol(query);
								if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
									constraintClause(query);
								}else{
									error = true;
									displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
								}
							}
						}else if(isToModify){
							const char* table_name = tableName.c_str();
							const char* column_name = identName.c_str();
							if(check_constraint_name(table_name, column_name)){
								constraintName.push_back(identName);
								DDL_getSymbol(query);
								if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
									constraintClause(query);
								}else{
									error = true;
									displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
								}
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint " +identName+ " is not yet declared");
							}
						}
					}else{
						constraintName.push_back(identName);
						DDL_getSymbol(query);
						if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
							constraintClause(query);
						}else{
							error = true;
							displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES, or CHECK is expected after the column name declaration in column " + columnNameList.back());
						}
					}
				}
			}else{
				error = true;
				displayError("Error near 'constraint'.");
			}
		}else if(DDL_sym != T_COMMA && DDL_sym != T_RPAREN && DDL_sym != T_SEMICOLON){
			constraintColumnName.push_back(identName);
			constraintName.push_back("NULL");
			if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
				constraintClause(query);
			}else{
				error = true;
				displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES or CHECK is expected after the column name declaration in column " + columnNameList.back());
			}
		}	
	}else if(DDL_sym == T_CONSTRAINT){
		columnNameNotNull.push_back("Null");
		constraintColumnName.push_back(identName);
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			if(std::find(constraintName.begin(), constraintName.end(), identName) != constraintName.end()) {
				displayError("Constraint Name " +identName+ " is declared twice!");
				error = true;
			}else{
				if(isToAdd || isToModify){
					if(isToAdd){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name, column_name)){
							error = true;
							displayError("PARSING ERROR: Constraint " +identName+ " is already declared");
						}else{
							constraintName.push_back(identName);
							DDL_getSymbol(query);
							if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
								constraintClause(query);
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
							}
						}
					}else if(isToModify){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name, column_name)){
							constraintName.push_back(identName);
							DDL_getSymbol(query);
							if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
								constraintClause(query);
							}else{
								error = true;
								displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES or CHECK is expected after the constraint name declaration in column " + columnNameList.back());
							}
						}else{
							error = true;
							displayError("PARSING ERROR: Constraint " +identName+ " is not yet declared");
						}
					}
				}else{
					constraintName.push_back(identName);
					DDL_getSymbol(query);
					if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
						constraintClause(query);
					}else{
						error = true;
						displayError("PARSING ERROR: Constraint UNIQUE, REFERENCES, or CHECK is expected after the column name declaration in column " + columnNameList.back());
					}
						
				}
			}
		}else{
			error = true;
			displayError("Error near 'constraint'.");
		}
	}else if(DDL_sym == T_RES_UNIQUE || DDL_sym == T_REFERENCES || DDL_sym == T_CHECK){
		columnNameNotNull.push_back("Null");
		constraintColumnName.push_back(identName);
		constraintName.push_back("NULL");
		constraintClause(query);
	}else{
		columnNameNotNull.push_back("Null");
	}
                                                
}

void createTableClause(std::string query){
	DDL_getSymbol(query);
	if(DDL_sym == T_TABLE_NAME || DDL_sym == T_DATABASE_TABLE_NAME || DDL_sym == T_USER_DATABASE_TABLE_NAME){
		const char* table_name = tableName.c_str();
		if(check_exist(table_name)){
			error = true;
			displayError("PARSING ERROR: Table " +tableName+ " is already created.");			
		}else{
			DDL_getSymbol(query);
        	if(DDL_sym == T_LPAREN){
				multipleColumnName(query);
			}else{
				error = true;
				displayError("PARSING ERROR: A '(' should be followed after the table name.");
			}	
		}
	}else{
		error=true;
		displayError("PARSING ERROR: A valid table name should be followed after 'table'.");
	}
}
