void createViewClause(std::string query);
void multipleColumnNameView(std::string query);
void simpleSelectQueryClause(std::string query);
void selectWhereConditionClause(std::string query);

bool isMultipleColumnNameView = false;

std::string viewTableName = "";
std::string viewName = "";
std::string selectWhereCond = "";

void createViewClause(std::string query){
	if(DDL_sym == T_IDENT){
		viewName = identName;
		DDL_getSymbol(query);
		if(DDL_sym == T_LPAREN){
			DDL_getSymbol(query);
			multipleColumnNameView(query);
		}else if(DDL_sym == T_AS){
			DDL_getSymbol(query);
			if(DDL_sym == T_LPAREN){
				DDL_getSymbol(query);
				simpleSelectQueryClause(query);
			}else{
				error = true;
				displayError("PARSING ERROR: '(' is expected after AS");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: '(' or AS is expected after view name");
		}
	}else{
		error = true;
		displayError("PARSING ERROR: Invalid view name format.");
	}
}

void multipleColumnNameView(std::string query){
	if(DDL_sym == T_IDENT){
		do{
			if(isMultipleColumnNameView && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_IDENT){
				if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
					displayError("PARSING ERROR: Parameter " +identName+ " is declared twice");
					error = true;
				}else{
					columnNameList.push_back(identName);
					DDL_getSymbol(query);
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid column name is expected after ','");
			}

			isMultipleColumnNameView = true;

		}while(DDL_sym == T_COMMA && !error);

		if(DDL_sym == T_RPAREN && !error){
			DDL_getSymbol(query);
			if(DDL_sym == T_AS){
				DDL_getSymbol(query);
				if(DDL_sym == T_LPAREN){
					DDL_getSymbol(query);
					simpleSelectQueryClause(query);
				}else{
					error = true;
					displayError("PARSING ERROR: '(' is expected after AS");
				}
			}else {
				error = true;
				displayError("PARSING ERROR: AS is expected after the column name list.");
			}
		}else if(!error){
			error = true;
			displayError("PARSING ERROR: '(' is expected after the column name list");
		}

	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name is expected after '('");
	}
}

void simpleSelectQueryClause(std::string query){
	if(DDL_sym == T_SELECT){
		DDL_getSymbol(query);
		if(DDL_sym == T_TIMES){
			DDL_getSymbol(query);
			if(DDL_sym == T_FROM){
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT){
					if(check_exist(identName.c_str())){
						viewTableName = identName;
						int i = 0;
						for(i=0;i<(int)columnNameList.size();i++){
							if(!check_column_name(viewTableName.c_str(), columnNameList[i].c_str())){
								error = true;
								displayError("PARSING ERROR: Column "+columnNameList[i]+" is not declared in table " +viewTableName);
								break;
							}
						}
						if(i >= (int)columnNameList.size() && !error){
							DDL_getSymbol(query);
							if(DDL_sym == T_WHERE){
								DDL_getSymbol(query);
								selectWhereConditionClause(query);
								if(DDL_sym == T_RPAREN && !error){
									DDL_getSymbol(query);
									if(DDL_sym == T_SEMICOLON && charPos >= query.length() && !error){
										cout << "View created!\n";
									}else{
										error = true;
										displayError("PARSING ERROR: ';' is expected after the ')' and and no other extra character should be after ';'.");
									}
								}else if(!error){
									error = true;
									displayError("PARSING ERROR: ')' is expected after the WHERE condition.");
								} 
							}else if(DDL_sym == T_RPAREN){
								DDL_getSymbol(query);
								if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
									cout << "View created!\n";
								}else{
									error = true;
									displayError("PARSING ERROR: ';' is expected after the ')' and and no other extra character should be after ';'.");
								}
							}else{
								error = true;
								displayError("PARSING ERROR: ')' or WHERE is expected after the table name.");
							} 
						}
					}else{
						error = true;
						displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: A valid table name format is expected after FROM");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: FROM is expected after '*'");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: '*' is expected after SELECT.");
		}
	}else{
		error = true;
		displayError("PARSING ERROR: SELECT is expected after the ");
	}
}

void selectWhereConditionClause(std::string query){
	if(DDL_sym == T_IDENT){
		if(check_column_name(viewTableName.c_str(),identName.c_str())){
			std::string tempColumnName = identName;
			selectWhereCond = identName + " ";
			DDL_getSymbol(query);
			if((DDL_sym == T_NEQ || DDL_sym == T_EQL || DDL_sym == T_LSS || DDL_sym == T_GTR || DDL_sym == T_LEQ || DDL_sym == T_GEQ) && !error){
				prevSymbol = DDL_sym;
				selectWhereCond = selectWhereCond + getReserveCharacter(DDL_sym) + " ";
				DDL_getSymbol(query);
				if(DDL_sym == T_NUMBER && check_data_type(viewTableName.c_str(),tempColumnName,"int")){
					isWithArithGroup = false;
					selectWhereCond = selectWhereCond + " " + parseArithmeticExpression(query, "int");
					if(!error){
						charPos = prevCharPos;
						DDL_getSymbol(query);
					}
				}else if (DDL_sym == T_DECIMAL && check_data_type(viewTableName.c_str(), tempColumnName, "float")){
					isWithArithGroup = false;
					selectWhereCond = selectWhereCond + " " + parseArithmeticExpression(query, "float");
					if(!error){
						charPos = prevCharPos;
						DDL_getSymbol(query);
					}
				}else if(DDL_sym == T_LPAREN){
					isWithArithGroup = true;
					openParen = 0;
					selectWhereCond = selectWhereCond + " " + parseArithmeticExpression(query, check_data_type(viewTableName.c_str(),tempColumnName,"int")?"int":"float");
					if(!error){
						charPos = prevCharPos;
						DDL_getSymbol(query);
					}
				}else if(DDL_sym == T_APOSTROPHE && check_data_type(viewTableName.c_str(),tempColumnName,"varchar")){
					DDL_getString(query);
					if(DDL_sym == T_STRING){
						DDL_getSymbol(query);
						if(DDL_sym == T_APOSTROPHE){
							selectWhereCond = selectWhereCond + " '" + tempString + "'";
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: A end quote is expected after the string in the where condition");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: A string is expected in the where condition");
					}
				}else if(DDL_sym == T_APOSTROPHE && check_data_type(viewTableName.c_str(),tempColumnName,"char") ){
					DDL_getCharac(query);
					if(DDL_sym == T_CHARACTER){
						DDL_getSymbol(query);
						if(DDL_sym == T_APOSTROPHE){
							selectWhereCond = selectWhereCond + " '" + tempString + "'";
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: A end quote is expected after the character in the where condition");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: A character is expected in the where condition");
					}
				}else if(DDL_sym == T_NUMBER && (numberValue == "0" || numberValue == "1") && check_data_type(viewTableName.c_str(),tempColumnName,"boolean") ){
					if(prevSymbol == T_EQL || prevSymbol == T_NEQ){
						selectWhereCond = selectWhereCond + numberValue;
						DDL_getSymbol(query);
					}else{
						error = true;
						displayError("PARSING ERROR: Invalid operator for boolean data type");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: A arithmetic expression, string or boolean  should be followed after the opertor and type should be compatible in the where condition.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A correct operator should be followed after the " + checkCond);
			}
		}else{
			error = true;
			displayError("PARSING ERROR: The column "+identName+" is not declared in table " +viewTableName);
		}
	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name format is expected after WHERE");
	}
}